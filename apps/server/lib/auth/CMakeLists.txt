add_library( User User.cpp )

add_library( Authenticator Authenticator.cpp )
target_link_libraries ( Authenticator DatabaseTool User )