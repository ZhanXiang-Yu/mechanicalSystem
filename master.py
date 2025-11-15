import serial
import time
import subprocess
from pathlib import Path

def startSerial(port, baud):
    ser = serial.Serial(port, baud, timeout=1)
    time.sleep(2)
    print("started serial terminal")
    return ser

"""
nested for loops to move
"""
def raster(xSteps: int, ySteps: int, command: str, printerSerial, moveX: str, moveY: str): #TODO: determine G code based on raster scan
    for i in range(ySteps):
        for j in range(xSteps):
            if(j == 0):
                runApplicationAndSync(command)
            move(printerSerial, moveX) #TODO: Gcode move in X
            runApplicationAndSync(command)
        move(printerSerial, moveY) #TODO: Gcode move in Y

"""
move based on GCode str, Gcode str is composed in Gcode()
"""
def move(printerSerial, GCode: str): #TODO: determine G code based on raster scan
    printerSerial.write(GCode.encode("ascii"))
    printerSerial.flush()


"""
start the applicationn thread, either VNA or TI stuff
"""
def runApplicationAndSync(command: str): #command is the command you will type on your terminal to run your application
    command = command.split()
    process = subprocess.Popen(command)
    print("application started")
    process.wait(timeout=10)#I am setting 10s to test
    if process.returncode == 0:
        print("application finished successfully")
    else:
        print("failed: " + process.stderr) #not sure the error stream works with application

"return to initial coordinate"
def returnHome(initXPos, initYPos, finalXPos, finalYPos):
    #TODO: based on the init./final pos in sarParams dictionary, write Gcode for the 3D printer to return
    pass

"""
based on user input, return a dictionary of followingn key-value pairs:
{
    "xSteps": 10,
    "ySteps": 20,
    "deltaX": 0.5,
    "deltaY": 0.5,
    "initXPos": 0,
    "initYPos": 0,
    "finalXPos": 5.0,
    "finalYPos": 10.0
}
values are random in the above
to access a dict do dict["keyname"]
"""
def parseUI():
    #TODO: what is unit of 3D printer, I used cm here
    keys1 = ["xSteps", "ySteps", "deltaX", "deltaY", "command"]
    keys2 = ["initXPos", "initYPos", "finalXPos, finalYPos"]
    argLst1 = input("enter xSteps, ySteps, deltaX, deltaY, CLI command to run application in cm with 2 sig figs: ").split()
    sarParams = dict(zip(keys1, argLst1))
    argLst2 = [
        0,
        0,
        round(sarParams["xSteps"] * sarParams["deltaX"],2),
        round(sarParams["ySteps"] * sarParams["deltaY"],2)
    ]
    sarParams.update(zip(keys2, argLst2))
    #TODO: input validation, what is the absolute dist of the grid, what would the printer behave if movement command is invalid
    
    #TODO: based on values from  parseUI, compose Gcode constants
    moveXG = ""
    moveYG = ""
    return sarParams, moveXG, moveYG, 


if __name__ == "__main__":
    """
    sarParams, moveXG, moveYG = parseUI()    
    printer=startSerial(-1,-1)
    raster(sarParams["xSteps"], sarParams["ySteps"], sarParams["command"], printer, moveXG, moveYG,)
    returnHome(sarParams["initXPos"], sarParams["initYPos"], sarParams["finalXPos"], sarParams["finalYPos"])
    #close serial?
    """
    
    #do testing here