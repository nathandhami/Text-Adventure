add_library( ConsoleReader ConsoleReader.cpp )
target_link_libraries ( ConsoleReader ConsoleWriter )

add_library( ConsoleWriter ConsoleWriter.cpp )
