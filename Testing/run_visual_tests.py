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

failed_tests = []
passed_tests = []

# Visual Tests
SikuliPath = os.environ["SIKULI_DIR"]


# tests = [
#    "RedCubeTest.sikuli",
#    "StackedCubeTest.sikuli",
#    "LoadObjTest.sikuli",
# ]

tests = [
    "StackedCubeTest.sikuli",
]
passed_tests = []


def run_tests(tests):

    failed_tests = []

    for test in tests:

        print(os.path.abspath("VisualTests"))

        proc = subprocess.Popen(
            [sys.executable, "run_test.py"],
            cwd=os.path.abspath("VisualTests/" + test),
            stderr=subprocess.STDOUT,
            stdout=subprocess.PIPE
        )
        output = proc.communicate()[0].decode("utf-8")

        if proc.returncode == 0:
            print("Passed Test " + test + ": " + os.linesep + str(output))
            passed_tests.append(test)
        else:
            print("Failed Test " + test + " with exit code: "
                  + str(proc.returncode) + "output:")
            print(output)
            failed_tests.append(test)


run_tests(tests)

retry_count = 0
while failed_tests and retry_count < 3:
    run_tests(failed_tests)
    retry_count += 1

print("passed " + str(len(passed_tests)) + " tests:")
print("failed " + str(len(failed_tests)) + " tests:")

for test in failed_tests:
    print(test)

if failed_tests:
    exit(1)
