import os
import subprocess
import platform

SikuliPath = os.environ["SIKULI_DIR"]

sikuli_command = [SikuliPath + "/runsikulix", "-r"]

tests = [
    "RedCubeTest.sikuli",
    "StackedCubeTest.sikuli",
    "LoadObjTest.sikuli",
]

failed_tests = []
passed_tests = []
for test in tests:
    test_command = []
    test_command.extend(sikuli_command)
    test_command.append(test)
    try:
        output = subprocess.check_output(test_command)
        print("test result: " + str(output))
        passed_tests.append(test)
    except subprocess.CalledProcessError as grepexc:                                                                                                   
        print("failed test " + test + "with exit code: " + str(grepexc.returncode) + "output:\n\n" + grepexc.output)
        failed_tests.append(test)
        pass

print( "passed " + str(len(passed_tests)) + " tests:")

print( "failed " + str(len(failed_tests)) + " tests:")
for test in failed_tests:
    print(test)

if(len(failed_tests) > 0):
    exit(1)
