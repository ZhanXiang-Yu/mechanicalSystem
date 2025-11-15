#ifndef FUNCS_H
#define FUNCS_H

#include <Arduino.h>

//not sure if need this one
#include <Stepper.h>

//Arduino does not contain the standard library of C++ on PC
//TODO: when using NANOVNA saver, need to write a separte script(it was the case) for PC
//for mock application
//#include <fstream>
//#include <iostream>

//for file path
//#include <string>

//if u do this, this is a pointer to a const char in a header, if use header in 2 diff. files(.ino,.cpp) -> multiple definitions error for a const type, because compiler is weird
// const char* MOCK_APPLICATION_FILEPATH = "TODO:putherefortesting";
//instead
#define MOCK_APPLICATION_FILEPATH "TODO:putherefortesting"

// pin stuff

/*
STEPY -> high move a step
DIRY -> high -> CW -> down, low -> CCW -> up 
STEPX -> high move a step
DIRX -> high -> CW -> right, low -> CCW -> left 
*/

const int DIR_X      = 8;
const int STEP_X     = 7;
const int DIR_Y      = 6;
const int STEP_Y     = 5;

const int TRIGGER   = 9; //mmwave sensor trigger, not used doesnt matter
const int XHOME    = 11; //used to return home, not relevant, do ur own way
const int YHOME    = 10; //used to return home, not relevant, do ur own way

// raster params
//unit is m
//TODO: 7.4 um for 1110(roughly), chanage val when designing new mech.
const double MOTOR_STEPSIZE = 7.4e-6; 
const double MAX_X = 2.5e-1; //25 cm safe range
const double MAX_Y = 1.5e-1; //19 cm safe range

//from spec -> The width of PUL signal is at least 1.0μs, duty cycle is recommended 50%
const int PWM_DELAY = 100; //us

//TODO: delta x,y put based on SAR theory
//just use values good for testing mech. sys rn
const double DELTA_X = 3.7e-2;
const double DELTA_Y = 2.22e-2;
const int RASTER_STEPS_X = static_cast<int>(MAX_X / DELTA_X);
const int RASTER_STEPS_Y = static_cast<int>(MAX_Y / DELTA_Y);
const int MOTOR_STEPS_X = static_cast<int>(DELTA_X / MOTOR_STEPSIZE);
const int MOTOR_STEPS_Y = static_cast<int>(DELTA_Y / MOTOR_STEPSIZE);

//UI
extern String cmd;

//pipeline sync
extern bool appDone;

//setup funcs
void pinInit();
void serialStart();

//move 1 delta x, y step
void moveDeltaX();
void moveDeltaY();


//state machine for a single pipeline execution of move platform, data acq, wati till finish
enum State
{
  MECH_MOVE,
  DATA_ACQ,
  WAIT,
  STATE_COUNT
};
extern State state;
/*
corresponding funcs for each state
MECH_MOVE -> moveDeltaX/Y
DATA_ACQ -> sync(), TODO: figure out how to capturing, you can test by write to a file?
WAIT -> waiting(), use Serial polling when radar application(NANOVNA-Saver) is done or some other types of trigger -> TODO: figure out how to do/if NANOVNA-Saver can do this even
this is really cool, you can either use a switch statement to execute the state machine for one single execution, for more you need a loop
or you can use a function pointer, a function pointer array and run the loop
the 2nd approach increases readbility and scalibility
*/
void sync();
void waiting();

typedef void (*stateHandler) ();
extern stateHandler stateHandlers[STATE_COUNT];

void stateMachineX();





//when moving up a col step, move back to the other end of the row to do the same left right scan
void returnToBeginningRow();

//return to origin when raster scan is finished
void returnHome();

//mock application func, write to a file and send to serial something when done, also wait for 3 sec
void mockApplication();

//the main func
void raster();

//you know you can just do even odd rows to do left->right/right->left swap

//position encoding stuff
/*
TODO: I need to return the exact position(a grid like scenario) with how much moved(delta x,y) but also correspond to mech. motor move(encoder stuff)
do I acutally need?
*/
float position();


//TODO make sure at (0,0) when start
void initPosCalibrate();

//testing funcs

//move in dirs till driver give error(calibrate)
//hardcode steps to be 10000
void testMoveL();
void testMoveR();
void testMoveU();
void testMoveD();
#endif