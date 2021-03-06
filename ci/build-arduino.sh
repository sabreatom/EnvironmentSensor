#!/bin/bash
# Exit immediately if a command exits with a non-zero status.
set -e
# Enable the globstar shell option
shopt -s globstar
# Make sure we are inside the github workspace
cd $GITHUB_WORKSPACE
# Create directories
mkdir $HOME/Arduino
mkdir $HOME/Arduino/libraries
# Install Arduino IDE
export PATH=$PATH:$GITHUB_WORKSPACE/bin
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
arduino-cli config init
arduino-cli core update-index

# Install Arduino Nano 33 BLE core
arduino-cli core install arduino:mbed

# Install libraries:
arduino-cli lib install ArduinoBLE
arduino-cli lib install Arduino_HTS221

# Compile all *.ino files for the Arduino Nano 33 BLE
for f in **/*.ino ; do
    arduino-cli compile -b arduino:mbed:nano33ble $f
done
