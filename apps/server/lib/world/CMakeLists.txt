add_library(Zone Zone.cpp)
add_library(World World.cpp)
add_library(WorldConstants WorldConstants.cpp)

target_link_libraries ( World ${Boost_LIBRARIES} DatabaseTool Zone WorldConstants Character Combat )
target_link_libraries ( Zone DatabaseTool Character WorldConstants )
target_link_libraries ( WorldConstants ${Boost_LIBRARIES} )