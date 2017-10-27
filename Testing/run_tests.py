import os
import subprocess
import platform

SikuliPath = os.environ["SIKULI_DIR"]

sikuli_command = [SikuliPath + "/runsikulix", "-r"]

tests = [
    "cubeTestRed.sikuli",
    "StackedCubeTest.sikuli",
    "LoadObjTest.sikuli",
]

for test in tests:
    test_command = []
    test_command.extend(sikuli_command)
    test_command.append(test)
    try:
        output = subprocess.check_output(test_command)
        print("test result: " + str(output))
    except subprocess.CalledProcessError as grepexc:                                                                                                   
        print("failed test " + test + "with exit code: " + str(grepexc.returncode) + "output:\n\n" + grepexc.output)
        pass

