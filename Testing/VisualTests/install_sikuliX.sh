export SIKULI_DIR=$PWD/SikuliX
export DISPLAY=:0
mkdir SikuliX && cd SikuliX
wget https://launchpad.net/sikuli/sikulix/1.1.1/+download/sikulixsetup-1.1.1.jar
java -jar sikulixsetup-1.1.1.jar options 1.1
cd ..
