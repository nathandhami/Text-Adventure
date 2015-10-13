add_library(CommandParser CommandParser.cpp)
add_library(Dictionary DictionaryCmds.cpp)
target_link_libraries ( CommandParser World  )