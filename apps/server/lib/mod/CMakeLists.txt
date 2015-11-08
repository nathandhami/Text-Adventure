add_library( ObjectEditor ObjectEditor.cpp )
target_link_libraries ( ObjectEditor ${Boost_LIBRARIES} DatabaseTool Server )

add_library( WorldEditor WorldEditor.cpp )
target_link_libraries ( WorldEditor ${Boost_LIBRARIES} DatabaseTool Server ObjectEditor Zone )

add_library( Editor Editor.cpp )
target_link_libraries ( Editor ${Boost_LIBRARIES} WorldEditor ObjectEditor DatabaseTool Server )