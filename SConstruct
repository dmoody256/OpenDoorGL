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
import stat
import subprocess
import SCons.Action

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
        ncpus = int( os.environ[ "NUMBER_OF_PROCESSORS" ] )
    if ncpus > 0:
        return ncpus
    return 1 # Default

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
tests =     SConscript('Testing/SConscript', duplicate = 0, exports = 'mainenv')

# setup installs
for header in dependLibs['headers']['GLEW']:
    mainenv.Install("build/include/GLEW", header)
for header in dependLibs['headers']['GLFW']:
    mainenv.Install("build/include/GLFW", header)

testnode = mainenv.Install("build/include", 'Dependencies/glm/glm')

for lib in dependLibs['libs']:
    mainenv.Depends(coreLib, lib )
    mainenv.Install("build/lib", lib)

for buildFile in coreLib:
    if(str(buildFile).endswith(".hpp")):
        mainenv.Install("build/include", buildFile)
    else:
        mainenv.Install("build/lib", buildFile)

mainenv.Depends(framework,coreLib )

for buildFile in framework:
    if(str(buildFile).endswith(".hpp")):
        mainenv.Install("build/include", buildFile)
    else:
        mainenv.Install("build/lib", buildFile)

def ChmodBuildDir():

    def make_executable(path):
        mode = os.stat(path).st_mode
        mode |= (mode & 0o444) >> 2    # copy R bits to X
        os.chmod(path, mode)
        
    for dir in ['build']:
        for root, dirs, files in os.walk(dir):
            for f in files:
                make_executable(os.path.join(root, f))

chmod_callback = SCons.Action.ActionFactory( ChmodBuildDir,
            lambda: 'Setting build to exec permissions')

chmod_command = mainenv.Command('never_exists', 'build', chmod_callback())
mainenv.Depends(chmod_command,framework )
mainenv.AlwaysBuild(chmod_command )

def RunTests():
    
    if(not os.path.isdir(mainenv.baseProjectDir+'/Testing/SikuliX')):
        proc = subprocess.Popen(args=['./install_sikuliX.sh'], cwd=mainenv.baseProjectDir+'/Testing', stdout=subprocess.PIPE, shell=True)
        output = proc.communicate()[0]
        print(output)

    test_env = os.environ
    test_env['SIKULI_DIR'] = mainenv.baseProjectDir+'/Testing/SikuliX'
    proc = subprocess.Popen(args=['python', 'run_tests.py'], cwd=mainenv.baseProjectDir+'/Testing', env=test_env)
    output = proc.communicate()[0]
    print(output)

test_callback = SCons.Action.ActionFactory( RunTests,
            lambda: 'Running Tests... Please be Patient')
            
tests_bins = []
for test in tests:
    mainenv.Depends(test['executable'], framework)
    tests_bins.append(mainenv.Install("build/bin", test['executable']))
    for resource in test['resources']:
        mainenv.Install("build/bin/resources", resource)
    mainenv.Depends(chmod_command, test['executable'] ) 

if(GetOption('run_test')):
    test_command = mainenv.Command(Dir('Testing/SikuliX'), tests_bins, test_callback())
    mainenv.Depends(test_command,chmod_command )
    mainenv.AlwaysBuild(test_command)
