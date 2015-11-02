cmake_minimum_required ( VERSION 2.6 )
project ( TextAdventure )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lsqlite3" )

set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin" )
set ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib" )

find_package( PkgConfig REQUIRED )
pkg_check_modules( GTKMM REQUIRED gtkmm-3.0 )
find_package( Boost COMPONENTS system filesystem thread regex REQUIRED )


include_directories ( 
	${Boost_INCLUDE_DIR} 
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/include/msqlite
	${GTKMM_INCLUDE_DIRS}
)

link_directories( ${Boost_LIBRARY_DIR} ${GTKMM_LIBRARY_DIRS} )

add_subdirectory ( apps )
