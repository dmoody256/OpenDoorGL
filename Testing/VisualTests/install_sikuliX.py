import os
import urllib.request
import subprocess

if not os.path.exists("SikuliX"):
    os.makedirs("SikuliX")

url = 'https://launchpad.net/sikuli/sikulix/1.1.1/+download/sikulixsetup-1.1.1.jar'
response = urllib.request.urlopen(url)

with open("SikuliX/sikulixsetup-1.1.1.jar", 'wb') as f:
    jarFile = bytearray(response.read())
    f.write(jarFile)

install_sikuli = ["java", "-jar", "sikulixsetup-1.1.1.jar", "options", "1.1"]

proc = subprocess.Popen(
    install_sikuli,
    cwd=os.path.abspath("SikuliX"),
    stderr=subprocess.STDOUT,
    stdout=subprocess.PIPE
)
output = proc.communicate()[0].decode("utf-8")
print(output)
