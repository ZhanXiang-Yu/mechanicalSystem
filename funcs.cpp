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


void computeSARParams(int pX, int pY)
{
    // Error handling
    if (!initUIValidation(pX, pY)){
        // THROW ERROR HERE IF NEEDED LATER //
        return;
    }

    // Points updated globally
    pointsX = pX;
    pointsY = pY;    

    // Step size = (dim / points) / (microstep)
        // Essentially how many microsteps for one full step
    SARstepXUI = (dimX / pointsX) / microStep;
    SARstepYUI = (dimY / pointsY) / microStep;

    return;
}


bool initUIValidation(int pX , int pY)
{
    // Check:
        // pX, pY : (0, 7500]
        // TODO: others?
        
    // pX
    if (pX <= 0)  {return 0;}
    if (pX > 7500){return 0;}
    
    // pY
    if (pY <= 0)  {return 0;}
    if (pY > 7500){return 0;}


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
	//loop across the entire grid of points as calculated above
	for (int i = 0; i < pointsY; i++){
		for (int j = 0; j < pointsX; j++){
			step_x();
		}
		//NOTE: How can step_x be reversed? This is an inefficient placeholder
		returnToOrigin();
		for (int k = 0; k < j; k++){
			step_y();
		}
	}
	returnToOrigin();
	bool checker = false;
	while (!checker){
		checker = returnToOriginValidation();
	}
	return;
}
