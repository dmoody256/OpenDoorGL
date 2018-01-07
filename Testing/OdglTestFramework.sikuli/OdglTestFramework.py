from sikuli import *

import os
import signal
import subprocess
import platform
import threading
import re

def StartTest(test, working_dir):
    dname = os.path.abspath(getBundlePath() + "/../../build/bin/")

    command = []
    if(os.path.isfile("/opt/VirtualGL/bin/vglrun")):
        command.append("vglrun")
    command.extend(["./run_test.sh", test])

    my_env = os.environ.copy()
    my_env["LD_LIBRARY_PATH"] = '../lib'

    os.chdir(working_dir)
    print("Running: " + str(command))
    return subprocess.Popen('./' + test, stderr=subprocess.STDOUT, stdout=subprocess.PIPE, env=my_env)

def EndTest(process, framerate = 0.016):
    out, err = process.communicate()
    framerateResults = re.findall('([\d.]+$)', out)
    if(float(framerateResults[0]) < framerate ):
        print("FAIL: Frame rate " + str(float(framerateResults[0])) + " too low") 
        exit(3)
    else:
        print("Test Passed: Framerate = " + str(float(framerateResults[0])))
        exit(0)
