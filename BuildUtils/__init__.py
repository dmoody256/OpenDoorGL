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
Utility functios used for building.
"""

# python
import os
import glob
import re
import time
import datetime
import atexit
import platform
import subprocess

# scons
from SCons.Script.SConscript import call_stack
from SCons.Script.Main import Progress

testFile = "test"
def get_num_cpus():
    """
    Function to get the number of CPUs the system has.
    """
    # Linux, Unix and MacOS:
    if hasattr(os, "sysconf"):
        if 'SC_NPROCESSORS_ONLN' in os.sysconf_names:
            # Linux & Unix:
            ncpus = os.sysconf("SC_NPROCESSORS_ONLN")
        if isinstance(ncpus, int) and ncpus > 0:
            return ncpus
        # OSX:
        return int(os.popen("sysctl -n hw.ncpu")[1].read())
    # Windows:
    if 'NUMBER_OF_PROCESSORS' in os.environ:
        ncpus = int(os.environ["NUMBER_OF_PROCESSORS"])
    if ncpus > 0:
        return ncpus
    return 1 # Default

def ImportVar(import_name):
    """
    Function to workaround pylints dislike for globals.
    """
    frame = call_stack[-1]
    return frame.exports[import_name]

def bf_to_str(bf):
    """Convert an element of GetBuildFailures() to a string
    in a useful way."""
    import SCons.Errors
    if bf is None: # unknown targets product None in list
        return '(unknown tgt)'
    elif isinstance(bf, SCons.Errors.StopError):
        return str(bf)
    elif bf.node:
        return str(bf.node) + ': ' + bf.errstr
    elif bf.filename:
        return bf.filename + ': ' + bf.errstr
    return 'unknown failure: ' + bf.errstr


def build_status():
    """Convert the build status to a 2-tuple, (status, msg)."""
    from SCons.Script import GetBuildFailures
    bf = GetBuildFailures()
    if bf:
        # bf is normally a list of build failures; if an element is None,
        # it's because of a target that scons doesn't know anything about.
        status = 'failed'
        failures_message = "\n".join(["%s" % bf_to_str(x)
                            for x in bf if x is not None])
    else:
        # if bf is None, the build completed successfully.
        status = 'ok'
        failures_message = ''
    return (status, failures_message)

def highlight_word(line, word, color):
    ENDC = '\033[0m'
    return line.replace(word, color + word + ENDC )


def display_build_status():
    """Display the build status.  Called by atexit.
    Here you could do all kinds of complicated things."""
    status, _unused_failures_message = build_status()

    #HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

    compileOutput = []
    for filename in glob.glob('build/build_logs/*_compile.txt'):
        f = open(filename, "r")
        tempList = f.read().splitlines()
        if(len(tempList) > 0):
            compileOutput += [OKBLUE + os.path.basename(filename).replace("_compile.txt", "") + ":" + ENDC]
            compileOutput += tempList
    for line in compileOutput:
        line = highlight_word(line, "error", FAIL)
        line = highlight_word(line, "warning", WARNING)
        print(line)
    linkOutput = []
    for filename in glob.glob('build/build_logs/*_link.txt'):
        f = open(filename, "r")
        tempList = f.read().splitlines()
        if(len(tempList) > 0):
            linkOutput += [OKBLUE + os.path.basename(filename).replace("_link.txt", "") + ":" + ENDC]
            linkOutput += tempList
    for line in linkOutput:
        line = highlight_word(line, "error", FAIL)
        line = highlight_word(line, "warning", WARNING)
        print(line)
        
    if status == 'failed':
        print(FAIL + "Build failed." + ENDC)
    elif status == 'ok':
        print (OKGREEN + "Build succeeded." + ENDC)

class ColorPrinter():

    def __init__(self):

        if("Windows" in platform.system()):
            self.HEADER = ''
            self.OKBLUE = ''
            self.OKGREEN = ''
            self.WARNING = ''
            self.FAIL = ''
            self.ENDC = ''
        else:
            self.HEADER = '\033[95m'
            self.OKBLUE = '\033[94m'
            self.OKGREEN = '\033[92m'
            self.WARNING = '\033[93m'
            self.FAIL = '\033[91m'
            self.ENDC = '\033[0m'

    def InfoPrint(self, message):
        print(self.HEADER + "[   INFO] " + self.ENDC + message)

    def ErrorPrint(self, message):
        print(self.FAIL + "[  ERROR] " + self.ENDC + message)

    def CompilePrint(self, percent, message):
        percent_string = "{0:.2f}".format(percent)
        if(percent < 100): percent_string = " " + percent_string
        if(percent < 10):  percent_string = " " + percent_string
        print(self.OKGREEN + "[" + percent_string + "%] " + self.ENDC + message )

    def LinkPrint(self, message):
        print(self.OKGREEN + "[   LINK] " + self.ENDC + message)

    def ConfigString(self, message):
        return self.OKBLUE + "[ CONFIG] " + self.ENDC + message

class ProgressCounter(object):

    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

    def __init__(self, sourceFiles, targetBinaries):
        self.count = 0.0
        self.maxCount = float(len(sourceFiles))
        self.progressSources = sourceFiles
        self.targetBinaries = targetBinaries

    def disable(self):
        self.HEADER = ''
        self.OKBLUE = ''
        self.OKGREEN = ''
        self.WARNING = ''
        self.FAIL = ''
        self.ENDC = ''

    def __call__(self, node, *_unused_args, **_unused_kw):
        
        slashedNode = str(node).replace("\\", "/")
        for binFileNode in self.targetBinaries:
            #print(slashedNode)

            if(slashedNode.endswith(binFileNode)):
                filename = os.path.basename(str(node))
                if(node.get_state() == 2):
                    print(self.OKGREEN + "[   LINK] " + self.ENDC + "Linking " +  filename )
                else:
                    print(self.OKGREEN + "[   LINK] " + self.ENDC + "Skipping, already built " +  filename )
        
        if(str(node).endswith(".obj") or str(node).endswith(".os") or str(node).endswith(".o")):

            slashedNodeObj = os.path.splitext(slashedNode)[0] + ".cpp"
            for sourceFileNode in self.progressSources:
                if(slashedNodeObj.endswith( sourceFileNode)):

                    if(self.count == 0):
                        start_build_string = "Building "
                        for binFile in self.targetBinaries:
                            start_build_string += os.path.basename(binFile) + ", "
                        start_build_string = start_build_string[:-2]
                        ColorPrinter().InfoPrint(start_build_string)

                    self.count += 1
                    percent = self.count / self.maxCount * 100.00
                    filename = os.path.basename(str(node))
                    percentString = "{0:.2f}".format(percent)
                    if(percent < 100):
                        percentString = " " + percentString
                    if(percent < 10):
                        percentString = " " + percentString
                        
                    if(node.get_state() == 2):
                        print(self.OKGREEN + "[" + percentString + "%] " + self.ENDC + "Compiling " + filename )
                    else:
                        print(self.OKGREEN + "[" + percentString + "%] " + self.ENDC + "Skipping, already built " + filename )
                    break

def SetupBuildOutput(env, sourceFiles):

    windowsRedirect = ""
    linuxRedirect = "2>&1"
    if("Windows" in platform.system()):
        windowsRedirect = "2>&1"
        linuxRedirect = ""

    soureFileObjs = []
    for file in sourceFiles:
        buildObj = env.SharedObject(file, 
                                CCCOM=env['CCCOM'] + " " + windowsRedirect + " > \"" + env.baseProjectDir + "/build/build_logs/" + os.path.splitext(os.path.basename(file))[0] + "_compile.txt\" " + linuxRedirect,
                                CXXCOM=env['CXXCOM'] + " " + windowsRedirect + " > \"" + env.baseProjectDir + "/build/build_logs/" + os.path.splitext(os.path.basename(file))[0] + "_compile.txt\" " + linuxRedirect)
        soureFileObjs.append(buildObj)

    if("Windows" in platform.system()):
        env['LINKCOM'].list[0].cmd_list = env['LINKCOM'].list[0].cmd_list.replace('",'," 2>&1 > \\\"" + env.baseProjectDir + "/build/build_logs/MyLifeApp_link.txt\\\"\",") 
    else:
        env['LINKCOM'] = env['LINKCOM'].replace('",'," > \\\"" + env.baseProjectDir + "/build/build_logs/MyLifeApp_link.txt\\\"\" 2>&1 ,") 

    prog = env.SharedLibrary("build/libOpenDoorGL", soureFileObjs)

    ###################################################
    # setup build output
    if not os.path.exists(env.baseProjectDir + "/build/build_logs"):
        os.makedirs(env.baseProjectDir + "/build/build_logs")

    #if ARGUMENTS.get('fail', 0):
    #    env.Command('target', 'source', ['/bin/false'])

    atexit.register(display_build_status)

    def print_cmd_line(s, _unused_targets, _unused_sources, env):
        with open(env.baseProjectDir + "/build/build_logs/build_" + env['BUILD_LOG_TIME'] + ".log", "a") as f:
            f.write(s + "\n")

    env['BUILD_LOG_TIME'] = datetime.datetime.fromtimestamp(time.time()).strftime('%Y_%m_%d__%H_%M_%S')
    if("VERBOSE" not in env):
        env['PRINT_CMD_LINE_FUNC'] = print_cmd_line

    builtBins = []
    if("Windows" in platform.system()):
        builtBins.append("build/MyLifeApp.exe")
    else:
        builtBins.append("build/libOpenDoorGL.so")
        
    Progress(ProgressCounter(sourceFiles, builtBins), interval=1)

    return [env, prog]

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