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
import os

def get_cpu_nums():
    # Linux, Unix and MacOS:
    if hasattr( os, "sysconf" ):
        if os.sysconf_names.has_key( "SC_NPROCESSORS_ONLN" ):
            # Linux & Unix:
            ncpus = os.sysconf( "SC_NPROCESSORS_ONLN" )
        if isinstance(ncpus, int) and ncpus > 0:
            return ncpus
        else: # OSX:
            return int( os.popen2( "sysctl -n hw.ncpu")[1].read() )
    # Windows:
    if os.environ.has_key( "NUMBER_OF_PROCESSORS" ):
        ncpus = int( os.environ[ "NUMBER_OF_PROCESSORS" ] );
    if ncpus > 0:
        return ncpus
    return 1 # Default

AddOption(
        '--debug_build',
        dest='debug_build',
        action='store_true',
        metavar='DIR',
        default=False,
        help='Build in debug mode'
    )

mainenv = Environment(DEBUG_BUILD = GetOption('debug_build'),TARGET_ARCH='x86_64')
mainenv.baseProjectDir = os.path.abspath(Dir('.').abspath).replace('\\', '/')

###################################################
# Determine number of CPUs
num_cpus = get_cpu_nums()
print ("Building with %d parallel jobs" % num_cpus)
mainenv.SetOption( "num_jobs", num_cpus )

SConscript('Dependencies/SConscript',
           duplicate = 0,
           exports = 'mainenv')

SConscript('Core/SConscript',
           duplicate = 0,
           exports = 'mainenv')

SConscript('AppFrameworks/SConscript',
           duplicate = 0,
           exports = 'mainenv')
