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


## How to run:
To start the server, run: `./server-start.sh`

To start the game, run: `./play.sh`

### Two accounts available to login to:
Username: `userTest1`, password: `test1`

Username: `userTest2`, password: `test2`


## Creator commands
#### Create and delete zones:
```
create zone [The name of your zone] seen as [Description of your zone]
delete zone [zoneID]
```
#### Add extended description to the zone:
```
describe zone [zoneID] as [keyword] telling [your extended description here] 
```
#### Create and delete doors
```
create door [keyword] in [zoneID] at [direction] to [zoneID] seen as [Door's description]
delete door [doorID]
```
#### Create and delete items
```
"a standard issue dagger ~: You see a standard issue dagger here~; You see a dagger of great craftsmanship. Imprinted on the side is: 'Merc Industries'~; dagger, merc"
create item [item name] seen as [what you see] upclose [the long description] recognized as [keywords]
```
