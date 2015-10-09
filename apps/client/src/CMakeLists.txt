add_executable (userInterface user_interface.cpp)
target_link_libraries ( userInterface ${Boost_LIBRARIES} Connector )
