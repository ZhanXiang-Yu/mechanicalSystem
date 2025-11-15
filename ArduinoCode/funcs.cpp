#include "funcs.h"

State state = MECH_MOVE;

stateHandler stateHandlers[STATE_COUNT] =
{
  moveDeltaX,
  sync,
  waiting
};

bool appDone = false;

//setup funcs
void pinInit()
{
    // Declare pins as output:
    pinMode(STEP_X, OUTPUT);
    pinMode(DIR_X, OUTPUT);
    pinMode(STEP_Y, OUTPUT);
    pinMode(DIR_Y, OUTPUT);

    //not used
    pinMode(TRIGGER, OUTPUT);
    pinMode(XHOME, INPUT_PULLUP);
    pinMode(YHOME, INPUT_PULLUP);

    digitalWrite(XHOME,LOW);
    digitalWrite(XHOME,LOW);
    digitalWrite(TRIGGER, LOW);


    digitalWrite(DIR_X, HIGH);
    digitalWrite(DIR_Y, HIGH);
    // Set all pin states
    digitalWrite(STEP_X, LOW);
    digitalWrite(STEP_Y, LOW);
}

void serialStart()
{
    Serial.begin(115200);
    while(!Serial){;}
    Serial.print("\n\nConnected!\n");

    Serial.println("pin init complete");

    Serial.print("Delta x: ");
    Serial.print(DELTA_X, 5);
    Serial.print("\n");

    Serial.print("Delta y: ");
    Serial.print(DELTA_Y, 5);
    Serial.print("\n");

    Serial.print("raster steps x: ");
    Serial.print(RASTER_STEPS_X);
    Serial.print("\n");

    Serial.print("raster steps y: ");
    Serial.print(RASTER_STEPS_Y);
    Serial.print("\n");

    Serial.print("motor steps x: ");
    Serial.print(MOTOR_STEPS_X);
    Serial.print("\n");

    Serial.print("motor steps y: ");
    Serial.print(MOTOR_STEPS_Y);
    Serial.print("\n");

    delay(500);

}

void moveDeltaX()
{
    Serial.println("curr state: MECH_MOVE");
    for(int i = 0; i < MOTOR_STEPS_X; i++)
    {
        digitalWrite(DIR_X, HIGH);
        digitalWrite(STEP_X, HIGH);
        delayMicroseconds(PWM_DELAY);
        digitalWrite(STEP_X, LOW);
        delayMicroseconds(PWM_DELAY);
    }
    Serial.println("movement completed");
    Serial.println();
    state = DATA_ACQ;
}

void moveDeltaY()
{
    Serial.println("Y MOVEMENT");
    for(int i = 0; i < MOTOR_STEPS_Y; i++)
    {
        digitalWrite(DIR_Y, LOW);
        digitalWrite(STEP_Y, HIGH);
        delayMicroseconds(PWM_DELAY);
        digitalWrite(STEP_Y, LOW);
        delayMicroseconds(PWM_DELAY);
    }
    Serial.println("movement completed");
    Serial.println();
}

void stateMachineX()
{
    for(int i = 0; i < STATE_COUNT; i++)
    {
        stateHandlers[i]();
    }
}

void returnToBeginningRow()
{
    Serial.println("returning to begin row");
    Serial.println();
    //move entire row
    for(int i = 0; i < RASTER_STEPS_X; i++)
    {
        Serial.println(i);
        //move 1 delta x
        for(int i = 0; i < MOTOR_STEPS_X; i++)
        {
            //set dir x pin to opposite -> left
            digitalWrite(DIR_X, LOW);
            digitalWrite(STEP_X, HIGH);
            delayMicroseconds(PWM_DELAY);
            digitalWrite(STEP_X, LOW);
            delayMicroseconds(PWM_DELAY);
        }
    }
}

/* dont need
void stateMachineY()
{
    switch(state)
    {
        case MECH_MOVE:
            moveDELTA_Y();
            state = DATA_ACQ;
            break;
        
        case DATA_ACQ:
            sync();
            state = WAIT;
            break;
        
        case WAIT:
            waiting();
            state = MECH_MOVE;
            break;
    }

}
*/

void sync()
{
    //send a "high" to start the application -> for testing write to a file -> mockApplication()
    Serial.println("curr state: DATA_ACQ");
    mockApplication();
    state = WAIT;
}

