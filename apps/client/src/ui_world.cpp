//user interface for world
#include <iostream>

//display size of the world
const int worldWidth = 100;
const int worldHeight = 100;

bool showWorld();
void showCommand();

int main ()
{
	//show world
	if(showWorld())
	{
		//insert command at the bottom
		showCommand();
	}

	return 0;
}

bool showWorld()
{
	//get world corresponding to the current location
	bool connection = true;
	if(!connection)
	{
		std::cout << "sorry your connection is bad\n" << std::endl; 
		return false;
	}
	else
	{
		std::cout << "your current world is " << std::endl;
		//parse world

		for (int i = 0; i < worldHeight; i++)
		{
			//display the world line by line
		}
		return true;
	}
	return false;
}

void showCommand()
{
	std::cout << "\n" << std::endl;
	std::cout << "please insert command: " << std::endl;
	return;
}