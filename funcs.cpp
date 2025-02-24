//TODO: all arduino stuff

#include "funcs.h"
#include <Arduino.h>


//motor/grid fixed params
const int microStep = 40; //40um (micrometers)
//arbitary delay for now till get radar params for sync
const int arbitaryDelay = 1000; //1s

//Roughly 40x40 cm Max grid size 
//Currently using 30x30 cm 
const int dimX = 300000; //30 cm ~ 300000 um
const int dimY = 300000;

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
    Serial.print("Enter total desired (X Y) points: ");
    while(valid == false)
    {
        if(Serial.available() > 0)
        {
            tempX = Serial.parseInt();
            tempY = Serial.parseInt();
            valid = initUIValidation(tempX, tempY);
            if(valid == true)
            {
                //Compute steps based on Points input
                computeSARParams(tempX, tempY);
                break;
            }
            else
            {
                Serial.print("\nInvalid step input\n");
                Serial.println("x: ");
                Serial.println(SARstepXUI);
                Serial.print("\ny: ");
                Serial.println(SARstepYUI);
                resetSARParams();
            }
        }
    }
    Serial.print("SAR dx: ");
    Serial.println(SARstepXUI);  
    Serial.print("SAR dy: ");
    Serial.println(SARstepYUI); 
    Serial.print("pointsX: ");
    Serial.println(pointsX);
    Serial.print("pointsY: ");
    Serial.println(pointsY); Serial.print(" \n");
}

void stopSync() //arbitary delay rn
{
    delayMicroseconds(arbitaryDelay);
}

//TODO: below are the functions  that need to be implemented and modified
/*

*/
#define STEP_DELAY 19 
// Preset Delay can be changed, 19us for fastest PWM

//Fastest PWM is 25600 for 128 microsteps 
void step_x()
{
    
    digitalWrite(stepPinx, HIGH);
    delayMicroseconds(STEP_DELAY);
    digitalWrite(stepPinx, LOW);
    delayMicroseconds(STEP_DELAY);
}

void step_y()
{
    digitalWrite(stepPiny, HIGH);
    delayMicroseconds(STEP_DELAY);
    digitalWrite(stepPiny, LOW);
    delayMicroseconds(STEP_DELAY);
}

/*
TODO: Adjust to update points then calculte steps.
      (i.e. SARstepXUI = (dimX / pointsX) / microStep)
      should calculate steps, or the number of times step_x should be 
      looped to get # of discrete points input 
*/
void computeSARParams(int pX, int pY)
{
    // Error handling
    if (!initUIValidation(SARstepX, SARstepY)){
        return;
    }

    // Updates:
        // SARstepXUI , SARstepYUI , pointsX , pointsY

    SARstepXUI = SARstepX;
    SARstepYUI = SARstepY;

    // Points = um / um
    pointsX = dimX / SARstepX;
    pointsY = dimY / SARstepY;
}

/*
TODO: Update to validate points instead of steps.
      points should be between 0 - 7500 
      Greater than 7500 points would require smaller than 40 um (not possible)
*/
bool initUIValidation(int pX , int pY)
{
    // Check:
        // Step size > micro step size
        // Step size < dimensions
        // TODO: others?
        
    // Step size > micro step size
    if (!(SARstepX > microStep)){return 0;}
    if (!(SARstepY > microStep)){return 0;}
    
    // Step size < dimensions
    if (!(SARstepX < dimX)){return 0;}
    if (!(SARstepY < dimY)){return 0;}


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
    while(SARstepYUI > 0)              // while y != 0
    {
      step_y();                         // create a motor movement down   
      SARstepYUI--;                     // decrement variable to indicate new coordinate position
    }
    while(SARstepXUI > 0)              // while x != 0
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
TODO: Did not compile. Debug & Verify ability to compile.
      Validation & ComputeSARParams removed, done previously so not necessary.
      Utilize move_x & move_y
      SetDirectionXLeft() can be used to reverse direction

*/



void SARScan(){
	SetDirectionXRight();
	SetDirectionYUp();
	//loop across the entire grid of points as calculated above
	for (int i = 0; i < pointsY; i++){
		move_x(pointsX);
		if (i%2 == 0){
			SetDirectionXLeft();
		} else {
			SetDirectionXRight();
		}
		move_y(1);
	}
	returnToOrigin();
	bool checker = false;
	while (!checker){
		checker = returnToOriginValidation();
	}
	return;
}
