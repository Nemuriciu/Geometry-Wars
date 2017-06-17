#pragma once
#include "Transform2D.h"
#include <Core/Engine.h>
#include <cmath>
#include <vector>
#include <Core/GPU/Mesh.h>
using namespace std;
using namespace glm;

class GameObject
{
public:
	float size = 40;
	float ship_x, ship_y;
	float mouse_x, mouse_y;
	float angle;
	int lives;
	mat3 modelMatrix;
	Mesh* mesh;

	vec3 corner = vec3(0, 0, 0);
	vec3 red = vec3(1, 0, 0);
	vec3 blue = vec3(0, 0, 1);
	vec3 yellow = vec3(1, 1, 0);

public:
	GameObject(){};
	virtual ~GameObject() {};

	Mesh* CreateShip(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = true)
	{
		glm::vec3 corner = leftBottomCorner;

		vector<VertexFormat> vertices =
		{
			VertexFormat(corner + glm::vec3(-length, length, 0), color),
			VertexFormat(corner + glm::vec3(-length, -length, 0), color),
			VertexFormat(corner + glm::vec3(0, 0, 0), color),

			VertexFormat(corner + glm::vec3(length, length, 0), color),
			VertexFormat(corner + glm::vec3(length, -length, 0), color),
			VertexFormat(corner + glm::vec3(0, 0, 0), color)
		};

		Mesh* ship = new Mesh(name);

		vector<unsigned short> indices = { 0, 1, 2, 3, 4, 5 };

		if (!fill) {
			ship->SetDrawMode(GL_LINE_LOOP);
		}
		else
		{
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(2);
			indices.push_back(3);
			indices.push_back(4);
			indices.push_back(5);
		}

		ship->InitFromData(vertices, indices);
		return ship;
	};
};