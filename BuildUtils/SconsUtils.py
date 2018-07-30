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
import time
import datetime
import atexit
import platform
import subprocess
import re
import sys

# scons
from SCons.Script.SConscript import call_stack
from SCons.Script.Main import Progress
from SCons import Action
from SCons.Script import Main
from SCons.Environment import Environment
from SCons.Script.Main import GetOption
from SCons.Errors import BuildError


from BuildUtils.ColorPrinter import ColorPrinter


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
    # Default
    return 1


def ImportVar(import_name):
    """
    Function to workaround pylints dislike for globals.
    """
    frame = call_stack[-1]
    return frame.exports[import_name]


def bf_to_str(bf):
    """
    Convert an element of GetBuildFailures() to a string
    in a useful way.
    """
    import SCons.Errors
    # unknown targets product None in list
    if bf is None:
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


def display_build_status(start_time):
    """Display the build status.  Called by atexit.
    Here you could do all kinds of complicated things."""
    status, _unused_failures_message = build_status()

    printer = ColorPrinter()

    for filename in glob.glob('build/build_logs/*_compile.txt'):
        compileOutput = []

        f = open(filename, "r")
        tempList = f.read().splitlines()
        if tempList:
            compileOutput += [
                printer.OKBLUE
                + os.path.basename(filename).replace("_compile.txt", "")
                + ":"
                + printer.ENDC
            ]
            compileOutput += tempList

        pending_output = os.linesep
        found_info = False
        for line in compileOutput:
            if('error' in line or 'warning' in line or "note" in line):
                line = printer.highlight_word(line, "error", printer.FAIL)
                line = printer.highlight_word(line, "warning", printer.WARNING)
                line = printer.highlight_word(line, "note", printer.OKBLUE)
                found_info = True
            pending_output += line + os.linesep
        if found_info:
            print(pending_output)

    for filename in glob.glob('build/build_logs/*_link.txt'):
        linkOutput = []
        f = open(filename, "r")
        tempList = f.read().splitlines()
        if tempList:
            linkOutput += [
                printer.OKBLUE
                + os.path.basename(filename).replace("_link.txt", "")
                + ":"
                + printer.ENDC
            ]
            linkOutput += tempList

        pending_output = os.linesep
        found_info = False
        for line in linkOutput:
            if('error' in line or 'warning' in line or "note" in line):
                line = printer.highlight_word(line, "error", printer.FAIL)
                line = printer.highlight_word(line, "warning", printer.WARNING)
                line = printer.highlight_word(line, "note", printer.OKBLUE)
                found_info = True
            pending_output += line + os.linesep
        if found_info:
            print(pending_output)

    if status == 'failed':
        print(printer.FAIL + "Build failed" + printer.ENDC +
              " in %.3f seconds" % (time.time() - start_time))
    elif status == 'ok':
        print(printer.OKGREEN + "Build succeeded" + printer.ENDC +
              " in %.3f seconds" % (time.time() - start_time))


class ProgressCounter(object):

    """
    Utility class used for printing progress during the build.
    """
    class ProgressBuild():

        def __init__(self, sources, target):

            self.count = 0.0
            self.progress_sources = dict()
            self.target = None
            for source in sources:
                # print("Making key: " + os.path.splitext(source)[0])
                self.progress_sources[os.path.splitext(source)[0]] = False
            self.target = target

    def __init__(self):
        self.printer = ColorPrinter()
        self.progress_builders = []
        self.target_name_size = 0

    def AddBuild(self, env, sources, target):
        if self.target_name_size < len(target):
            self.target_name_size = len(target)
            # self.printer.SetSize(self.target_name_size)
        pathed_sources = [env.File(source).abspath.replace('\\', '/').replace(env['PROJECT_DIR'] + '/', '')
                          for source in sources]
        self.progress_builders.append(
            self.ProgressBuild(pathed_sources, target))

    def __call__(self, node, *args, **kw):
        # print(str(node))

        slashed_node = str(node).replace("\\", "/")
        for build in self.progress_builders:
            # print(build.target + ": "+str(node.get_state())+" - " + slashed_node)
            if(slashed_node.endswith("build/" + build.target)):

                if(build.count == 0):
                    self.printer.InfoPrint(
                        self.printer.OKBLUE + "[ " + os.path.splitext(build.target)[0] + " ]" + self.printer.ENDC + " Building " + build.target)

                filename = os.path.basename(slashed_node)
                if(node.get_state() == 2):
                    self.printer.LinkPrint(os.path.splitext(
                        build.target)[0], "Linking " + filename)
                else:
                    self.printer.LinkPrint(os.path.splitext(
                        build.target)[0], "Skipping, already built " + filename)

        # TODO: make hanlding this file extensions better
        if(slashed_node.endswith(".obj")
           or slashed_node.endswith(".o")
           or slashed_node.endswith(".os")):

            slashed_node_file = os.path.splitext(slashed_node)[0]
            # print(" - " + slashed_node_file)
            for build in self.progress_builders:
                try:
                    if(not build.progress_sources[slashed_node_file]):
                        build.progress_sources[slashed_node_file] = True

                        if(build.count == 0):
                            self.printer.InfoPrint(
                                self.printer.OKBLUE + "[ " + os.path.splitext(build.target)[0] + " ]" + self.printer.ENDC + " Building " + build.target)

                        build.count += 1
                        percent = build.count / \
                            len(build.progress_sources.keys()) * 100.00
                        filename = os.path.basename(slashed_node)

                        if(node.get_state() == 2):
                            self.printer.CompilePrint(
                                percent, os.path.splitext(build.target)[0], "Compiling " + filename)
                        else:
                            self.printer.CompilePrint(
                                percent, os.path.splitext(build.target)[0], "Skipping, already built " + filename)
                        break
                except KeyError:
                    pass


