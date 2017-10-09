import subprocess
import os

os.chdir("/Data/ODGL/build/bin")
output = subprocess.check_output(["./run_example.sh", "BasicCube"])
res = exists("1507505686231.png")
type(Key.ESC)
print res.getScore()
if(res.getScore() < .99):
    print("Failure")
else:
    print("Success")
