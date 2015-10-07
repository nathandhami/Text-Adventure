add_executable (servertest server-net-test.cpp)
target_link_libraries ( servertest ${Boost_LIBRARIES} Watcher )