#include "GeometryWars.h"

#include <Core/Engine.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <Core/GPU/Mesh.h>
#include <GL\GLU.h>
#include <typeinfo>
#include <iterator>
using namespace std;
using namespace glm;

GeometryWars::GeometryWars()
{
}

GeometryWars::~GeometryWars()
{
}

void GeometryWars::Init()
{
	resolution = window->GetResolution();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(vec3(0, 0, 50));
	camera->SetRotation(vec3(0, 0, 0));
	camera->Update();
	cameraInput->SetActive(false);

	current_time = 0;
	last_time = 0;
	spawn_time = 2;

	my_ship = new Player_Ship();
	AddMeshToList(my_ship->mesh);
	my_ship->lives = 3;
}

void GeometryWars::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void GeometryWars::Update(float deltaTimeSeconds)
{
	if (!game_over)
	{
		/* Draw Player Ship */
		my_ship->draw_ship(window);
		RenderMesh2D(meshes["Player_Ship"], shaders["VertexColor"], my_ship->modelMatrix);

		/* Draw Enemy Ships */
		for (int i = 0; i < enemy_ships.size(); i++)
		{
			Enemy_Ship* current = enemy_ships[i];

			if (current->lives > 1)
			{
				current->draw_ship(window, my_ship->ship_x, my_ship->ship_y);
				RenderMesh2D(meshes["Enemy_Ship_lvl_2"], shaders["VertexColor"], current->modelMatrix);
			}
			else if (!current->bigShip)
			{
				current->draw_ship(window, my_ship->ship_x, my_ship->ship_y);
				RenderMesh2D(meshes["Enemy_Ship_lvl_1"], shaders["VertexColor"], current->modelMatrix);
			}
			else
			{
				if (current->scale_x > 0.5f)
				{
					current->scale_x -= deltaTimeSeconds * 2;
					current->scale_y -= deltaTimeSeconds * 2;
				}

				current->draw_ship(window, my_ship->ship_x, my_ship->ship_y);
				RenderMesh2D(meshes["Enemy_Ship_lvl_1"], shaders["VertexColor"], current->modelMatrix);
			}

			/* Check if Player Ship is hit */
			if (isCollision(my_ship, current))
			{
				i--;
				continue;
			}

			/* Check if any enemy ship is hit by laser */
			for (int j = 0; j < ship_lasers.size(); j++)
				if (isCollision(ship_lasers[j], current)) break;
		}

		/* Ship movement with translation */
		if (key_W && my_ship->ship_y < resolution.y)
			my_ship->ship_y += deltaTimeSeconds * 400;
		if (key_A && my_ship->ship_x > 0)
			my_ship->ship_x -= deltaTimeSeconds * 400;
		if (key_S && my_ship->ship_y > 0)
			my_ship->ship_y -= deltaTimeSeconds * 400;
		if (key_D && my_ship->ship_x < resolution.x)
			my_ship->ship_x += deltaTimeSeconds * 400;


		/* Draw Player Ship_Lasers */
		for (int i = 0; i < ship_lasers.size(); i++)
		{
			Ship_Laser* current = ship_lasers[i];
			current->draw_laser(window);

			/* Eliminate laser object if out of screen */
			if (out_of_bounds(current, resolution))
			{
				ship_lasers.erase(ship_lasers.begin() + i);
				i--; // Elements will shift to the left => Redo iteration
			}
			else
				RenderMesh2D(meshes["Ship_Laser"], shaders["VertexColor"], current->modelMatrix);
		}

		if (mouseClick)
		{
			Ship_Laser* ship_laser = new Ship_Laser(my_ship->ship_x, my_ship->ship_y);
			ship_lasers.push_back(ship_laser);
			AddMeshToList(ship_laser->mesh);
			mouseClick = false;
		}
	}
	else
	{
		if(color < 1)
			color += deltaTimeSeconds * 0.5f;

		glClearColor(color, 0, 0, 0.000001);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

bool GeometryWars::isCollision(GameObject* obj1, Enemy_Ship* obj2)
{
	/* Collision Player <-> Enemy */
	if (dynamic_cast<Player_Ship*> (obj1) != NULL)
	{
		float deltaX = obj1->ship_x - obj2->ship_x;
		float deltaY = obj1->ship_y - obj2->ship_y;
		deltaX *= deltaX;
		deltaY *= deltaY;

		float radius = obj1->size + obj2->size; 
		radius *= radius;

		if (deltaX + deltaY <= radius) 
		{			
			obj1->lives--;
			vector<Enemy_Ship*>::iterator position = find(enemy_ships.begin(), enemy_ships.end(), obj2);
			int pos = std::distance(enemy_ships.begin(), position);

			enemy_ships.erase(enemy_ships.begin() + pos);
			cout << "Ouch! -1 Life." << endl;

			return true;
		}
	}

	/* Collision Laser <-> Enemy */
	if (dynamic_cast<Ship_Laser*> (obj1) != NULL)
	{
		float deltaX = obj1->ship_x - obj2->ship_x;
		float deltaY = obj1->ship_y - obj2->ship_y;
		deltaX *= deltaX;
		deltaY *= deltaY;

		float radius = dynamic_cast<Ship_Laser*> (obj1)->laser_length + obj2->size;
		radius *= radius;

		if (deltaX + deltaY <= radius)
		{
			if (obj2->lives > 1)
			{
				obj2->lives--;
				obj2->speed *= 2;
			}
			else
			{
				vector<Enemy_Ship*>::iterator position = find(enemy_ships.begin(), enemy_ships.end(), obj2);
				int pos = std::distance(enemy_ships.begin(), position);
				enemy_ships.erase(enemy_ships.begin() + pos);

				game_score++;
				cout << "Score: " << game_score << endl;
			}

			vector<Ship_Laser*>::iterator _position = find(ship_lasers.begin(), ship_lasers.end(), obj1);
			int _pos = std::distance(ship_lasers.begin(), _position);
			ship_lasers.erase(ship_lasers.begin() + _pos);

			return true;
		}
	}
	return false;
};

bool GeometryWars::out_of_bounds(Ship_Laser* laser, vec2 res)
{
	if (laser->ship_x > res.x || laser->ship_x < 0) return true;
	else if (laser->ship_y > res.y || laser->ship_y < 0) return true;

	return false;
};

void GeometryWars::FrameEnd()
{
	DrawCoordinatSystem();
}

void GeometryWars::OnInputUpdate(float deltaTime, int mods)
{
	my_ship->mouse_x = window->GetCursorPosition().x;
	my_ship->mouse_y = window->GetResolution().y - window->GetCursorPosition().y;

	current_time = Engine::GetElapsedTime();	
	if (current_time >= 15) spawn_time = 0.5f;
	else if (current_time >= 10) spawn_time = 1;

	/* Generate random enemy_ships every spawn_sec time */
	if (current_time - last_time >= spawn_time)
	{
		float radius = 640;
		float theta = fmod(rand(), 2 * M_PI);

		float random_x = resolution.x / 2 + radius * cos(theta);
		float random_y = resolution.y / 2 + radius * sin(theta);

		Enemy_Ship* enemy = new Enemy_Ship(random_x, random_y);

		/* Decide if small/big ship */
		enemy->lives = rand() % 2 + 1;
		if (enemy->lives > 1) enemy->bigShip = true;

		enemy_ships.push_back(enemy);

		AddMeshToList(enemy->mesh);
		AddMeshToList(enemy->mesh_2);

		last_time = Engine::GetElapsedTime();
	}

	if (my_ship->lives <= 0)
		game_over = true;
};

void GeometryWars::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_W)
		key_W = true;
	if (key == GLFW_KEY_A)
		key_A = true;
	if (key == GLFW_KEY_S)
		key_S = true;
	if (key == GLFW_KEY_D)
		key_D = true;
};	

void GeometryWars::OnKeyRelease(int key, int mods)
{
	if (key == GLFW_KEY_W)
		key_W = false;
	if (key == GLFW_KEY_A)
		key_A = false;
	if (key == GLFW_KEY_S)
		key_S = false;
	if (key == GLFW_KEY_D)
		key_D = false;
};

void GeometryWars::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (button == 1)
		mouseClick = true;
};

