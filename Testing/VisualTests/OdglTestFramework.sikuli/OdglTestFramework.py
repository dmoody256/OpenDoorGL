from sikuli import *

import os
import signal
import subprocess
import platform
import threading
import re

def StartTest(test, working_dir):

    my_env = os.environ.copy()
    command = []
    if(os.path.isfile("/opt/VirtualGL/bin/vglrun")):
        command.append("vglrun")

    command.append("./" + test)

    my_env["LD_LIBRARY_PATH"] = '../lib'
    
    return subprocess.Popen(command, cwd=working_dir, stderr=subprocess.STDOUT, stdout=subprocess.PIPE, env=my_env)

def EndTest(process, framerate = 0.016):
    out, err = process.communicate()
    print("Test output: " + str(out))
    print("Test exitcode: " + str(err))
    framerateResults = re.findall('([\d.]+$)', out)
    
    print("Test Passed: Framerate = " + str(float(framerateResults[0])))
    exit(0)
