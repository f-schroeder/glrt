#include "GLRT/Utils/Window.hpp"
#include "GLRT/Utils/AssetLoader.hpp"
#include "GLRT/Rendering/ScreenFillingQuad.hpp"
#include "GLRT/Scenery/Scene.hpp"

using namespace gl;
using namespace globjects;

int main() {
	auto w = glrt::Window::makeWindow("SVOGI");

	//auto q = std::make_shared<glrt::ScreenFillingQuad>();
	auto program = Program_ptr(new Program(), DelFunc);
	program->attach(
		Shader::fromFile(gl::GL_VERTEX_SHADER, glrt::AssetLoader::find("gBuffer.vert")),
		Shader::fromFile(gl::GL_FRAGMENT_SHADER, glrt::AssetLoader::find("gBuffer.frag"))
	);

	auto img = glrt::AssetLoader::ImageData(glrt::AssetLoader::find("cv_logo.bmp"));

	auto t = std::shared_ptr<Texture>(Texture::createDefault(), DelFunc);
	t->image2D(0, GL_RGBA32F, glm::ivec2(img.width,img.height), 0, GL_RGBA, GL_FLOAT, img.data.data());
	//q->setTexture(t);

	auto s = glrt::Scene::fromFile(glrt::AssetLoader::find("bunny.fbx"));

	w->loop([&](float deltaTime)
	{
		//q->draw();
		program->use();
		s->draw();
	});

	return 0;
}
