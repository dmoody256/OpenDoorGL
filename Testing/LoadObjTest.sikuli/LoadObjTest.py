import os
import signal
import subprocess
import platform
import threading

dname = os.path.abspath(getBundlePath() + "/../../build/bin/")
os.chdir(dname)
print dname
redCubeCommand = []
if(os.path.isfile("/opt/VirtualGL/bin/vglrun")):
    redCubeCommand.append("vglrun")
redCubeCommand.extend(["./run_test.sh", "LoadObjTest"])
print "running command " +str(redCubeCommand)
subprocess.Popen(redCubeCommand, cwd=dname, stderr=subprocess.STDOUT, stdout=subprocess.PIPE)
res = exists("1509078145523.png", 5 )
type(Key.ESC)
waitVanish("1509078145523.png")
        
if(res == None):
    print("Failure") 
    exit(1)
print res.getScore()
if(res.getScore() < .999):
    print("Failure")
    exit(2)
else:
    print("Success")
    exit(0)
