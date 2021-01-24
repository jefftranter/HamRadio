A Qt-based application for calculating values of precision resistors
using several standard values in parallel and/or series.

Requires the Qt framework. Tested with Qt 5.11.3.

You can build and run it with the Qt Creator IDE or manually using
commands like the following (on Linux):

    mkdir build
    cd build
    cmake ..
    make
    ./precresistor

It can also be build using qmake using the provided project file, e.g.

    qmake
    make
