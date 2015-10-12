cmake_minimum_required(VERSION 2.8.12)
project(yamlparser)
#set(sourceFiles main.cpp SortIntegers.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
#include_directories(${CMAKE_SOURCE_DIR}/include)
#add_subdirectory (lib)
add_subdirectory (yaml-cpp-release-0.5.2)

#install(FILES include/SortIntegers.h DESTINATION include)
SET(YAMLCPP_STATIC_LIBRARY TRUE)
#add_executable(sorter ${sourceFiles})
find_package(YamlCpp REQUIRED)
include_directories(${YamlCpp_INCLUDE_DIRS})
set(LIBS ${LIBS} ${YamlCpp_LIBRARIES})

add_executable (yaml yamlparser.cpp)
target_link_libraries (yaml ${LIBS})

#install(TARGETS fibSorter DESTINATION bin)


