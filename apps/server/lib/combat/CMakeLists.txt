add_library(Combat Combat.cpp)
add_library(CombatInstance CombatInstance.cpp)
target_link_libraries ( Combat DatabaseTool CombatInstance Server )
target_link_libraries ( CombatInstance DatabaseTool Zone Server CastSpell Character ${CMAKE_THREAD_LIBS_INIT} )