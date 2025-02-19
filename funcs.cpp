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
extern int SARstepXUI = -1;
extern int SARstepYUI = -1;
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

void move_x(int SARstepX)
{
    for(int i = 0; i < SARstepX; i++)
    {
        step_x();
    }
}

void move_y(int SARstepY)
{
    for(int i = 0; i < SARstepY; i++)
    {
        step_y();
    }
}

void resetSARParams()
{
    SARstepXUI = -1;
    SARstepYUI = -1;
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
            SARstepXUI = Serial.parseInt();
            SARstepYUI = Serial.parseInt();
            valid = initUIValidation(SARstepXUI, SARstepYUI);
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
    Serial.print(SARstepXUI); Serial.print(" "); 
    Serial.print(SARstepXUI); Serial.print(" ");
    Serial.print(SARstepXUI); Serial.print(" ");
    Serial.print(SARstepXUI); Serial.print(" \n");
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
void computeSARParams(int SARstepX, int SARstepY)
{
    // Error handling
    if (!initUIValidation(SARstepX, SARstepY)){
        return;
    }

    // Updates:
        // SARstepXUI , SARstepYUI , pointsX , pointsY

    SARstepXUI = SARstepX;
    SARstepYUI = SARstepY;

    // Points = cm / cm
    pointsX = dimX / SARstepX;
    pointsY = dimY / SARstepY;
}

/*

*/
bool initUIValidation(int SARstepX , int SARstepY)
{
    // Check:
        // Step size > micro step size
        // Step size < dimensions
        // TODO: others?
        
    // Step size > micro step size
    if (SARstepX < microStep){return 0;}
    if (SARstepY < microStep){return 0;}
    
    // Step size < dimensions
    if (SARstepX > dimX){return 0;}
    if (SARstepY > dimY){return 0;}


    // If all is good, then return true
    return 1;
}

/*
    - resultToOrigin moves the mechanized system back to (0,0) on the generated coordinate plane
    - first while loop moves the mechanized system down until y = 0
    - second while loop moves the mechanized system left until x = 0
    - assumes that step_y and step_x do not update coord. pos of mechanized system & SARstepUI is mechanized system pos
*/
void returnToOrigin()
{
    setDirectionXLeft();                // set Y direction
    setDirectionYDown();                // set X direction
    while(SARstepYUI != 0)              // while y != 0
    {
      step_y();                         // create a motor movement down   
      SARstepYUI--;                     // decrement variable to indicate new coordinate position
    }
    while(SARstepXUI != 0)              // while x != 0
    {
      step_x();                         // create a motor movement down        
      SARstepXUI--;                     // decrement variable to indicate new coordinate position
    }
}
/*
    - boolean returns TRUE if mechanized system is back at (0,0) and FALSE otherwise
*/
bool returnToOriginValidation()
{
    if((SARstepXUI == 0)&&(SARstepYUI == 0)){ // determine if mechanized system is at the origin
      return True;
    }
    else{
      return False;
    }
}

/*

*/
void SARScan()
{

}