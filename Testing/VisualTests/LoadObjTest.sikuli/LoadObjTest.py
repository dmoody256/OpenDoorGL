import os
import signal
import subprocess
import platform
import threading

sys.path.append(os.path.abspath(getBundlePath() + "/.." ))
import OdglTestFramework

dname = os.environ['TEST_BIN_DIR']
p = OdglTestFramework.StartTest("LoadObjTest", dname)

res = exists("1509078145523.png", 30 )
type(Key.ESC)
waitVanish("1509078145523.png")

if(res == None):
    print("FAIL: No match found") 
    exit(1)

if(res.getScore() < .98):
    print("FAIL: Match score " + str(res.getScore()) + " too low")
    exit(2)

OdglTestFramework.EndTest(p)
