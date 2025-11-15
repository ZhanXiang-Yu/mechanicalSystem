from copy import error
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