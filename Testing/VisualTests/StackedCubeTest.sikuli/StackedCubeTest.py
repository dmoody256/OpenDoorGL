import os
import signal
import subprocess
import platform
import threading
import time
sys.path.append(os.path.abspath(getBundlePath() + "/.."))
import OdglTestFramework

dname = os.environ['TEST_BIN_DIR']
p = OdglTestFramework.StartTest("StackedCubeTest", dname)

res = exists("1508387700265.png", 30)
time.sleep(2)
type(Key.ESC)
if(not waitVanish("1508387700265.png", 10)):
    p.kill()

if(res == None):
    print("FAIL: No match found")
    exit(1)

if(res.getScore() < .98):
    print("FAIL: Match score " + str(res.getScore()) + " too low")
    exit(2)

OdglTestFramework.EndTest(p)
