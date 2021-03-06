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
import platform
import subprocess

from BuildUtils.ColorPrinter import ColorPrinter

failed_tests = []
passed_tests = []
unittests = []

for test_header in os.listdir("UnitTests/TestHeaders"):
    unittests.append(os.path.splitext(test_header)[0])

for test in unittests:
    testenv = os.environ
    if "windows" in platform.system().lower():
        test_file = os.path.abspath("../build/bin") + "/" + test + '.exe'
    else:
        testenv['LD_LIBRARY_PATH'] = '../lib'
        test_file = "./" + test
    proc = subprocess.Popen(
        test_file,
        cwd=os.path.abspath("../build/bin"),
        stderr=subprocess.STDOUT,
        stdout=subprocess.PIPE,
        env=testenv
    )
    output = proc.communicate()[0]
    output_lines = output.decode('utf-8').strip().split(os.linesep)
    printer = None
    try:
        from BuildUtils.ColorPrinter import ColorPrinter
        printer = ColorPrinter()
        if str(output_lines[-1]).strip().endswith(".OK!"):
            printer.TestPassPrint(" " + output_lines[0])
        else:
            printer.TestFailPrint(" " + output.decode('utf-8').strip())
    except:
        print(output.decode('utf-8').strip())
        pass

    if proc.returncode == 0:
        passed_tests.append(test)
    else:
        failed_tests.append(test)

results_string = "Passed " + str(len(passed_tests)) + " tests." + os.linesep
if len(failed_tests) > 0:
    results_string += "Failed " + \
        str(len(failed_tests)) + " tests:" + os.linesep
    for test in failed_tests:
        results_string += test + os.linesep
else:
    results_string += "Failed 0 test." + os.linesep

if printer:
    printer.TestResultPrint(results_string.strip())
else:
    print(results_string.strip())

if len(failed_tests) > 0:
    exit(1)
