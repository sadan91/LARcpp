# LARcpp
## A C++ porting of the LAR library

## Installation
For compilation cmake (https://cmake.org/) is required

### Eclipse project

To import this project into eclipse open a cmd prompt into the project directory and type:

    cmake -G "Eclipse CDT4 - Unix Makefiles" ./

### Compilation and installation

Open a cmd prompt into the project directory and type:
    
    mkdir build
    cd build
    cmake ../
    make
    sudo make install
    
The library will be installed in /usr/local/lib/libLARcpp.so

The header files will be installed under /usr/local/include