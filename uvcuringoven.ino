/**
 * TODO::
 * Refactor nomi nelle librerie
 * Se mi sento abbastanza bolide stostituirò le librerie con degli oggetti
 * 
 */

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

/**
   è arrivata l'ideona:
   creo una matrice di caratteri e ogni cliclo main la carico nel lcd
   magari faccio un altro timerino che refresha il display non proprio ogni main
   ->soluzione rutta è un contatore tipo ogni 10 aggiorno
   ->la soluzione bella è un timer ctc a 0,1sec

*/
/*char MSG[][16] = {//ARRAY di stringhe da 16 caratteri
  "STATE: BASE",
  "STATE: START",
  "STATE: WORK",
  "STATE: STOP",
  "STATE: TIME",
  "STATE: TIME UP",
  "STATE: TIME DOWN",
};

char lcdOutput[2][16];
void lcdOutputFn(char *r1,char *r2) {
  int l_r1;
  int l_r2;
  //forse controllo sulla lunghezza delle stringhe
  sprintf(lcdOutput[0],"%-16s",r1);
  sprintf(lcdOutput[1],"%-16s",r2);
};*/


#include "gmTime.h"
#include "gmTimer.h"

#include "com.h"
#include "button.h"

#define STATE_BASE      0
#define STATE_START     1
#define STATE_WORK      2
#define STATE_STOP      3
#define STATE_TIME      4
#define STATE_TIME_UP   5
#define STATE_TIME_DOWN 6

/*
   PORTB
   PB0 PB1 PB2 PB3 PB4 PB5 PB6 PB7
    53  52  51  50  10  11  12  13
*/
/*
  PORTL
  PL0 PL1 PL2 PL3 PL4 PL5 PL6 PL7
   49  48  47  46  45  44  43  42
*/

OneSecondTimer timer;
char state;
Time objTime,objTimeOne;
Button btnOne, btnTwo, btnTimeUp, btnTimeDown;



void setup() {
  /*
     PIN 13 : D-Out
     PIN 53 : D-In
     PIN 52 : D-In
  */
  DDRB  &= B11110000;
  PORTB |= B00001111;
  DDRL  = B11111111;
  PORTL = B00000010;

  btnOne = btnCreate(&PINB, PB0);
  btnTwo = btnCreate(&PINB, PB1);
  btnTimeUp = btnCreate(&PINB, PB2);
  btnTimeDown = btnCreate(&PINB, PB3);
  objTime = Time();
  objTimeOne = Time();

  state = STATE_BASE;
  timer = OneSecondTimer();
  timer.stop();

  
  setup_com();
  state = 0;
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Il Fornaio UV :)");
  lcd.setCursor(0, 1);
  lcd.print("STATE: BASE");

}

void loop() {

  btnUpdate(&btnOne);
  btnUpdate(&btnTwo);
  btnUpdate(&btnTimeUp);
  btnUpdate(&btnTimeDown);

  switch (state) {
    case STATE_BASE:
      lcd.setCursor(0, 0);
      lcd.print("Il Fornaio UV :)");
      lcd.setCursor(0, 1);
      lcd.print("STATE: BASE     ");
      state_base();
      break;
    case STATE_START:
      lcd.setCursor(0,0);
      lcd.print("STATE: START    ");
      lcd.setCursor(0,1);
      lcd.print("                ");
      state_start();
      break;
    case STATE_WORK:
      lcd.setCursor(0,0);
      lcd.print("STATE: WORK     ");
      lcd.setCursor(0,1);
      lcd.print(objTimeOne.toStringHM());
      state_work();
      break;
    case STATE_STOP:
      lcd.setCursor(0,0);
      lcd.print("STATE: STOP     ");
      lcd.setCursor(0,1);
      lcd.print("                ");
      state_stop();
      break;
    case STATE_TIME:
      lcd.setCursor(0, 0);
      lcd.print("STATE: TIME     ");
      state_time();
      lcd.setCursor(0, 1);
      lcd.print(objTime.toStringHM());
      break;
    case STATE_TIME_UP:
      lcd.setCursor(0, 0);
      lcd.print("STATE: TIME UP  ");
      state_time_up();
      lcd.setCursor(0, 1);
      lcd.print(objTime.toStringHM());
      break;
    case STATE_TIME_DOWN:
      lcd.setCursor(0, 0);
      lcd.print("STATE: TIME DOWN");
      state_time_down();
      lcd.setCursor(0, 1);
      lcd.print(objTime.toStringHM());
      break;
  }
}


void state_base() {
  if (btnOne._signal && !btnTwo._signal) {
    if (objTime.toSeconds() > 0)state = STATE_START;
    else state = STATE_TIME;//PICCOLO BUG LOGICO MA PER ORA ME LO TENGO ... direbbero pocomale i sommi
  }
  if (btnTimeUp._signal || btnTimeDown._signal) {
    state = STATE_TIME;
    timer.start();
  }
}

void state_start() {
  timer.start();
  state = STATE_WORK;
  PORTL ^= 3;
}
void state_work() {
  if (timer.getSignal()) {
    objTimeOne = objTime;
    objTimeOne.decSeconds(timer.getSeconds());
  }
  if ((timer.getSeconds() == objTime.toSeconds()) || btnTwo._signal) state = STATE_STOP;
}
void state_stop() {
  timer.stop();
  state = STATE_BASE;
  PORTL ^= 3;
}

void state_time() {
  if (btnTimeUp._signal && btnTimeDown._signal) return;
  else {
    if (btnTimeUp._signal) {
      state = STATE_TIME_UP;
      timer.start();
    }
    if (btnTimeDown._signal) {
      state = STATE_TIME_DOWN;
      timer.start();
    }
  }
  if (btnOne._signal) {
    state = STATE_BASE;
    while (btnOne._signal) btnUpdate(&btnOne); //NON E' TROPPO BELLO PER STARE IN UNA SPECIE DI MACCHINA A STATI
  }
}
void state_time_up() {
  int up_s = pow(timer.getSeconds() + 1, 2);
  if(objTime.lessEqual( 2, 59, 60 - up_s)) {
    objTime.incSeconds(up_s);
  }
  if (!btnTimeUp._signal) {
    state = STATE_TIME;
    timer.stop();
  }
}
void state_time_down() {
  int dw_s = pow(timer.getSeconds() + 1, 2);
  if (objTime.aboveEqual(0, 0, dw_s)) {
    objTime.decSeconds(dw_s);
  }
  if (!btnTimeDown._signal) {
    state = STATE_TIME;
    timer.stop();
  }
}



