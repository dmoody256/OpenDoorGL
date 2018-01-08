import os
import signal
import subprocess
import platform
import threading

sys.path.append(os.path.abspath(getBundlePath() + "/.." ))
import OdglTestFramework

dname = os.path.abspath(getBundlePath() + "/../../build/bin/")
p = OdglTestFramework.StartTest("RedCubeTest", dname)

res = exists("1508297323827.png", 30)
type(Key.ESC)
waitVanish("1508297323827.png")

if(res == None):
    print("FAIL: No match found") 
    out, err = p.communicate()
    print('Test app exited with error code: ' + str(err) + ', and output: ' + out)
    exit(1)
else:
    print('INFO: Found match with score: ' + str(res.getScore()))

if(res.getScore() < .999):
    print("FAIL: Match score " + str(res.getScore()) + " too low")
    exit(2)

OdglTestFramework.EndTest(p)
