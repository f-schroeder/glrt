#pragma once

#include "../GLRT_Dependencies.hpp"
#include <unordered_set>
#include <globjects/base/File.h>

namespace glrt {

	class WindowEventHandler
	{
	public:
		virtual void resize_callback(int width, int height){}
		virtual void key_callback(int key, int scancode, int action, int mods){}
		virtual void char_callback(unsigned int codepoint) {}
		virtual void cursor_position_callback(double xpos, double ypos) {}
		virtual void mouse_button_callback(int button, int action, int mods) {}
		virtual void scroll_callback(double xoffset, double yoffset) {}
		virtual ~WindowEventHandler() = default;
	};
	using WindowEventHandler_ptr = std::shared_ptr<WindowEventHandler>;

	class Window;
	using Window_ptr = std::shared_ptr<Window>;

	class Window : public WindowEventHandler, public std::enable_shared_from_this<Window>
	{

	public:

		static Window_ptr makeWindow(const std::string & title = "OpenGL Window", glm::ivec2 size = glm::vec2(800, 600))
		{
			Window_ptr w(new Window(title, size));
			w->addEventHandler(w);
			return w;
		}

		void loop(const std::function<void(float)> & loop) const
		{
			auto lastTime = float(glfwGetTime());
			while (!glfwWindowShouldClose(glfwWindow))
			{
				auto currentTime = float(glfwGetTime());

				glfwPollEvents();
				loop(currentTime - lastTime);
				glfwSwapBuffers(glfwWindow);

				lastTime = currentTime;
			}
			glfwTerminate();
		}

		void addEventHandler(WindowEventHandler_ptr handler) { eventHandlers.push_back(handler); }

		glm::ivec2 getSize() const { return size; }
		void setSize(glm::ivec2 size) { this->size = size; gl::glViewport(0, 0, size.x, size.y); }

		virtual ~Window() 
		{
			glfwDestroyWindow(glfwWindow);
			glfwTerminate();
		}

	private:

		Window(const std::string & title = "OpenGL Window", glm::ivec2 size = glm::vec2(800, 600)) : size(size)
		{
			// Initialize GLFW
			if (!glfwInit()) {
				globjects::critical() << "GLFW init failed. Terminate execution.";
				std::cin.ignore();
				throw std::runtime_error("GLFW init failed. Terminate execution.");
			}

			glfwSetErrorCallback(glfw_error_callback);
			glfwDefaultWindowHints();
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			// Create a context and, if valid, make it current
			glfwWindow = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
			if (!glfwWindow)
			{
				globjects::critical() << "Context creation failed. Terminate execution.";

				glfwTerminate();
				std::cin.ignore();
				throw std::runtime_error("Context creation failed. Terminate execution.");
			}

			// Initialize callbacks
			glfwSetWindowUserPointer(glfwWindow, static_cast<void *>(&eventHandlers));
			glfwSetKeyCallback(glfwWindow, glfw_key_callback);
			glfwSetWindowSizeCallback(glfwWindow, glfw_resize_callback);
			glfwSetCharCallback(glfwWindow, glfw_char_callback);
			glfwSetCursorPosCallback(glfwWindow, glfw_cursor_position_callback);
			glfwSetMouseButtonCallback(glfwWindow, glfw_mouse_button_callback);
			glfwSetScrollCallback(glfwWindow, glfw_scroll_callback);

			glfwMakeContextCurrent(glfwWindow);

			// Initialize globjects (internally initializes glbinding, and registers the current context)
			globjects::init();
			setSize(size);

			std::cout << '\n'
				<< "OpenGL Version:  " << glbinding::ContextInfo::version() << '\n'
				<< "OpenGL Vendor:   " << glbinding::ContextInfo::vendor() << '\n'
				<< "OpenGL Renderer: " << glbinding::ContextInfo::renderer() << '\n'
				<< "globjects Version: " << GLOBJECTS_NAME_VERSION << '\n' << '\n';

			globjects::DebugMessage::enable();
		}

		void resize_callback(int width, int height) override { setSize(glm::ivec2(width, height)); }
		void key_callback(int key, int /*scancode*/, int action, int /*mods*/) override
		{
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(glfwWindow, 1);
			if (key == GLFW_KEY_F5 && action == GLFW_RELEASE)
				globjects::File::reloadAll();
		}

		std::vector<WindowEventHandler_ptr> eventHandlers;

		glm::ivec2 size;

		GLFWwindow* glfwWindow;


		//Static GLFW callback functions
		static void glfw_resize_callback(GLFWwindow * window, int width, int height)
		{
			auto handlers = *static_cast<std::vector<WindowEventHandler_ptr> *>(glfwGetWindowUserPointer(window));
			for (auto& h : handlers) h->resize_callback(width, height);
		}
		static void glfw_key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
		{
			auto handlers = *static_cast<std::vector<WindowEventHandler_ptr> *>(glfwGetWindowUserPointer(window));
			for (auto& h : handlers) h->key_callback(key, scancode, action, mods);
		}
		static void glfw_char_callback(GLFWwindow * window, unsigned int codepoint)
		{
			auto handlers = *static_cast<std::vector<WindowEventHandler_ptr> *>(glfwGetWindowUserPointer(window));
			for (auto& h : handlers) h->char_callback(codepoint);
		}
		static void glfw_cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
		{
			auto handlers = *static_cast<std::vector<WindowEventHandler_ptr> *>(glfwGetWindowUserPointer(window));
			for (auto& h : handlers) h->cursor_position_callback(xpos, ypos);
		}
		static void glfw_mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
		{
			auto handlers = *static_cast<std::vector<WindowEventHandler_ptr> *>(glfwGetWindowUserPointer(window));
			for (auto& h : handlers) h->mouse_button_callback(button, action, mods);
		}
		static void glfw_scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
		{
			auto handlers = *static_cast<std::vector<WindowEventHandler_ptr> *>(glfwGetWindowUserPointer(window));
			for (auto& h : handlers) h->scroll_callback(xoffset, yoffset);
		}
		static void glfw_error_callback(int errnum, const char * errmsg) { globjects::critical() << errnum << ": " << errmsg << '\n'; }

	};

}
