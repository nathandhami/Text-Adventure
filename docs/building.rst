=========
Building
=========

Dependencies
==============================================

textadventure depends on the following libraries:

* boost
* sqlite (database)
* gtkmm  (gui)
* sphinx (doc)

How to Install Dependencies
==============================================

To install all the neccessary dependancies on your machine go to the project's root and run:

sudo bash get-deps.sh

This process might take a while, so grab some coffee.


Basic Compliation Guidelines
==============================================

Make sure all the necessary dependencies are installed first.
In project's root folder: 

1. Create a new directory for building.

    mkdir -p build

2. Change into the new directory.

    cd build

3. Run CMake inside build folder exactly like this:

    cmake ..

4. Run make inside build folder

    make


