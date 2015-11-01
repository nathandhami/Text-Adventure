add_library( UIWriter UIWriter.cpp )
target_link_libraries ( UIWriter ConsoleWriter )


add_library( UIReader UIReader.cpp )
target_link_libraries ( UIReader ConsoleReader )