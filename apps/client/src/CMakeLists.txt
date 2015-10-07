add_executable (clienttest client-net-test.cpp)
add_executable (commandUI ui_command.cpp)
target_link_libraries ( clienttest ${Boost_LIBRARIES} Connector )
target_link_libraries ( commandUI ${Boost_LIBRARIES} Connector )
