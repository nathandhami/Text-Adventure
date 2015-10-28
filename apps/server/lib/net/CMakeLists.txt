add_library(Watcher Watcher.cpp)
target_link_libraries ( Watcher ${Boost_LIBRARIES} )

add_library(Session Session.cpp)
target_link_libraries ( Session ${Boost_LIBRARIES} )