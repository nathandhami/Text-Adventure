add_library( Watcher Watcher.cpp )
target_link_libraries ( Watcher ${Boost_LIBRARIES} Session Server )

add_library( Session Session.cpp )
target_link_libraries ( Session ${Boost_LIBRARIES} CommandParser Authenticator Server User Commander CarrierPigeon Editor CharacterManager )