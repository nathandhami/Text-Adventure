add_library(Spellcasting Spellcasting.cpp)

target_link_libraries ( Spellcasting ${Boost_LIBRARIES} Combat CombatInstance Character DatabaseTool )
