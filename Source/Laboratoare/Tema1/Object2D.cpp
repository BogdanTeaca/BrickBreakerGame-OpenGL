#include "Object2D.h"

#include <Core/Engine.h>

// TEACA BOGDAN

Mesh * Object2D::CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill) {
	// functie care creaza o mesha pentru un dreptunghi de latime width, inaltime height si culoare color
	glm::vec3 corner = leftBottomCorner;

	// creez cele 4 vertex-uri la coordonatele (0, 0), (width, 0), (width, height), (0, height).
	std::vector<VertexFormat> vertices = {
		VertexFormat(corner,                               color),
		VertexFormat(corner + glm::vec3(width, 0,      0), color),
		VertexFormat(corner + glm::vec3(width, height, 0), color),
		VertexFormat(corner + glm::vec3(0,     height, 0), color)
	};

	Mesh * rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		rectangle->SetDrawMode(GL_LINE_LOOP);
	} else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	// initializez mesha folosind vertex-urile si indicii
	rectangle->InitFromData(vertices, indices);

	return rectangle;
}

Mesh * Object2D::CreateCircle(std::string name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color, bool fill) {
	// functie care creaza o mesha pentru un cerc de raza radius si culoare color
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices;

	int circle_vertices = 16;

	// creez 16 puncte (si vertex-uri) care sa aproximeze conturul unui cerc
	for (int i = 0; i < circle_vertices; i++) {
		double angle = 2 * 3.1415f * i / circle_vertices;
		double x = radius + radius * glm::cos(angle);
		double y = radius + radius * glm::sin(angle);

		vertices.push_back(VertexFormat(corner + glm::vec3(x, y, 0), color));
	}

	Mesh * circle = new Mesh(name);
	std::vector<unsigned short> indices;;

	if (!fill) {
		// daca parametrul fill este false atunci creez doar
		// conturul cercului folosind setarea "GL_LINE_LOOP"
		indices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
		circle->SetDrawMode(GL_LINE_LOOP);
	}else {
		// daca parametrul fill este true atunci umplu si
		// interiorul cercului folosind setarea "GL_TRIANGLE_FAN"
		indices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
		circle->SetDrawMode(GL_TRIANGLE_FAN);
	}

	// initializez mesha folosind vertex-urile si indicii
	circle->InitFromData(vertices, indices);

	return circle;
}