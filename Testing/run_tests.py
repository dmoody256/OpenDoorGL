import os
import subprocess
import platform


redCubeCommand = []
if(os.path.isfile("/opt/VirtualGL/bin/vglrun")):
    redCubeCommand.append("vglrun")
redCubeCommand.extend(["./run_test.sh", "RedCubeTest"])

if platform.system() == 'Linux':
    proc = subprocess.Popen(redCubeCommand, cwd="../build/bin", stderr=subprocess.STDOUT, stdout=subprocess.PIPE)
else:
    proc = subprocess.Popen(redCubeCommand, cwd="../build/bin", stderr=subprocess.STDOUT, stdout=subprocess.PIPE, creationflags=subprocess.CREATE_NEW_PROCESS_GROUP)
    
output = subprocess.check_output(["/tank2/SikuliX/runsikulix", "-r", "cubeTestRed.sikuli"])
print("test result: " + str(output))