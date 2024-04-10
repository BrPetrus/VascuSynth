# VascuSynth

For more information about VascuSynth, please visit: https://vascusynth.cs.sfu.ca/

Follow the steps below meticulously to compile the code base on Ubuntu:

0. `git clone https://github.com/sfu-mial/VascuSynth`
1. `sudo apt-get install libinsighttoolkit4-dev`
2. `git clone -b v4.13.3 https://github.com/InsightSoftwareConsortium/ITK ITK-4.13.3`
3. `cd ITK-4.13.3`
4. `cmake -B build `
5. `cd build && make -j32 && cd ..`
6. `cmake . # to build the vascusynth bianry`
7. `make -j32`


**NOTE**: Dependencies for Windows 10: CMake 3.29.1 + VS Studio 2022 community + MSVC v140 C++ build tools (v14.00) + ITK 5.1.0

To run the built binary file:

`./VascuSynth paramFiles.txt imageNames.txt 0.04`
