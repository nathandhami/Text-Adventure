add_library( Game Game.cpp )
target_link_libraries ( Game ${Boost_LIBRARIES} Transceiver )