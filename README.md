# VascuSynth

Follow the steps below meticulously to compile the code base:

0. `git clone https://github.com/sfu-mial/VascuSynth`
1. `sudo apt-get install libinsighttoolkit4-dev`
2. `git clone -b v4.13.3 https://github.com/InsightSoftwareConsortium/ITK ITK-4.13.3`
3. `cd ITK-4.13.3`
4. `cmake .`
5. `make -j32`

To run the built binary file:

`./VascuSynth paramFiles.txt imageNames.txt 0.04`
