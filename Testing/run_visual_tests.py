# This file is licensed under the MIT License.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

"""
Script for running all the tests against the OpenDoorGL library.
"""

import os
import sys
import platform
import subprocess
import time

try:
    from BuildUtils.ColorPrinter import ColorPrinter
    printer = ColorPrinter()
except:
    printer = None
    pass


def StartGraphicsApp(test, working_dir):

    my_env = os.environ.copy()
    command = []

    if "windows" in platform.system().lower():
        command.append("python")
        command.append("windows_workaround.py")
        command.append(working_dir + "/" + test + ".exe")
    else:
        #if(os.path.isfile("/opt/VirtualGL/bin/vglrun")):
        #    command.append("vglrun")
        command.append("./" + test)
        my_env['DISPLAY'] = ':0'
        my_env["LD_LIBRARY_PATH"] = '../lib'

    if "windows" in platform.system().lower():
        proc = subprocess.Popen(command, stderr=subprocess.STDOUT,
                                stdout=subprocess.PIPE)
    else:
        proc = subprocess.Popen(command, cwd=working_dir, stderr=subprocess.STDOUT,
                                stdout=subprocess.PIPE, env=my_env, shell=False)
    return proc


def RunTest(test):

    # Visual Tests
    SikuliPath = os.environ["SIKULI_DIR"]
    dname = os.environ['TEST_BIN_DIR']
    testenv = os.environ
    sikuli_test = test + ".sikuli"

    relaunch_count = 20
    for i in range(relaunch_count):
        proc1 = StartGraphicsApp(test, testenv['TEST_BIN_DIR'])
        time.sleep(2)
        if proc1.poll() == None:
            break
        if i == relaunch_count-1:
            return False

    if "windows" in platform.system().lower():
        sikuli_command = ['java', '-jar', SikuliPath + "/sikulix.jar", "-r", sikuli_test]
    else:
        testenv['LD_LIBRARY_PATH'] = '../lib'
        testenv['DISPLAY'] = ':0'
        sikuli_command = ['java', '-jar', SikuliPath + "/sikulix.jar", "-r", "../" + sikuli_test]

    proc = subprocess.Popen(
        sikuli_command,
        cwd=os.path.abspath("VisualTests/SikuliX"),
        stderr=subprocess.STDOUT,
        stdout=subprocess.PIPE,
        env=testenv
    )

    output, err = proc.communicate()
    proc1.terminate()

    if printer:
        if("INFO: Passed!" in output):
            printer.TestPassPrint(" " + test + " Passed!")
            return True
        printer.TestFailPrint(" " + test + " Failed!")
    else:
        if("INFO: Passed!" in output):
            print(" " + test + " Passed!")
            return True
        print(" " + test + " Failed!")

    print(output)

    return False


failed_tests = []
passed_tests = []
tests = []

for test_dir in os.listdir("VisualTests"):
    if(test_dir.endswith('.sikuli')):
        tests.append(os.path.splitext(test_dir)[0])

for test in tests:
    if(RunTest(test)):
        passed_tests.append(test)
    else:
        failed_tests.append(test)

if printer:
    printer.InfoPrint(" Passed " + str(len(passed_tests)) + " tests!")
    printer.InfoPrint(" Failed " + str(len(failed_tests)) + " tests!")
else:
    print(" Passed " + str(len(passed_tests)) + " tests!")
    print(" Failed " + str(len(failed_tests)) + " tests!")

if failed_tests:
    exit(1)