def SetupBuildEnv(env, prog_type, prog_name, source_files):

    build_env = env.Clone()
    variant_dir_str = build_env['BUILD_DIR'] + \
        "/" + prog_type + "_" + prog_name + "_objs"
    build_env.VariantDir(variant_dir_str, '.', duplicate=0)
    header_files = []
    if prog_type == "unit":
        temp = []
        for source in source_files:
            temp.append(source.replace(".h", ".c"))
        header_files = source_files
        source_files = temp

    variant_source_files = []
    for file in source_files:
        variant_source_files.append(file.replace(
            build_env['BUILD_DIR'], variant_dir_str))

    win_redirect = ""
    linux_redirect = "2>&1"
    if("Windows" in platform.system()):
        win_redirect = "2>&1"
        linux_redirect = ""

    source_objs = []
    for file in variant_source_files:
        if(prog_type == 'shared'):
            build_obj = build_env.SharedObject(file,
                                               SHCCCOM=build_env['SHCCCOM'] + " " + win_redirect + " > \"" + build_env['PROJECT_DIR'] + "/build/build_logs/" + os.path.splitext(
                                                   os.path.basename(file))[0] + "_compile.txt\" " + linux_redirect,
                                               SHCXXCOM=build_env['SHCXXCOM'] + " " + win_redirect + " > \"" + build_env['PROJECT_DIR'] + "/build/build_logs/" + os.path.splitext(os.path.basename(file))[0] + "_compile.txt\" " + linux_redirect)
            source_objs.append(build_obj)
        elif(prog_type == 'static' or prog_type == 'exec'):

            filename = os.path.splitext(os.path.basename(file))[0]
            build_obj = build_env.Object(file,
                                         CCCOM=build_env['CCCOM'] + " " + win_redirect + " > \"" + build_env['PROJECT_DIR'] +
                                         "/build/build_logs/" + filename + "_compile.txt\" " + linux_redirect,
                                         CXXCOM=build_env['CXXCOM'] + " " + win_redirect + " > \"" + build_env['PROJECT_DIR'] + "/build/build_logs/" + filename + "_compile.txt\" " + linux_redirect)
            source_objs.append(build_obj)

    if(prog_type == 'shared'):
        if("Windows" in platform.system()):
            combinedActions = build_env['SHLINKCOM'].list

            new_actions = []
            for i in range(len(combinedActions)):
                if(i == 0):
                    new_actions.append(Action.Action('${TEMPFILE("$SHLINK $SHLINKFLAGS $_SHLINK_TARGETS $_LIBDIRFLAGS $_LIBFLAGS $_PDB $_SHLINK_SOURCES", "$SHLINKCOMSTR")} 2>&1 > \"' +
                                                     build_env['PROJECT_DIR'] + '/build/build_logs/' + prog_name + '_link.txt\"'))
                else:
                    new_actions.append(combinedActions[i])
            build_env['SHLINKCOM'] = new_actions
        else:
            linkcom_string_match = re.sub(
                r"\s\>\".*", "\",", build_env['SHLINKCOM'])
            build_env['SHLINKCOM'] = linkcom_string_match + str(
                " > " + build_env['PROJECT_DIR'] + "/build/build_logs/" + prog_name + "_link.txt 2>&1")
    elif(prog_type == 'static' or prog_type == 'exec' or prog_type == 'unit'):
        if("Windows" in platform.system()):
            try:
                combinedActions = build_env['LINKCOM'].list
            except AttributeError as e:
                combinedActions = build_env['LINKCOM']
            new_actions = []
            for i in range(len(combinedActions)):
                if(i == 0):
                    new_actions.append(Action.Action('${TEMPFILE("$LINK $LINKFLAGS /OUT:$TARGET.windows $_LIBDIRFLAGS $_LIBFLAGS $_PDB $SOURCES.windows", "$LINKCOMSTR")} 2>&1 > \"' +
                                                     build_env['PROJECT_DIR'] + '/build/build_logs/' + prog_name + '_link.txt\"'))
                else:
                    new_actions.append(combinedActions[i])

            build_env['LINKCOM'] = new_actions

        else:
            linkcom_string_match = re.sub(
                r"\s\>\".*", "\",", build_env['LINKCOM'])
            build_env['LINKCOM'] = linkcom_string_match + str(
                " > " + build_env['PROJECT_DIR'] + "/build/build_logs/" + prog_name + "_link.txt 2>&1")

    if(prog_type == "shared"):
        prog = build_env.SharedLibrary(
            build_env['BUILD_DIR'] + "/" + prog_name, source_objs)

    elif(prog_type == "static"):
        prog = build_env.StaticLibrary(
            build_env['BUILD_DIR'] + "/" + prog_name, source_objs)

    elif(prog_type == 'exec'):
        prog = build_env.Program(
            build_env['BUILD_DIR'] + "/" + prog_name, source_objs)

    elif(prog_type == 'unit'):
        prog = build_env.CxxTest(
            build_env['BUILD_DIR'] + "/" + prog_name, header_files)

    if not os.path.exists(build_env['PROJECT_DIR'] + "/build/build_logs"):
        os.makedirs(build_env['PROJECT_DIR'] + "/build/build_logs")

    # if ARGUMENTS.get('fail', 0):
    #    Command('target', 'source', ['/bin/false'])

    def print_cmd_line(s, targets, sources, env):
        with open(env['PROJECT_DIR'] + "/build/build_logs/build_" + env['BUILD_LOG_TIME'] + ".log", "a") as f:
            f.write(s + "\n")

    build_env['BUILD_LOG_TIME'] = datetime.datetime.fromtimestamp(
        time.time()).strftime('%Y_%m_%d__%H_%M_%S')
    if(GetOption('no_progress')):
        build_env['PRINT_CMD_LINE_FUNC'] = print_cmd_line

    built_bins = []
    if("Windows" in platform.system()):
        if(prog_type == 'shared'):
            built_bins.append(build_env['BUILD_DIR'] + "/" + build_env.subst(
                '$SHLIBPREFIX') + prog_name + build_env.subst('$SHLIBSUFFIX'))
        elif(prog_type == 'static'):
            built_bins.append(build_env['BUILD_DIR'] + "/" + build_env.subst(
                '$LIBPREFIX') + prog_name + build_env.subst('$LIBSUFFIX'))
        elif(prog_type == 'exec'):
            built_bins.append(build_env['BUILD_DIR'] + "/" + build_env.subst(
                '$PROGPREFIX') + prog_name + build_env.subst('$PROGSUFFIX'))
    else:
        if(prog_type == 'shared'):
            built_bins.append(build_env['BUILD_DIR'] + "/" + build_env.subst(
                '$SHLIBPREFIX') + prog_name + build_env.subst('$SHLIBSUFFIX'))
        elif(prog_type == 'static'):
            built_bins.append(build_env['BUILD_DIR'] + "/" + build_env.subst(
                '$LIBPREFIX') + prog_name + build_env.subst('$LIBSUFFIX'))
        elif(prog_type == 'exec'):
            built_bins.append(build_env['BUILD_DIR'] + "/" + build_env.subst(
                '$PROGPREFIX') + prog_name + build_env.subst('$PROGSUFFIX'))

    return [build_env, prog]


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


