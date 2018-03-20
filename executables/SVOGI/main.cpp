#include <Utils/Window.hpp>

int main() {
	glrt::Window &w = glrt::Window::initInstance("SVOGI");

	w.loop([](float deltaTime) {});

	return 0;
}