add_executable ( servertest server-net-test.cpp )
add_executable ( dbtest databaseTest.cpp )
add_executable ( testStuff testStuff.cpp )
add_executable ( testmod testmod.cpp )

<<<<<<< HEAD
target_link_libraries (dbtest DatabaseTool)

add_executable(yamltest yamlparser.cpp)
#target_link_libraries (yamltest DatabaseTool /usr/local/lib/libyaml-cpp.a)
target_link_libraries (yamltest DatabaseTool ${Boost_LIBRARIES} ${CMAKE_SOURCE_DIR}/apps/server/lib/yaml-cpp/libyaml-cpp.a)

target_link_libraries ( servertest ${Boost_LIBRARIES} Watcher Session Authenticator DatabaseTool )

add_executable ( runServer main.cpp )
target_link_libraries ( runServer Watcher Server )

target_link_libraries ( dbtest DatabaseTool )
target_link_libraries ( servertest ${Boost_LIBRARIES} Watcher Session Authenticator DatabaseTool )
target_link_libraries ( testStuff CommandParser DatabaseTool World )
target_link_libraries ( testmod Editor DatabaseTool )

