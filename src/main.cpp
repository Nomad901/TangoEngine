#include <iostream>
#include "Program.h"

int32_t main(int32_t argc, int8_t* argv[])
{
	Program program;
	program.run();
}

/*
	Problems: 
	1. It seems like frustum culling is working great with fps camera, but it doesnt work correctly with 3rd one - frustum culling / 3rd person camera
	2. the terrain itself is really crazy, thats not a terrain - this is just bunch of angles from the floor - terrain's surface
	   - try to append another algorithm for the terrain, more modern version
	3. light system is crazy, working incorectly - light system / slope light


	1. collision
	2. frustum culling 
	3. light
	4. water 
	5. repair other terrain generations 
*/