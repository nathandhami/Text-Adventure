add_executable ( servertest server-net-test.cpp )
add_executable ( dbtest databaseTest.cpp )
#add_executable ( testStuff testStuff.cpp )
add_executable ( testmod testmod.cpp )

add_executable ( runServer main.cpp )
target_link_libraries ( runServer Watcher Server )

target_link_libraries ( dbtest DatabaseTool )
target_link_libraries ( servertest ${Boost_LIBRARIES} Watcher Session Authenticator DatabaseTool )
#target_link_libraries ( testStuff CommandParser DatabaseTool World )
target_link_libraries ( testmod Editor DatabaseTool )