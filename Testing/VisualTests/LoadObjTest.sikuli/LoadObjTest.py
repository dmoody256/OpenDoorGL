res = exists("1509078145523.png", 10)

if(res == None):
    print("FAIL: No match found")
    exit(1)
else:
    print('INFO: Found match with score: ' + str(res.getScore()))

if(res.getScore() < .98):
    print("FAIL: Match score " + str(res.getScore()) + " too low")
    exit(1)

print("INFO: Passed!")
