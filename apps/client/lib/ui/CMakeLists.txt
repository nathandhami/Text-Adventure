#add_library( UIWriter UIWriter.cpp )
#target_link_libraries ( UIWriter ConsoleWriter )
#
#
#add_library( UIReader UIReader.cpp )
#target_link_libraries ( UIReader ConsoleReader )

add_library( LoginWindow LoginWindow.cpp )
target_link_libraries ( LoginWindow GameWindow )

add_library( GameWindow GameWindow.cpp )
