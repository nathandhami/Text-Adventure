add_library(Combat Combat.cpp)
add_library(CombatInstance CombatInstance.cpp)
target_link_libraries ( Combat DatabaseTool CombatInstance )
target_link_libraries ( CombatInstance DatabaseTool ${CMAKE_THREAD_LIBS_INIT} )