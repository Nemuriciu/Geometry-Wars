#include <ctime>
#include <iostream>
#include <Core/Engine.h>
#include "GeometryWars.h"
using namespace std;

int main(int argc, char **argv)
{
	srand((unsigned int)time(NULL));

	WindowProperties wp;
	wp.resolution = glm::ivec2(1280, 1024);

	WindowObject* window = Engine::Init(wp);

	World *world = new GeometryWars();
	world->Init();
	world->Run();
	Engine::Exit();

	return 0;
}