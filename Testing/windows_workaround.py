import sys
import os

# popen is not working with the graphics app called directly
# on windows os using system call wrapped in python called
# from popen
test = sys.argv[1]
working_dir = os.path.dirname(test)
original_dir = os.getcwd()
os.chdir(working_dir)
os.system(test)
os.chdir(original_dir)
