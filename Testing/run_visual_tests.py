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

FAILED_TESTS = []
PASSED_TESTS = []

# Visual Tests
SIKULI_PATH = os.environ["SIKULI_DIR"]

SIKULI_COMMAND = [SIKULI_PATH + "/runsikulix", "-r"]

TESTS = [
    "RedCubeTest.sikuli",
    "StackedCubeTest.sikuli",
    "LoadObjTest.sikuli",
]

for test in TESTS:
    test_command = []
    test_command.extend(SIKULI_COMMAND)

    test_file = "./" + test
    if "windows" in platform.system().lower():
        test_file += '.exe'
    test_command.append(test_file)
    proc = subprocess.Popen(
        test_command,
        cwd=os.path.abspath("VisualTests"),
        stderr=subprocess.STDOUT,
        stdout=subprocess.PIPE
    )
    output = proc.communicate()[0]

    if proc.returncode == 0:
        print("Passed Test " + test + ": " + os.linesep + str(output))
        PASSED_TESTS.append(test)
    else:
        print("Failed Test " + test + " with exit code: "
              + str(proc.returncode) + "output:")
        print(output)
        FAILED_TESTS.append(test)

print("passed " + str(len(PASSED_TESTS)) + " tests:")
print("failed " + str(len(FAILED_TESTS)) + " tests:")

for test in FAILED_TESTS:
    print(test)

if FAILED_TESTS:
    exit(1)
