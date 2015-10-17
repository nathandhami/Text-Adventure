add_library( Server Server.cpp )
target_link_libraries ( Server Watcher Session )