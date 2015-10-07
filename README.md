# Text Gale Online

## Project Structure

```
.
+-- apps
|       CMakeLists.txt
|   +-- client
|           +-- include
|           +-- lib
|           +-- src
|           CMakeLists.txt
|           README.md
|   +-- server
|           +-- include
|           +-- lib
|           +-- src
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

`$ mkdir -p build` do this only once (or if your build folder is missing)

`$ cd build`

`$ cmake ..` do this everytime any of the CMakeLists are updated

`$ make`