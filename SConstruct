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

###
# build subProjects
dependLibs= SConscript('Dependencies/SConscript',  duplicate = 0, exports = 'mainenv')
coreLib =   SConscript('Core/SConscript',          duplicate = 0, exports = 'mainenv')
framework = SConscript('AppFrameworks/SConscript', duplicate = 0, exports = 'mainenv')

tests = []
for root, dirs, files in os.walk("Testing"):
    for file in files:
        if file.endswith("SConscript"):
             tests.append(SConscript(os.path.join(root, file), duplicate = 0, exports = 'mainenv'))

# setup installs
for header in dependLibs['headers']['GLEW']:
    mainenv.Install("build/include/GLEW", header)
for header in dependLibs['headers']['GLFW']:
    mainenv.Install("build/include/GLFW", header)

for lib in dependLibs['libs']:
    mainenv.Depends(coreLib, lib )
    mainenv.Install("build/lib", lib)

for buildFile in coreLib:
    if(str(buildFile).endswith(".h")):
        mainenv.Install("build/include", buildFile)
    else:
        mainenv.Install("build/lib", buildFile)

mainenv.Depends(framework,coreLib )

for buildFile in framework:
    if(str(buildFile).endswith(".h")):
        mainenv.Install("build/include", buildFile)
    else:
        mainenv.Install("build/lib", buildFile)

mainenv.Install("build/bin", File("Testing/run_test.sh"))
mainenv.Install("build/bin", File("Testing/debug_test.sh"))

for test in tests:
    mainenv.Depends(test['executable'], framework)
    mainenv.Install("build/bin", test['executable'])
    for resource in test['resources']:
        mainenv.Install("build/bin/resources", resource)




 