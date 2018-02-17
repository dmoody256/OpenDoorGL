import os
import subprocess
import platform

failed_tests = []
passed_tests = []

# Visual Tests
SikuliPath = os.environ["SIKULI_DIR"]

sikuli_command = [SikuliPath + "/runsikulix", "-r"]

tests = [
    "RedCubeTest.sikuli",
    "StackedCubeTest.sikuli",
    "LoadObjTest.sikuli",
]

for test in tests:
    test_command = []
    test_command.extend(sikuli_command)
    test_command.append(test)
    try:
        output = subprocess.check_output(test_command)
        print("Passed Test " + test + ": " + os.linesep + str(output))
        passed_tests.append(test)
    except subprocess.CalledProcessError as grepexc:                                                                                                   
        print("Failed Test " + test + " with exit code: " + str(grepexc.returncode) + "output:\n\n" + grepexc.output)
        failed_tests.append(test)
        pass



print( "passed " + str(len(passed_tests)) + " tests:")

print( "failed " + str(len(failed_tests)) + " tests:")
for test in failed_tests:
    print(test)

if(len(failed_tests) > 0):
    exit(1)

