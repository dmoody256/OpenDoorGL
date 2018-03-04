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
import subprocess

failed_tests = []
passed_tests = []

unittests = [
    'test_translate'
]

for test in unittests:
    testenv = os.environ
    testenv['LD_LIBRARY_PATH'] = '../lib'
    proc = subprocess.Popen(
        "./" + test,
        cwd=os.path.abspath("../build/bin"),
        stderr=subprocess.STDOUT,
        stdout=subprocess.PIPE,
        env=testenv
    )
    output = proc.communicate()[0]
    print(str(output))
    if proc.returncode == 0:
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
    proc = subprocess.Popen(
        test_command,
        cwd=os.path.abspath("VisualTests"),
        stderr=subprocess.STDOUT,
        stdout=subprocess.PIPE
    )
    output = proc.communicate()[0]

    if proc.returncode == 0:
        print("Passed Test " + test + ": " + os.linesep + str(output))
        passed_tests.append(test)
    else:
        print("Failed Test " + test + " with exit code: "
              + str(proc.returncode) + "output:")
        print(output)
        failed_tests.append(test)

print("passed " + str(len(passed_tests)) + " tests:")
print("failed " + str(len(failed_tests)) + " tests:")

for test in failed_tests:
    print(test)

if failed_tests:
    exit(1)
