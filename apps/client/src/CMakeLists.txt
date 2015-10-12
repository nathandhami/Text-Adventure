add_executable (userInterface main.cpp)
target_link_libraries ( userInterface ${Boost_LIBRARIES} Transceiver )

add_executable (clienttest client-net-test.cpp)
target_link_libraries ( clienttest ${Boost_LIBRARIES} Transceiver )

