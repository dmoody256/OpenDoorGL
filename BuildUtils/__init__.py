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
    if 'PYTHONPATH' in test_env:
        test_env['PYTHONPATH'] = test_env['PYTHONPATH'] + os.pathsep + base_dir
    else:
        test_env['PYTHONPATH'] = base_dir

    proc = subprocess.Popen(
        args=['python', 'run_unit_tests.py'],
        cwd=base_dir+'/Testing',
        env=test_env
    )
    output = proc.communicate()[0]
    # print(output)


def convertShadersToHeaders(shaderHeader, shaderFiles):

    with open(shaderHeader, 'w') as header:
        header.write('''#ifndef ODGL_SHADERS_H
#define ODGL_SHADERS_H

#include "odgl_Include.hpp"

#include <string>

namespace OpenDoorGL
{
    ''')
        for shader in shaderFiles:
            with open(shader) as f:
                lines = f.readlines()
            header.write('static const std::string ' +
                         os.path.splitext(os.path.basename(shader))[0] + "_" + os.path.splitext(os.path.basename(shader))[1][1:] + ' = ')
            for line in lines:
                line = line.strip()
                header.write('"' + line + '\\n"\n')
            header.write(';\n\n')

        header.write("}\n#endif\n")


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
    output, err = proc.communicate()
    if not proc.returncode == 0:
        exit(proc.returncode)
