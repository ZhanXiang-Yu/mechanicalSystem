from copy import error
import serial
import time
import subprocess
from pathlib import Path
from gcodepy.gcode import Gcode

returnHome = "returnHome"
moveHorizontalX = "moveHorizontalX"
moveVerticalZ = "moveVerticalZ"

def startSerial(port, baud):
    ser = serial.Serial(port, baud, timeout=1)
    time.sleep(2)
    print("started serial terminal")
    return ser

"""
write G code -> returnHome, moveHorizontal->X, moveVertical->Z, based on stepsizes
"""
def homeG():
    g = Gcode("returnHome")
    g.home()
    g.zero_extruder()
    g.close()

def moveHorizontalG(stepsize: tuple[int, 0, 0]): #in mm
    g = Gcode("moveHorizontalX")
    g.zero_extruder()
    g.travel(stepsize)
    g.close()

def moveVerticalG(stepsize: tuple[0, 0, int]): #in mm
    g = Gcode("moveVerticalZ")
    g.zero_extruder()
    g.travel(stepsize)
    g.close()

"""
nested for loops to move
"""
def raster(xSteps: int, ySteps: int, command: str, printerSerial):
    
    for i in range(ySteps):
        for j in range(xSteps):
            if(j == 0):
                runApplicationAndSync(command)
            move(printerSerial, moveHorizontalX)
            runApplicationAndSync(command)
        move(printerSerial, moveVerticalZ)

"""
move based on GCode str, Gcode str is composed in Gcode()
"""
def move(printerSerialObj, GCodeFile: str): #TODO: determine G code based on raster scan
    #send Gcode file via serial
    with(GCodeFile, "r") as f:
        for line in f:
            line = line.strip()
            if not line or line.startwith(";"):
                continue
            G = (line + "\n").encode()
            printerSerialObj.write(G)
            printerSerialObj.flush()
            print("sent: ", G)


"""
start the applicationn thread, either VNA or TI stuff
"""
def runApplicationAndSync(command: str, timeout: float = None): #command is the command you will type on your terminal to run your application
    command = command.split()
    #there may be more error types I need to consider    
    try:
        process = subprocess.Popen(
            command,
            stdout=subprocess.PIPE, 
            stderr=subprocess.PIPE,
            text=True)
    except FileNotFoundError as e:
        print("invalid CLI cmd: ", e)
        return {
            "status": "invalid command",
            "returncode": None,
            "stdout": None,
            "stderr": f"Command not found: {command[0]}",
            "command": command
        }
    print("application started")
    try:
        out, err = process.communicate(timeout=timeout)
    except subprocess.TimeoutExpired:
        process.kill()
        out, err = process.communicate()
        print("application timed out")
        return {
            "status": "timeout",
            "returncode": process.returncode,
            "stdout": out,
            "stderr": err,
            "command": command
        }
    if process.returncode == 0:
        print("application finished successfully")
        status = "success"
    else:
        print("failed") #not sure the error stream works with application
        status = "failure"
    return {
            status: "timeout",
            "returncode": process.returncode,
            "stdout": out,
            "stderr": err,
            "command": command
        }

# return to initial coordinate
def returnHome(printerSerialObj):
    #send Gcode file via serial
    with(returnHome, "r") as f:
        for line in f:
            line = line.strip()
            if not line or line.startwith(";"):
                continue
            G = (line + "\n").encode()
            printerSerialObj.write(G)
            printerSerialObj.flush()
            print("sent: ", G)
    print("home returned")

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
def parseUI(): # in mm
    keys1 = ["xSteps", "ySteps", "deltaHorizontal", "deltaVertical", "command"]
    argLst1 = input("enter xSteps ySteps deltaHorizontal deltaYVertical CLI command to run application in cm with 2 sig figs: ").split()
    sarParams = dict(zip(keys1, argLst1))
    argLst2 = [
        0,
        0,
        round(sarParams["xSteps"] * sarParams["deltaX"],2),
        round(sarParams["ySteps"] * sarParams["deltaY"],2)
    ]
    #TODO: input validation, what is the absolute dist of the grid, what would the printer behave if movement command is invalid
    
    
    return sarParams


if __name__ == "__main__":
    sarParams = parseUI()    
    printer=startSerial(-1,-1) #find/set port/baud rate
    homeG()
    moveHorizontalG((float(sarParams["deltaHorizontal"]), 0 ,0))
    moveVerticalG((0, 0, float(sarParams["deltaVertical"])))
    
    raster(sarParams["xSteps"], sarParams["ySteps"], sarParams["command"], printer)
    returnHome(printer)
    #close serial?
    

    
    
    """
    test Python gcode module: write a Gcode script -> move abs 0.0,0, move action 1, ..., return home
    """
    
    """
    #testing application pipeline
    tests = [
        {
            "name": "Success (3s timer)",
            "cmd": "python test.py 3",
            "timeout": 10,
        },
        {
            "name": "Intentional fail (--fail)",
            "cmd": "python test.py 3 --fail",
            "timeout": 10,
        },
        {
            "name": "Runtime error",
            "cmd": "python test.py 3 --runtime-error",
            "timeout": 10,
        },
        {
            "name": "Simulated syntax error",
            "cmd": "python test.py 3 --syntax-error",
            "timeout": 10,
        },
        {
            "name": "Timeout (timer longer than timeout)",
            "cmd": "python test.py 20",
            "timeout": 2,
        },
        {
            "name": "Invalid command (command not found)",
            "cmd": "pythonnnn test.py 3",
            "timeout": 5,
        },
    ]

    for t in tests:
        print(f"##### TEST: {t['name']} #####")
        runApplicationAndSync(t["cmd"], timeout=t["timeout"])
"""