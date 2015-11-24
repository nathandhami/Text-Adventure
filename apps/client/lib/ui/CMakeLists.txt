#add_library( UIWriter UIWriter.cpp )
#target_link_libraries ( UIWriter ConsoleWriter )
#
#
#add_library( UIReader UIReader.cpp )
#target_link_libraries ( UIReader ConsoleReader )

add_library( LoginFrame LoginFrame.cpp )
target_link_libraries( LoginFrame ${GTKMM_LIBRARIES} Game )

add_library( RegisterFrame RegisterFrame.cpp )
target_link_libraries( RegisterFrame ${GTKMM_LIBRARIES} Game )

add_library( GameFrame GameFrame.cpp )
target_link_libraries( GameFrame ${GTKMM_LIBRARIES} Game )
#target_link_libraries ( UIReader ConsoleReader )

add_library( MainWindow MainWindow.cpp )
target_link_libraries ( MainWindow LoginFrame RegisterFrame GameFrame )

#target_link_libraries ( LoginWindow GameWindow )
