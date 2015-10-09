add_library(Zone Zone.cpp)
add_library(World World.cpp)
target_link_libraries ( World ${Boost_LIBRARIES} )