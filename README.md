# FornoUV

## Dependencies
[Adafruit Sensor](https://github.com/adafruit/Adafruit_Sensor)

## Description

### Usage logic

* POWER ON:  
It turns on a green light.
* SELECTING TIME:  
It enters into the state pressing the button BTNTIME+ or BTNTIME- and releasing it. It returns in the base state pressing and releasing the button BTNSTART.  
Buttons work:
  * Click less than one second: increments/decreases by one minute;
  * Click more than one second: increments/decreases by a number of seconds as the duration of the pression.
* START WORK:  
From the base state, pressing the BTNSTART it starts the work. The servomotor self move in closed position and, when it arrives to position, will be turned on the work red light, the UV leds and the internal engine. The work continues until the time ends or the button STOP has been pressed. The button STOP resets the UV oven (turns off the red light, the internal engine, the UV leds and take the servomotor to the open position and at last turns on the green light).
* POWER OFF

### State Machine

![State Machine](https://raw.githubusercontent.com/STB1019/FornoUV/master/images/state%20machine%20-%20FornoUV.png)

![State Machine file](https://github.com/STB1019/FornoUV/tree/master/images/state%20machine%20-%20FornoUV.png "State Machine")

### Pin Connections

Pin | Use | Relè Connections
--- | --- | ----------------
8, 9, 4, 5, 6, 7 | LCD | 
51, 50 | BTNTIME+, BTNTIME- | 
49, 48 | Green LED, Red LED | 
36, 37 | Motor CW, Motor CCW | IN1, IN2
35, 34, 33, 32 | Resistances | IN3, IN4, IN5, IN6
31 | Fan | IN7
30 | UV LED | IN8
3 | Servomotor | 
43 | BTNSERVO | 
42 | Input DHT11 | 
53 | BTNSTART | 
52 | BTNSTOP | 

### Relè

![Relè](https://raw.githubusercontent.com/STB1019/FornoUV/master/images/rele%20image%20-%20FornoUV.png)

![Relè file](https://github.com/STB1019/FornoUV/tree/master/images/rele%20image%20-%20FornoUV.png "Rele")


NC:    NORMALLY CLOSED    →    ALL TO GROUND  
C:     COMMON  
NO:    NORMALLY OPENED    →    APPROPRIATE SUPPLY (engine -> 12V, …)

### Notes

The buttons have a pull up resistance inside to Arduino, so they have to be closed to ground.


## Ideas and Improvements

* Change the selection of the time, implementing an interface to choose indipendently the number of hours, minutes and seconds
