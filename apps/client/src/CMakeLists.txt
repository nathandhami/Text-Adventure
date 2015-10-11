add_executable (clienttest client-net-test.cpp)
target_link_libraries ( clienttest ${Boost_LIBRARIES} Connector )