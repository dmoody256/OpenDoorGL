from sikuli import *

import os
import signal
import subprocess
import platform
import threading
import re
import sys
import java.lang.System
import time


def StartTest(test, working_dir):

    my_env = os.environ.copy()
    command = []

    if "windows" in java.lang.System.getProperty('os.name').lower():
        command.append(working_dir + "/" + test + ".exe")

    else:
        if(os.path.isfile("/opt/VirtualGL/bin/vglrun")):
            command.append("vglrun")
        command.append("./" + test)
        my_env["LD_LIBRARY_PATH"] = '../lib'
    proc = subprocess.Popen(command, cwd=working_dir, stderr=subprocess.STDOUT,
                            stdout=subprocess.PIPE, env=my_env, shell=False)
    time.sleep(2)
    return proc


def EndTest(process, framerate=0.016):
    out, err = process.communicate()
    print("Test output: " + str(out))
    print("Test exitcode: " + str(err))
    framerateResults = re.findall('([\d.]+$)', out)

    if(err):
        print("Test Failed: " + out.decode('utf-8'))
    else:
        if(framerateResults):
            print("Test Passed: Framerate = " +
                  str(float(framerateResults[0])))
        else:
            print("Test Passed: Framerate = " + out.decode('utf-8'))
