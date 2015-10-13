cmake_minimum_required ( VERSION 2.6 )
project ( TextAdventure )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lsqlite3" )

set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin" )
set ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib" )

find_package( Boost COMPONENTS system filesystem thread REQUIRED )
include_directories ( ${Boost_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include )
link_directories( ${Boost_LIBRARY_DIR} )


add_subdirectory ( apps )