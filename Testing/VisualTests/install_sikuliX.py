import os

# Python 2 and 3: alternative 4
try:
    import urllib.request as pyurl
except ImportError:
    import urllib2 as pyurl

import subprocess

if not os.path.exists("SikuliX"):
    os.makedirs("SikuliX")

url = 'https://launchpad.net/sikuli/sikulix/2.0.4/+download/sikulixide-2.0.4.jar'
response = pyurl.urlopen(url)

with open("SikuliX/sikulix.jar", 'wb') as f:
    jarFile = bytearray(response.read())
    f.write(jarFile)

url = 'https://repo1.maven.org/maven2/org/python/jython-standalone/2.7.1/jython-standalone-2.7.1.jar'
response = pyurl.urlopen(url)

with open("SikuliX/jython-standalone-2.7.1.jar", 'wb') as f:
    jarFile = bytearray(response.read())
    f.write(jarFile)
    

##install_sikuli = ["java", "-jar", "sikulixsetup-1.1.1.jar", "options", "1.1"]

#proc = subprocess.Popen(
#    install_sikuli,
#    cwd=os.path.abspath("SikuliX"),
#    stderr=subprocess.STDOUT,
#    stdout=subprocess.PIPE
#)
#output, err = proc.communicate()
#print(output.decode("utf-8"))
