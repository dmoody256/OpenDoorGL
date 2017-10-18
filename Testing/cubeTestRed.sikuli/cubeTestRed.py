res = exists("1508297323827.png")
click("1508297323827.png")
type(Key.ESC)
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