void mockApplication()
{
    /*
    std::string fileName = MOCK_APPLICATION_FILEPATH + std::to_string(currStep);
    // 1) Create and open the file
    std::ofstream outFile(fileName);
    if (!outFile) {
        std::cerr << "Failed to open output.txt for writing\n";
        return 1;
    }

    // 2) Write a set of contents
    outFile << "Mock Application\n";
    outFile << "current step is: \n";
    outFile << currStep << "\n";

    // 3) Close the file
    outFile.close();
    */
    //delay and print to serial when complete
    Serial.println("count down 3");
    delay(200);
    Serial.println("count down 2");
    delay(200);
    Serial.println("count down 1");
    delay(200);
    Serial.println("DONE");
    Serial.println();
    appDone = true;
}


void waiting()
{
    if(appDone == true)
    {
        state = MECH_MOVE;
        Serial.println("curr state: WAIT");

        //delay for data processing or whatever
        Serial.println("count down 3");
        delay(200);
        Serial.println("count down 2");
        delay(200);
        Serial.println("count down 1");
        delay(200);
        Serial.println("DONE");
        
        Serial.println("Waiting completed");
        Serial.println();
        appDone = false;
    }
    
}

float position()
{
    return -1;
}

void raster()
{
    for(int currColStep = 0; currColStep < RASTER_STEPS_Y; currColStep++)
    {
        Serial.print("curr col step: ");
        Serial.println(currColStep);
        Serial.println();
        for(int currRowStep = 0; currRowStep < RASTER_STEPS_X; currRowStep++)
        {
            Serial.print("curr row step: ");
            Serial.println(currRowStep);
            Serial.println();
            if(currRowStep == 0)
            {
                //take measurements only
                sync();
                waiting();
                stateMachineX();
                continue;
            }
            stateMachineX();
        }
        
        //move 1 y up
        moveDeltaY();
        //return to the start of row to do left->right
        returnToBeginningRow();
    }
    Serial.println("raster scan finished");
}

void returnHome()
{
    Serial.println("return to origin");

    //move all the way down
    for(int currColStep = 0; currColStep < RASTER_STEPS_Y; currColStep++)
    {
        for(int i = 0; i < MOTOR_STEPS_Y; i++)
        {
            digitalWrite(DIR_Y, HIGH);
            digitalWrite(STEP_Y, HIGH);
            delayMicroseconds(PWM_DELAY);
            digitalWrite(STEP_Y, LOW);
            delayMicroseconds(PWM_DELAY);
        }
    }
    Serial.println("returned");
    Serial.println();
}

void testMoveL()
{
    for(int i = 0; i < 10000; i++)
    {
        digitalWrite(DIR_X, LOW);
        digitalWrite(STEP_X, HIGH);
        delayMicroseconds(PWM_DELAY);
        digitalWrite(STEP_X, LOW);
        delayMicroseconds(PWM_DELAY);
    }

    delay(500);
    Serial.println("calibrated moving left");
}

void testMoveR()
{
    for(int i = 0; i < 10000; i++)
    {
        digitalWrite(DIR_X, HIGH);
        digitalWrite(STEP_X, HIGH);
        delayMicroseconds(PWM_DELAY);
        digitalWrite(STEP_X, LOW);
        delayMicroseconds(PWM_DELAY);
    }

    delay(500);
    Serial.println("calibrated moving right");
}

void testMoveU()
{
    for(int i = 0; i < 10000; i++)
    {
        digitalWrite(DIR_Y, LOW);
        digitalWrite(STEP_Y, HIGH);
        delayMicroseconds(PWM_DELAY);
        digitalWrite(STEP_Y, LOW);
        delayMicroseconds(PWM_DELAY);
    }

    delay(500);
    Serial.println("calibrated moving up");
}

void testMoveD()
{
    for(int i = 0; i < 10000; i++)
    {
        digitalWrite(DIR_Y, HIGH);
        digitalWrite(STEP_Y, HIGH);
        delayMicroseconds(PWM_DELAY);
        digitalWrite(STEP_Y, LOW);
        delayMicroseconds(PWM_DELAY);
    }

    delay(500);
    Serial.println("calibrated moving down");
}
