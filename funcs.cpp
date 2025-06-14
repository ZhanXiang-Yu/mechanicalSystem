#include "funcs.h"
#include <Arduino.h>


//DO NOT CHANGE
//-----
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
void stopSync() //arbitary delay rn
{
    delayMicroseconds(arbitaryDelay);
}
//-----


//TODO
void step_x()
{
    
    digitalWrite(stepPinx, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(stepPinx, LOW);
    delayMicroseconds(stepDelay);
}

void step_y()
{
    digitalWrite(stepPiny, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(stepPiny, LOW);
    delayMicroseconds(stepDelay);
}


/*
    - resultToOrigin moves the mechanized system back to (0,0) on the generated coordinate plane
    - first while loop moves the mechanized system down until y = 0
    - second while loop moves the mechanized system left until x = 0
    - assumes that step_y and step_x do not update coord. pos of mechanized system & SARstepUI is mechanized system pos
*/
void returnToOrigin()
{
    //TODO: later
}
/*
    - boolean returns TRUE if mechanized system is back at (0,0) and FALSE otherwise
*/
bool returnToOriginValidation()
{
   //TODO: later
}
