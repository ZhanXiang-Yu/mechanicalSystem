/*
Goal: 
write a programming assignment latex style from your old CS class so that you can divide up tasks to people to finish this
write your own function headers/description, describe structure

Mechanical System:
Make sure Arduino code move with efficiency in a cartesian coordinate system

Arduino Uno R3, 2 stepper motors with driver DM542
Stepper motor <-> cartesian coordinate:
Dir +, DIR - -> CW, CCW
Pul +, Pul - -> PWM for steps
How to get position -> need an encoder?

Syn. Aperture = dimX * dimY
T to finish dimX = (dimX / Dist(1 microstep)) * freq. of pul +
freq. of pul + = microstep/s 

To do a full scan: 
one full horizontal, move vertically by y, one full horizontal, do so discretely -> given X = {x1, x2, … xn}, move from xi to xi+1, stop/sync, repeates, i <= n
same thing for y but after one full horizontal scan
parameters: dx, dy, pointsX, pointsY

relate above params. to motor params: It relates to how fast you can sync -> Walabot triggers, motor driver power constraints

dx = dimX / pointsX -> relate to radar params.(unknown)
= N{1,2,3,…} * # of microsteps
T to finish dx = N * freq. of pul +

Y axis is more arbitrary, but the same, need to pause when it moves up

0.04 mm per step, 1.8 deg per step, 360 deg(duh)

*/

#include "funcs.h"
#include <Arduino.h>


//init. params.
void setup()
{
  // Declare pins as output:
  pinMode(stepPinx, OUTPUT);
  pinMode(dirPinx, OUTPUT);
  pinMode(stepPiny, OUTPUT);
  pinMode(dirPiny, OUTPUT);
  pinMode(stepPinp, OUTPUT); //trigger not used for now

  //return func
  pinMode(xhome, INPUT_PULLUP);
  pinMode(yhome, INPUT_PULLUP);

  //set default directions right and up
  setDirectionXRight();
  setDirectionYUp();

  //set PUL states, not moving in init. stage
  digitalWrite(stepPinx, LOW);
  digitalWrite(stepPiny, LOW);
  digitalWrite(stepPinp, LOW); //trigger not used  for now
  
  //setup serial terminal
  Serial.begin(115200);
  delay(500);
  Serial.print("\n\n\n");
  delay(500);
  Serial.print("\n\nConnecting...\n");
  delay(500);
  Serial.print("Connected!\n");
  delay(2000); // delay 3s on start
}


void loop()
{
 //TODO: just write a superloop here, reference PA.pdf
}