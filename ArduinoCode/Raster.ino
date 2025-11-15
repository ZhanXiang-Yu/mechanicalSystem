#include "funcs.h"
//maybe need free RTOS

String cmd = "-1";

void setup() 
{
  /*
  //testing stuff here
  Serial.begin(115200);
  while(!Serial){;}
  Serial.print("\n\nConnected!\n");
  */
  
  pinInit();
  serialStart();

}

void loop() 
{
  if (Serial.available() > 0)
  {
    cmd = Serial.readStringUntil('\n');
  }
  if(cmd == "CL")
  {
    testMoveL();
    cmd = "-1";
  }

  if(cmd == "CR")
  {
    testMoveR();
    cmd = "-1";
  }

  if(cmd == "CU")
  {
    testMoveU();
    cmd = "-1";
  }

  if(cmd == "CD")
  {
    testMoveD();
    cmd = "-1";
  }

  if(cmd == "TEST")
  {
    Serial.println("testing");
    raster();
    returnHome();

    
    cmd = "-1";
  }

  
}
