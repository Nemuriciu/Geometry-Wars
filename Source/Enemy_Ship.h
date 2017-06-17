#pragma once
#include <iostream>
using namespace std;
using namespace glm;

class Enemy_Ship : public GameObject
{
public:
	Enemy_Ship(float ship_x, float ship_y)
	{
		mesh = CreateShip("Enemy_Ship_lvl_2", corner, size, red);
		mesh_2 = CreateShip("Enemy_Ship_lvl_1", corner, size, blue);

		this->ship_x = ship_x;
		this->ship_y = ship_y;

		scale_x = 1;
		scale_y = 1;

		cx = corner.x + size / 2;
		cy = corner.y + size / 2;
	};
	~Enemy_Ship() {};

	void draw_ship(WindowObject* window, float player_x, float player_y)
	{
		/* Change direction towards the player_ship*/
		direction_x = player_x - ship_x;
		direction_y = player_y - ship_y;
		float hyp = sqrt(direction_x * direction_x + direction_y * direction_y);
		direction_x /= hyp;
		direction_y /= hyp;		

		ship_x += direction_x * speed;
		ship_y += direction_y * speed;

		modelMatrix = mat3(1);
		modelMatrix *= Transform2D::Translate(ship_x, ship_y);

		/* Rotate to face the player_ship */
		angle = atan2(player_x - ship_x, player_y - ship_y);
		modelMatrix *= Transform2D::Rotate(-angle);	

		/* Scale enemy_ship if hit */
		modelMatrix *= Transform2D::Translate(cx, cy);
		modelMatrix *= Transform2D::Scale(scale_x, scale_y);
		modelMatrix *= Transform2D::Translate(-cx, -cy);
	};

public:
	float direction_x, direction_y;
	float speed = 3.0f;
	float scale_x, scale_y;
	float cx, cy;
	bool bigShip;
	Mesh* mesh_2;
};