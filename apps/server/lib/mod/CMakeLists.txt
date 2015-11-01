add_library( WorldEditor WorldEditor.cpp )
target_link_libraries ( WorldEditor ${Boost_LIBRARIES} DatabaseTool Server )

add_library( Editor Editor.cpp )
target_link_libraries ( Editor ${Boost_LIBRARIES} WorldEditor DatabaseTool Server )