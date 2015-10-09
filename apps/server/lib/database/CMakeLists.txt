add_library(wrapped Database.cpp Query.cpp StderrLog.cpp SysLog.cpp)
target_link_libraries ( wrapped sqlite3)

add_library(DatabaseTool DatabaseTool.cpp)
target_link_libraries ( DatabaseTool sqlite3 wrapped ${Boost_LIBRARIES})