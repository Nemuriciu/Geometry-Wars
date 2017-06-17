#pragma once
#include "GameObject.h"
using namespace std;
using namespace glm;

class Ship_Laser : public GameObject
{
public:
	float laser_length = 20.0f;
	float laser_width = 5.0f;
	float speed = 15.0f;
	float direction_x, direction_y;
	float mouse_x, mouse_y;
	bool fixed;

public:
	Ship_Laser(float ship_x, float ship_y)
	{
		mesh = CreateLaser("Ship_Laser", corner, laser_length, laser_width, red);
		this->ship_x = ship_x;
		this->ship_y = ship_y;
	};
	~Ship_Laser() {};

	Mesh* CreateLaser(std::string name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 color, bool fill = true)
	{
		glm::vec3 corner = leftBottomCorner;

		vector<VertexFormat> vertices =
		{
			VertexFormat(corner + glm::vec3(-width / 2, 0, 0), color),
			VertexFormat(corner + glm::vec3(width / 2, 0, 0), color),
			VertexFormat(corner + glm::vec3(0, length / 2, 0), color),

			VertexFormat(corner + glm::vec3(0, length / 2, 0), color),
			VertexFormat(corner + glm::vec3(width / 2, length, 0), color),
			VertexFormat(corner + glm::vec3(-width / 2, length, 0), color),

			VertexFormat(corner + glm::vec3(-width / 2, 0, 0), color),
			VertexFormat(corner + glm::vec3(0, length / 2, 0), color),
			VertexFormat(corner + glm::vec3(-width / 2, length, 0), color),

			VertexFormat(corner + glm::vec3(width / 2, 0, 0), color),
			VertexFormat(corner + glm::vec3(width / 2, length, 0), color),
			VertexFormat(corner + glm::vec3(0, length / 2, 0), color)
		};

		Mesh* laser = new Mesh(name);

		vector<unsigned short> indices = { 0, 1, 2, 3 , 4, 5, 6, 7, 8, 9, 10, 11};

		if (!fill) {
			laser->SetDrawMode(GL_LINE_LOOP);
		}
		else
		{
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(2);
			indices.push_back(3);
			indices.push_back(4);
			indices.push_back(5);
			indices.push_back(6);
			indices.push_back(7);
			indices.push_back(8);
			indices.push_back(9);
			indices.push_back(10);
			indices.push_back(11);
		}

		laser->InitFromData(vertices, indices);
		return laser;
	};
	
	void draw_laser(WindowObject* window)
	{
		if(!fixed)
		{
			mouse_x = window->GetCursorPosition().x;
			mouse_y = window->GetResolution().y - window->GetCursorPosition().y;
			fixed = true;
		}

		modelMatrix = mat3(1);
		modelMatrix *= Transform2D::Translate(ship_x, ship_y);

		angle = atan2(mouse_x - ship_x, mouse_y - ship_y);
		modelMatrix *= Transform2D::Rotate(-angle);		

		direction_x = mouse_x - ship_x;
		direction_y = mouse_y - ship_y;
		float hyp = sqrt(direction_x * direction_x + direction_y * direction_y);
		direction_x /= hyp;
		direction_y /= hyp;

		ship_x += direction_x * speed;
		ship_y += direction_y * speed;	

		mouse_x += direction_x * speed;
		mouse_y += direction_y * speed;
	};

	
};
