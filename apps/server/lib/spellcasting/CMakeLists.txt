add_library(Spellcasting Spellcasting.cpp)
add_library(CastSpell CastSpell.cpp)

target_link_libraries ( Spellcasting ${Boost_LIBRARIES} Combat CastSpell Character DatabaseTool )
target_link_libraries ( CastSpell ${Boost_LIBRARIES} Server Character Zone DatabaseTool )