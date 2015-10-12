add_library(Zone Zone.cpp)
add_library(World World.cpp)
target_link_libraries ( World ${Boost_LIBRARIES} WorldConstants DatabaseTool Command )
target_link_libraries ( Zone WorldConstants DatabaseTool )