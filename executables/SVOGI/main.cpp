#include <GLRT/GLRT.hpp>

int main() {
	auto w = glrt::Window::makeWindow("SVOGI");

	auto p = glrt::AssetLoader::find("gBuffer.frag");
	std::cout << p << '\n';

	w->loop([](float deltaTime) {});

	return 0;
}