def run_unit_tests(base_dir):
    """
    Callback function to run the test script.
    """

    test_env = os.environ
    test_env['TEST_BIN_DIR'] = base_dir+'/build/bin'

    proc = subprocess.Popen(
        args=['python', 'run_unit_tests.py'],
        cwd=base_dir+'/Testing',
        env=test_env
    )
    output = proc.communicate()[0]
    # print(output)


def run_visual_tests(base_dir):
    """
    Callback function to run the test script.
    """
    if('SIKULI_DIR' not in os.environ
       and not os.path.isdir(base_dir+'/Testing/VisualTests/SikuliX')):

        printer = ColorPrinter()
        printer.InfoPrint(
            ' Need to download and install sikuli... please be extra patient...')
        proc = subprocess.Popen([sys.executable, 'install_sikuliX.py'],
                                cwd=base_dir+'/Testing/VisualTests',
                                stderr=subprocess.STDOUT,
                                stdout=subprocess.PIPE,
                                shell=False)
        output, result = proc.communicate()
        output = output.decode("utf-8")
        if 'RunSetup: ... SikuliX Setup seems to have ended successfully ;-)' in output:
            printer.InfoPrint(' Silkuli Installed!')
        else:
            printer.InfoPrint(' Silkuli Failed to install!:')
            print(output.decode("utf-8"))

    test_env = os.environ
    if 'SIKULI_DIR' not in os.environ:
        test_env['SIKULI_DIR'] = base_dir+'/Testing/VisualTests/SikuliX'

    test_env['TEST_BIN_DIR'] = base_dir+'/build/bin'

    if 'DISPLAY' not in test_env:
        test_env['DISPLAY'] = ':0'

    proc = subprocess.Popen(
        args=['python', 'run_visual_tests.py'],
        cwd=base_dir+'/Testing',
        env=test_env
    )
    output = proc.communicate()[0]
    print(output)


