#ifndef FUNCS_H
#define FUNCS_H

#include <Arduino.h>

//set pins
// Define stepper motor connections:
#define dirPinx 8
#define stepPinx 7
#define dirPiny 6
#define stepPiny 5
#define stepPinp 9 //trigger not used currently 
#define xhome 11
#define yhome 10


//motor/grid fixed params
const int microStep = 40; //40um (micrometers)
//arbitary delay for now till get radar params for sync
const int arbitaryDelayForStop = 2000; //2s

//Roughly 40x40 cm Max grid size 
//Currently using 30x30 cm 
const int dimX = 400000; //30 cm ~ 300000 um
const int dimY = 400000;

//TODO set a fixed values here
//dx, dy, pointsX, pointsY, not shared, should be fine
const int SARstepXUI = -1;
const int SARstepYUI = -1;
const int pointsX = -1;
const int pointsY = -1; 
const int stepDelay = -1;



void move_x(int SARstepX);
void move_y(int SARstepY);

//TODO
void step_x();
void step_y();




void stopSync(); //arbitary delay rn



//TODO later
void returnToOrigin();
bool returnToOriginValidation();


void setDirectionXLeft();
void setDirectionXRight();
void setDirectionYUp();
void setDirectionYDown();


#endif