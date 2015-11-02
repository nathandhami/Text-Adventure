add_library( ItemEditor ItemEditor.cpp )
target_link_libraries ( ItemEditor ${Boost_LIBRARIES} DatabaseTool Server )

add_library( WorldEditor WorldEditor.cpp )
target_link_libraries ( WorldEditor ${Boost_LIBRARIES} DatabaseTool Server ItemEditor )

add_library( Editor Editor.cpp )
target_link_libraries ( Editor ${Boost_LIBRARIES} WorldEditor ItemEditor DatabaseTool Server )