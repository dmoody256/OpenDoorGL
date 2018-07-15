res = exists("1508297323827.png", 10)

if(res == None):
    print("FAIL: No match found")
else:
    print('INFO: Found match with score: ' + str(res.getScore()))

if(res.getScore() < .98):
    print("FAIL: Match score " + str(res.getScore()) + " too low")
