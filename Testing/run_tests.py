import os
import subprocess
import platform

failed_tests = []
passed_tests = []

unittests = [
    'test_translate'
]

for test in unittests:
    testenv = os.environ
    testenv['LD_LIBRARY_PATH'] = '../lib'
    proc = subprocess.Popen("./" + test, cwd=os.path.abspath("../build/bin"),stderr=subprocess.STDOUT, stdout=subprocess.PIPE, env=testenv)
    output = proc.communicate()[0]
    print(str(output))
    if(proc.returncode == 0):
        passed_tests.append(test)
    else:    
        failed_tests.append(test)

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
    proc = subprocess.Popen(test_command, cwd=os.path.abspath("VisualTests"),stderr=subprocess.STDOUT, stdout=subprocess.PIPE)
    output = proc.communicate()[0]

    if(proc.returncode == 0):
        print("Passed Test " + test + ": " + os.linesep + str(output))
        passed_tests.append(test)
    else:                                                                                                  
        print("Failed Test " + test + " with exit code: " + str(proc.returncode) + "output:\n\n" + output)
        failed_tests.append(test)

print( "passed " + str(len(passed_tests)) + " tests:")

print( "failed " + str(len(failed_tests)) + " tests:")
for test in failed_tests:
    print(test)

if(len(failed_tests) > 0):
    exit(1)

