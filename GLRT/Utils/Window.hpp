#pragma once

#include <iostream>
#include <globjects/globjects.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace glrt {
	class Window
	{
	public:

		static Window& initInstance(const std::string & title = "OpenGL Window", glm::ivec2 size = glm::vec2(800, 600));
		static Window& getInstance();

		void loop(const std::function<void(float)>& loop);

		void setSize(glm::ivec2 newSize);
		glm::vec2 getSize();

		virtual ~Window();

		//non-copyable
		Window(Window const&) = delete;
		void operator=(Window const&) = delete;

	private:
		Window(const std::string & title, glm::ivec2 size);

		static void resize_callback(GLFWwindow * window, int width, int height);
		static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);

		static void default_error_callback(int errnum, const char * errmsg);
		static void default_resize_callback(GLFWwindow * /*window*/, int width, int height);
		static void default_key_callback(GLFWwindow * /*window*/, int key, int /*scancode*/, int action, int /*mods*/);

		std::vector<std::function<void(GLFWwindow *, int, int, int, int)>> keyCallbacks;
		std::vector<std::function<void(GLFWwindow *, int, int)>> resizeCallbacks;

		glm::ivec2 size;
		GLFWwindow* glfwWindow;
	};
}
