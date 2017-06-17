#pragma once
#include "GameObject.h"
using namespace std;
using namespace glm;

class Player_Ship : public GameObject
{
public:
	Player_Ship()
	{
		mesh = CreateShip("Player_Ship", corner, size, yellow);
		ship_x = 640;
		ship_y = 512;
	};
	~Player_Ship(){};

	void draw_ship(WindowObject* window)
	{
		modelMatrix = mat3(1);
		modelMatrix *= Transform2D::Translate(ship_x, ship_y);

		/* Ship rotation */		

		angle = atan2(mouse_x - ship_x, mouse_y - ship_y);
		modelMatrix *= Transform2D::Rotate(-angle);
	};	
};