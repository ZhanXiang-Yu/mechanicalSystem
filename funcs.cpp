//TODO: all arduino stuff

#include "funcs.h"
#include "Arduino.h"


//motor/grid fixed params
const int microStep = 0.004; //4mm
//arbitary delay for now till get radar params for sync
const int arbitaryDelay = 1000; //1s

//TODO: get vals
const int dimX = -1;
const int dimY = -1;

//dx, dy, pointsX, pointsY, not shared, should be fine
extern int SARStepXUI = -1;
extern int SARStepYUI = -1;
extern int pointsX = -1;
extern int pointsY = -1;

void setDirectionXLeft()
{
    digitalWrite(dirPinx, LOW); //high = right, low = left
}
void setDirectionXRight()
{
    digitalWrite(dirPinx, HIGH); //high = right, low = left
}
void setDirectionYUp()
{
    digitalWrite(dirPiny, LOW); //low = up, high = down
}
void setDirectionYDown()
{
    digitalWrite(dirPiny, HIGH); //low = up
}

void move_x(int SARStepX)
{
    for(int i = 0; i < SARStepX; i++)
    {
        step_x();
    }
}

void move_y(int SARStepY)
{
    for(int i = 0; i < SARStepY; i++)
    {
        step_y();
    }
}

void resetSARParams()
{
    SARStepXUI = -1;
    SARStepYUI = -1;
    pointsY = -1;
    pointsY = -1;
}

void initUI()
{
    bool valid = false;
    Serial.print("enter SAR step size in x/y in int format in this order: dx dy\n");
    while(valid == false)
    {
        if(Serial.available() > 0)
        {
            SARStepXUI = Serial.parseInt();
            SARStepYUI = Serial.parseInt();
            valid = initUIValidation(SARStepXUI, SARStepYUI);
            if(valid == true)
            {
                //computation for pointsY and pointsY
                computeSARParams();
                break;
            }
            else
            {
                resetSARParams();
                Serial.print("invalid SAR step size x/y input\n");
            }
        }
    }
    Serial.print("SAR dx SAR dy pointsY pointsY are: \n");
    Serial.print(SARStepXUI); Serial.print(" "); 
    Serial.print(SARStepXUI); Serial.print(" ");
    Serial.print(SARStepXUI); Serial.print(" ");
    Serial.print(SARStepXUI); Serial.print(" \n");
}

void stopSync() //arbitary delay rn
{
    delayMicroseconds(arbitaryDelay);
}

//TODO: below are the functions  that need to be implemented and modified
/*

*/
void step_x()
{
    digitalWrite(stepPinx, HIGH);
    //TODO: do PWM as fast as possible, check motor spec sheet
    delayMicroseconds(-1);
}

/*

*/
void step_y()
{
    digitalWrite(stepPiny, HIGH);
    //TODO: do PWM as fast as possible, check motor spec sheet
    delayMicroseconds(-1);
}

/*

*/
void computeSARParams()
{
    
}

/*

*/
bool initUIValidation(int dx, int dy)
{

}

/*

*/
void returnToOrigin()
{

}
/*

*/
bool returnToOriginValidation()
{

}

/*

*/
void SARScan()
{

}