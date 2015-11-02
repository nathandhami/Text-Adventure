#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

#include "Watcher.hpp"


int main() {
	Watcher watcher;
	watcher.run();
	watcher.wait();
	
	return 0;
}