import subprocess
import os

os.chdir("/Data/ODGL/build/bin")
output = subprocess.check_output(["./run_test.sh", "RedCubeTest"])
res = exists("1507505686231.png")
type(Key.ESC)
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
