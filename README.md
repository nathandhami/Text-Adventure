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

## Basic Compilation Guidelines
In project's root folder:

`$ mkdir build`

`$ cd build`

`$ cmake ..`