#include "Window.hpp"

using namespace glrt;

Window::Window(const std::string & title, glm::ivec2 size) : size(size)
{
	// Initialize GLFW
	if (!glfwInit()) {
		globjects::critical() << "GLFW init failed. Terminate execution.";
		std::cin.ignore();
		exit(0);
	}

	glfwSetErrorCallback(default_error_callback);
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
		exit(0);
	}

	// Initialize callbacks
	keyCallbacks.push_back(default_key_callback);
	glfwSetKeyCallback(glfwWindow, key_callback);
	resizeCallbacks.push_back(default_resize_callback);
	glfwSetWindowSizeCallback(glfwWindow, resize_callback);

	glfwMakeContextCurrent(glfwWindow);

	// Initialize globjects (internally initializes glbinding, and registers the current context)
	globjects::init();

	std::cout << '\n'
		<< "OpenGL Version:  " << glbinding::ContextInfo::version() << '\n'
		<< "OpenGL Vendor:   " << glbinding::ContextInfo::vendor() << '\n'
		<< "OpenGL Renderer: " << glbinding::ContextInfo::renderer() << '\n' << '\n';

	globjects::DebugMessage::enable();
}

void Window::loop(const std::function<void(float)> & loop)
{
	float lastTime = (float)glfwGetTime();
	while (!glfwWindowShouldClose(glfwWindow))
	{
		float currentTime = (float)glfwGetTime();

		glfwPollEvents();
		loop(currentTime - lastTime);
		glfwSwapBuffers(glfwWindow);

		lastTime = currentTime;
	}
	glfwTerminate();
}

void glrt::Window::resize_callback(GLFWwindow * window, int width, int height)
{
	for (auto var : Window::getInstance().resizeCallbacks)
		var(window, width, height);
}

void glrt::Window::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	for (auto var : Window::getInstance().keyCallbacks)
		var(window, key, scancode, action, mods);
}

Window& glrt::Window::initInstance(const std::string & title, glm::ivec2 size)
{
	static Window instance(title, size);
	return instance;
}

Window& Window::getInstance()
{
	return initInstance();
}

void glrt::Window::setSize(glm::ivec2 newSize)
{
	size = newSize;
	gl::glViewport(0, 0, size.x, size.y);
}

glm::vec2 glrt::Window::getSize()
{
	return size;
}

Window::~Window()
{
	glfwDestroyWindow(glfwWindow);
	glfwTerminate();
}

void Window::default_error_callback(int errnum, const char * errmsg)
{
	globjects::critical() << errnum << ": " << errmsg << '\n';
}

void Window::default_resize_callback(GLFWwindow * /*window*/, int width, int height)
{
	Window::getInstance().setSize(glm::ivec2(width, height));
}

void Window::default_key_callback(GLFWwindow * window, int key, int /*scancode*/, int action, int /*mods*/)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}