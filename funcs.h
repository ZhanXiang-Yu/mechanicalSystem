#ifndef FUNCS_H
#define FUNCS_H

#include "Arduino.h"

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
extern const int microStep; //4mm

//TODO: get vals
extern const int dimX;
extern const int dimY;

//dx, dy, pointsX, pointsY, not shared, should be fine
extern int SARstepXUI;
extern int SARstepYUI;
extern int pointsX;
extern int pointsY;

//arbitary delay for now till get radar params for sync
extern const int arbitaryDelay; //1s

void move_x(int SARstepX);

//TODO
void step_x();
void step_y();

void move_y(int SARstepY);

//TODO
void computeSARParams();

//TODO
bool initUIValidation(int dx, int dy);

void initUI();

//TODO
void returnToOrigin();
bool returnToOriginValidation();

void stopSync(); //arbitary delay rn

void setDirectionXLeft();
void setDirectionXRight();
void setDirectionYUp();
void setDirectionYDown();

void resetSARParams();

//TODO
void SARScan();

#endif