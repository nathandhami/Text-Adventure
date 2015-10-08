add_library(Watcher Watcher.cpp)
target_link_libraries ( Watcher ${Boost_LIBRARIES} )

add_library(Connection Connection.cpp)
target_link_libraries ( Connection ${Boost_LIBRARIES} )