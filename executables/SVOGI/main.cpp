#include "GLRT/Utils/Window.hpp"
#include "GLRT/Utils/AssetLoader.hpp"
#include "GLRT/Rendering/ScreenFillingQuad.hpp"

using namespace gl;
using namespace globjects;

int main() {
	auto w = glrt::Window::makeWindow("SVOGI");

	auto q = std::make_shared<glrt::ScreenFillingQuad>();
	w->addEventHandler(q);

	auto img = glrt::AssetLoader::ImageData(glrt::AssetLoader::find("cv_logo.bmp"));

	auto t = std::shared_ptr<Texture>(Texture::createDefault(), DelFunc);
	t->image2D(0, GL_RGBA32F, glm::ivec2(img.width,img.height), 0, GL_RGBA, GL_FLOAT, img.data.data());
	q->setTexture(t);

	w->loop([q](float deltaTime)
	{
		q->draw();
	});

	return 0;
}
