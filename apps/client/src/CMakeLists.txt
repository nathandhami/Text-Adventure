add_executable ( runClient client.cpp )
#target_link_libraries ( runClient Transceiver UIWriter UIReader MainWindow ${GTKMM_LIBRARIES})
target_link_libraries ( runClient Game MainWindow SubWindow ${GTKMM_LIBRARIES} )
