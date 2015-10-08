add_library(DatabaseTool DatabaseTool.cpp)
target_link_libraries ( DatabaseTool ${SQLITE3_LIBRARIES} )