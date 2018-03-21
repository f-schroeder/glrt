#include <GLRT/Utils/Window.hpp>

int main() {
	auto w = glrt::Window::makeWindow("SVOGI");

	w->loop([](float deltaTime) {});

	return 0;
}
