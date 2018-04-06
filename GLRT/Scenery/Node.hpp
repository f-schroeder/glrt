#pragma once

#include "../GLRT_Dependencies.hpp"
#include "Geometry.hpp"
#include "Material.hpp"

namespace glrt
{
	class Node;
	using Node_ptr = std::shared_ptr<Node>;

	class Node
	{
	public:
		Geometry_ptr geometry;
		Material_ptr material;
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		void draw() const
		{
			geometry->draw();
		}
	};
}
