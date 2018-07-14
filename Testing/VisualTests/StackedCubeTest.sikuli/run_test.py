import os
import platform
import subprocess
import signal
import threading
import time


def StartTest(test, working_dir):

    my_env = os.environ.copy()
    command = []

    if "windows" in platform.system().lower():
        command.append(working_dir + "/" + test + ".exe")

    else:
        if(os.path.isfile("/opt/VirtualGL/bin/vglrun")):
            command.append("vglrun")
        command.append("./" + test)
        my_env["LD_LIBRARY_PATH"] = '../lib'
    if "windows" in platform.system().lower():
        proc = subprocess.Popen(command, cwd=working_dir, stderr=subprocess.STDOUT,
                                stdout=subprocess.PIPE, env=my_env, shell=False, creationflags=subprocess.CREATE_NEW_PROCESS_GROUP)
    else:
        proc = subprocess.Popen(command, cwd=working_dir, stderr=subprocess.STDOUT,
                                stdout=subprocess.PIPE, env=my_env, shell=False)
    return proc


# Visual Tests
SikuliPath = os.environ["SIKULI_DIR"]
dname = os.environ['TEST_BIN_DIR']

testenv = os.environ
test = "StackedCubeTest.sikuli"

if "windows" in platform.system().lower():
    test_file = test
    sikuli_command = [SikuliPath + "/runsikulix.cmd", "-r"]
else:
    test_file = "./" + test
    testenv['LD_LIBRARY_PATH'] = '../lib'
    testenv['DISPLAY'] = ':0'
    sikuli_command = [SikuliPath + "/runsikulix", "-r"]

proc1 = StartTest("StackedCubeTest", testenv['TEST_BIN_DIR'])

retry_count = 0


test_command = []
test_command.extend(sikuli_command)
test_command.append(test_file)

proc = subprocess.Popen(
    test_command,
    cwd=os.path.abspath('..'),
    stderr=subprocess.STDOUT,
    stdout=subprocess.PIPE,
    env=testenv
)
output, err = proc.communicate()
proc1.terminate()
print(output.decode("utf-8"))
if("FAIL:" in output.decode("utf-8")):
    exit(1)
