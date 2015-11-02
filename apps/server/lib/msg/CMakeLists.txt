add_library( CarrierPigeon CarrierPigeon.cpp )
target_link_libraries ( CarrierPigeon ${Boost_LIBRARIES} Server DatabaseTool )