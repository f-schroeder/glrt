#pragma once

#include "../GLRT_Dependencies.hpp"
#include "../Utils/Window.hpp"

namespace glrt
{
	class Camera;
	using Camera_ptr = std::shared_ptr<Camera>;

	class Camera : public WindowEventHandler
	{
	public:

	private:
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

	};
}
