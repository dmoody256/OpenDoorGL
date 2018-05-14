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

# scons
import SCons.Action
from SCons.Environment import Environment
from SCons.Script import SConscript
from SCons.Script.Main import AddOption
from SCons.Script.Main import GetOption

# project
from BuildUtils import get_num_cpus
from BuildUtils import chmod_build_dir
from BuildUtils import run_tests

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
    RUN_TEST=GetOption('run_test'),
    PROJECT_DIR=""
)
MAIN_ENV['PROJECT_DIR'] = MAIN_ENV.Dir('.').abspath.replace('\\', '/')

###################################################
# Determine number of CPUs
NUM_CPUS = get_num_cpus()
print("Building with " + str(NUM_CPUS) + " parallel jobs")
MAIN_ENV.SetOption("num_jobs", 1)

###
# build subProjects
DEPEND_LIBS = SConscript(
    'Dependencies/SConscript',
    duplicate=0,
    exports='MAIN_ENV'
)

CORE_LIBS = SConscript(
    'Core/SConscript',
    duplicate=0,
    exports='MAIN_ENV'
)

FRAMEWORK_LIBS = SConscript(
    'AppFrameworks/SConscript',
    duplicate=0,
    exports='MAIN_ENV'
)

TESTS = SConscript(
    'Testing/SConscript',
    duplicate=0,
    exports='MAIN_ENV'
)


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
    print(buildFile)
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

CHMOD_CALLBACK = SCons.Action.ActionFactory(
    chmod_build_dir,
    lambda: 'Setting build to exec permissions'
)

CHMOD_COMMAND = MAIN_ENV.Command('never_exists', 'build', CHMOD_CALLBACK())
MAIN_ENV.Depends(CHMOD_COMMAND, FRAMEWORK_LIBS)
MAIN_ENV.AlwaysBuild(CHMOD_COMMAND)

TEST_CALLBACK = SCons.Action.ActionFactory(
    run_tests,
    lambda base_dir: 'Running Tests... Please be Patient'
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
        TEST_CALLBACK(MAIN_ENV.baseProjectDir))
    MAIN_ENV.Depends(TEST_COMMAND, CHMOD_COMMAND)
    MAIN_ENV.AlwaysBuild(TEST_COMMAND)
