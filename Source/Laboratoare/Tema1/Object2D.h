#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

// TEACA BOGDAN

namespace Object2D
{
	// Create rectangle with given bottom left corner, width, height and color
	Mesh* CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill = false);
	Mesh* CreateCircle(std::string name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color, bool fill);
}

