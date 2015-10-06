# Text Gale Online

## Project Structure

```
.
+-- apps
|       CMakeLists.txt
|   +-- client
|           client modules
|           CMakeLists.txt
|           README.md
|   +-- server
|           server modules
|           CMakeLists.txt
|           README.md
+-- deps
|       vendor/non-specific modules
|       CMakeLists.txt
|       README.md
+-- include
|       vendor header files
|   .gitingore
|   CMakeLists.txt
|   README.md
```

## Dependencies
To install all the neccessary dependancies on your machine go to the project's root and run:

`$ sudo bash get-deps.sh`

This process might take a while, so go grab some coffee.

## Basic Compilation Guidelines
In project's root folder:
```
$ mkdir -p build
$ cd build
$ cmake ..
$ make
```