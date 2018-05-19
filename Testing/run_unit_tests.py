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

failed_tests = []
passed_tests = []

unittests = [
    'test_translate'
]

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
    print(str(output.decode('utf-8')))
    if proc.returncode == 0:
        passed_tests.append(test)
    else:
        failed_tests.append(test)

print("passed " + str(len(passed_tests)) + " tests.")


if len(failed_tests) > 0:
    print("failed " + str(len(failed_tests)) + " tests:")
    for test in failed_tests:
        print(test)
    exit(1)
else:
    print("failed 0 test.")
