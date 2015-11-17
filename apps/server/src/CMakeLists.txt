add_executable ( runServer main.cpp )
target_link_libraries ( runServer Server )

add_executable ( testk testk.cpp )
target_link_libraries ( testk Server ${Boost_LIBRARIES} )





add_executable ( dbtest databaseTest.cpp )
#add_executable ( testStuff testStuff.cpp )

target_link_libraries (dbtest DatabaseTool)

add_executable(yamltest yamlparser.cpp)
#target_link_libraries (yamltest DatabaseTool /usr/local/lib/libyaml-cpp.a)
target_link_libraries (yamltest DatabaseTool ${Boost_LIBRARIES} ${CMAKE_SOURCE_DIR}/apps/server/lib/yaml-cpp/libyaml-cpp.a)


target_link_libraries ( dbtest DatabaseTool )
#target_link_libraries ( testStuff CommandParser DatabaseTool World )
