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
Main SConstruct file for building the OpenDoorGL library along with tests.
"""

# python
import os
import subprocess

# scons
import SCons.Action
from SCons.Environment import Environment
from SCons.Script import SConscript
from SCons.Script.Main import AddOption
from SCons.Script.Main import GetOption

# project
import build_utils

AddOption(
    '--debug_build',
    dest='debug_build',
    action='store_true',
    metavar='BOOL',
    default=False,
    help='Build in debug mode'
)

AddOption(
    '--test',
    dest='run_test',
    action='store_true',
    metavar='BOOL',
    default=False,
    help='run tests after build'
)

MAIN_ENV = Environment(
    DEBUG_BUILD=GetOption('debug_build'),
    TARGET_ARCH='x86_64',
    RUN_TEST=GetOption('run_test')
)

MAIN_ENV.baseProjectDir = os.path.abspath(MAIN_ENV.Dir('.').abspath).replace('\\', '/')

###################################################
# Determine number of CPUs
NUM_CPUS = build_utils.get_num_cpus()
print("Building with " + str(NUM_CPUS) + " parallel jobs")
MAIN_ENV.SetOption("num_jobs", NUM_CPUS)

###
# build subProjects
DEPEND_LIBS = SConscript('Dependencies/SConscript', duplicate=0, exports='MAIN_ENV')
CORE_LIBS = SConscript('Core/SConscript', duplicate=0, exports='MAIN_ENV')
FRAMEWORK_LIBS = SConscript('AppFrameworks/SConscript', duplicate=0, exports='MAIN_ENV')
TESTS = SConscript('Testing/SConscript', duplicate=0, exports='MAIN_ENV')


# setup installs
for header in DEPEND_LIBS['headers']['GLEW']:
    MAIN_ENV.Install("build/include/GLEW", header)
for header in DEPEND_LIBS['headers']['GLFW']:
    MAIN_ENV.Install("build/include/GLFW", header)

TEST_NODE = MAIN_ENV.Install("build/include", 'Dependencies/glm/glm')

for lib in DEPEND_LIBS['libs']:
    MAIN_ENV.Depends(CORE_LIBS, lib)
    MAIN_ENV.Install("build/lib", lib)

for buildFile in CORE_LIBS:
    if str(buildFile).endswith(".hpp"):
        MAIN_ENV.Install("build/include", buildFile)
    else:
        MAIN_ENV.Install("build/lib", buildFile)

MAIN_ENV.Depends(FRAMEWORK_LIBS, CORE_LIBS)

for buildFile in FRAMEWORK_LIBS:
    if str(buildFile).endswith(".hpp"):
        MAIN_ENV.Install("build/include", buildFile)
    else:
        MAIN_ENV.Install("build/lib", buildFile)

def chmod_build_dir():
    """
    Callback function used to change the permission of the build files
    so they can be executed.
    """
    def make_executable(path):
        """
        Utility function to perform the chmod command.
        """
        mode = os.stat(path).st_mode
        mode |= (mode & 0o444) >> 2    # copy R bits to X
        os.chmod(path, mode)

    for dir_name in ['build']:
        for root, _unused_dirs, files in os.walk(dir_name):
            for file_to_chmod in files:
                make_executable(os.path.join(root, file_to_chmod))

CHMOD_CALLBACK = SCons.Action.ActionFactory(
    chmod_build_dir,
    lambda: 'Setting build to exec permissions'
)

CHMOD_COMMAND = MAIN_ENV.Command('never_exists', 'build', CHMOD_CALLBACK())
MAIN_ENV.Depends(CHMOD_COMMAND, FRAMEWORK_LIBS)
MAIN_ENV.AlwaysBuild(CHMOD_COMMAND)

def run_tests():
    """
    Callback function to run the test script.
    """
    if('SIKULI_DIR' not in os.environ
       and not os.path.isdir(MAIN_ENV.baseProjectDir+'/Testing/VisualTests/SikuliX')):
        proc = subprocess.Popen(
            args=['./install_sikuliX.sh'],
            cwd=MAIN_ENV.baseProjectDir+'/Testing/VisualTests',
            stdout=subprocess.PIPE,
            shell=True)
        output = proc.communicate()[0]
        print(output)

    test_env = os.environ
    if 'SIKULI_DIR' not in os.environ:
        test_env['SIKULI_DIR'] = MAIN_ENV.baseProjectDir+'/Testing/VisualTests/SikuliX'

    test_env['TEST_BIN_DIR'] = MAIN_ENV.baseProjectDir+'/build/bin'

    if 'DISPLAY' not in test_env:
        test_env['DISPLAY'] = ':0'

    proc = subprocess.Popen(
        args=['python', 'run_tests.py'],
        cwd=MAIN_ENV.baseProjectDir+'/Testing',
        env=test_env
    )
    output = proc.communicate()[0]
    print(output)

TEST_CALLBACK = SCons.Action.ActionFactory(
    run_tests,
    lambda: 'Running Tests... Please be Patient'
)

TEST_BINS = []
for test in TESTS:
    MAIN_ENV.Depends(test['executable'], FRAMEWORK_LIBS)
    TEST_BINS.append(MAIN_ENV.Install("build/bin", test['executable']))
    for resource in test['resources']:
        MAIN_ENV.Install("build/bin/resources", resource)
    MAIN_ENV.Depends(CHMOD_COMMAND, test['executable'])

if GetOption('run_test'):
    TEST_COMMAND = MAIN_ENV.Command(
        MAIN_ENV.Dir('Testing/VisualTests/SikuliX'),
        TEST_BINS,
        TEST_CALLBACK())
    MAIN_ENV.Depends(TEST_COMMAND, CHMOD_COMMAND)
    MAIN_ENV.AlwaysBuild(TEST_COMMAND)
