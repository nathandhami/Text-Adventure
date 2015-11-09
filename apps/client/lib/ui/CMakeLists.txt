#add_library( UIWriter UIWriter.cpp )
#target_link_libraries ( UIWriter ConsoleWriter )
#
#
#add_library( UIReader UIReader.cpp )
#target_link_libraries ( UIReader ConsoleReader )

add_library( LoginFrame LoginFrame.cpp )
add_library( GameFrame GameFrame.cpp )
#target_link_libraries ( UIReader ConsoleReader )

add_library( MainWindow MainWindow.cpp )
target_link_libraries ( MainWindow LoginFrame GameFrame )

#target_link_libraries ( LoginWindow GameWindow )
