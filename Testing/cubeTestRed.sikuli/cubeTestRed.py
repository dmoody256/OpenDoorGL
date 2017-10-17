import subprocess
import os

type ("T", KeyModifier.SHIFT + KeyModifier.KEY_CTRL )
type("cd /tank2/OpenDoorGL/build/bin\n")
type("vglrun ./run_test.sh RedCubeTest\n")
res = exists("1507766021321.png")
type(Key.ESC)
click("1507766167995.png")
type ("W", KeyModifier.SHIFT + KeyModifier.KEY_CTRL )
if(res == None):
    print("Failure") 
    exit(1)
print res.getScore()
if(res.getScore() < .999):
    print("Failure")
    exit(2)
else:
    print("Success")
    exit(0)
