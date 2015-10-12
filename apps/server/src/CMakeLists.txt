add_executable ( servertest server-net-test.cpp )
add_executable( dbtest databaseTest.cpp )

target_link_libraries (dbtest DatabaseTool)
target_link_libraries ( servertest ${Boost_LIBRARIES} Watcher Session Authenticator DatabaseTool )
