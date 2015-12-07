add_library( Character Character.cpp )
target_link_libraries( Character DatabaseTool )

add_library( CharacterManager CharacterManager.cpp )
target_link_libraries( CharacterManager DatabaseTool User )