def cppcheck_command(base_dir):
    """
    Callback function to run the test script.
    """
    printer = ColorPrinter()
    if "windows" in platform.system().lower():
        cppcheck_exec = base_dir+'/build/bin/cppcheck.exe'
    else:
        cppcheck_exec = './cppcheck'

    def execute():

        proc = subprocess.Popen(
            [cppcheck_exec,
                '--enable=all',
                '--suppress=*:../include/glm*',
                '--suppress=*:../../Testing*',
                '-I../include',
                '-DGLM_FORCE_RADIANS',
                '-DODGL_LIBRARAY_BUILD',
                '../../Core',
                '../../AppFrameworks',
                '../../Testing'
             ],
            cwd=base_dir+'/build/bin',
            stderr=subprocess.STDOUT,
            stdout=subprocess.PIPE,
            universal_newlines=True
        )
        for stdout_line in iter(proc.stdout.readline, ""):
            yield stdout_line
        proc.stdout.close()
        return_code = proc.wait()
        if return_code:
            raise subprocess.CalledProcessError(return_code, cmd)

    style = ' (style) '
    performance = ' (performance) '
    portability = ' (portability) '
    warning = ' (warning) '
    error = ' (error) '
    information = ' (information) '

    errors = 0
    warnings = 0
    noncritical = 0

    for output in execute():
        if(output.startswith('[')):

            output = output.strip()

            if(style in output):
                noncritical += 1
                output = printer.highlight_word(
                    output, ' (style) ', printer.OKBLUE)
            elif(performance in output):
                noncritical += 1
                output = printer.highlight_word(
                    output, ' (performance) ', printer.OKBLUE)
            elif(portability in output):
                noncritical += 1
                output = printer.highlight_word(
                    output, ' (portability) ', printer.OKBLUE)
            elif(warning in output):
                warnings += 1
                output = printer.highlight_word(
                    output, ' (warning) ', printer.WARNING)
            elif(error in output):
                errors += 1
                output = printer.highlight_word(
                    output, ' (error) ', printer.FAIL)
            else:
                noncritical += 1

            printer.CppCheckPrint(' ' + output)

    printer.InfoPrint(' Cppcheck finished, findings:')
    printer.InfoPrint('     Non-Critical: ' + str(noncritical))
    printer.InfoPrint('     Warnings:     ' + str(warnings))
    printer.InfoPrint('     Errors:       ' + str(errors))

    # TODO: enable once all cppcheck errors are cleaned up
    # if(noncritical + warnings + errors > 0):
    #    return BuildError(errstr='Cppcheck Failed!')
