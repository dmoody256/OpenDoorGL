import os
import subprocess
import platform

SikuliPath = os.environ["SIKULI_DIR"]
try:
    output = subprocess.check_output([SikuliPath + "/runsikulix", "-r", "cubeTestRed.sikuli"])
    print("test result: " + str(output))

    output = subprocess.check_output([SikuliPath + "/runsikulix", "-r", "StackedCubeTest.sikuli"])
    print("test result: " + str(output))
except:
    print output 