import os
import signal
import subprocess
import platform
import threading
import time

sys.path.append(os.path.abspath(getBundlePath() + "/.."))
import OdglTestFramework
#dname = os.environ['TEST_BIN_DIR']
dname = "C:\\Users\\Daniel\\workspace\\OpenDoorGL\\build\\bin"
p = OdglTestFramework.StartTest("RedCubeTest", dname)

res = exists("1508297323827.png", 10)
time.sleep(2)
type(Key.ESC)
if(not waitVanish("1508297323827.png", 5)):
    p.kill()

OdglTestFramework.EndTest(p)

if(res == None):
    print("FAIL: No match found")
    out, err = p.communicate()
    print('Test app exited with error code: ' +
          str(err) + ', and output: ' + out)
    exit(1)
else:
    print('INFO: Found match with score: ' + str(res.getScore()))

if(res.getScore() < .98):
    print("FAIL: Match score " + str(res.getScore()) + " too low")
    exit(2)
