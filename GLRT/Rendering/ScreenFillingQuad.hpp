#pragma once

#include "../GLRT_Dependencies.hpp"
#include "../Utils/AssetLoader.hpp"
#include <globjects/VertexAttributeBinding.h>
#include <globjects/TextureHandle.h>
#include <globjects/Texture.h>

using namespace globjects;

namespace glrt {

	class ScreenFillingQuad
	{
	public:
		ScreenFillingQuad()
		{
			auto program = Program_ptr(new Program(), DelFunc);
			program->attach(
				Shader::fromFile(gl::GL_VERTEX_SHADER, AssetLoader::find("screenFilling.vert")),
				Shader::fromFile(gl::GL_FRAGMENT_SHADER, AssetLoader::find("simpleTexture.frag"))
			);

			init(program);
		}

		ScreenFillingQuad(Shader_ptr fragmentShader)
		{
			if(fragmentShader->type() != gl::GL_FRAGMENT_SHADER)
			{
				critical() << "Initializing ScreenFillingQuad object without fragment shader.";
				std::cin.ignore();
				throw std::runtime_error("Initializing ScreenFillingQuad object without fragment shader.");
			}

			auto program = Program_ptr(new Program(), DelFunc);
			program->attach(
				Shader::fromFile(gl::GL_VERTEX_SHADER, AssetLoader::find("screenFilling.vert")),
				fragmentShader.get()
			);

			init(program);
		}

		ScreenFillingQuad(Program_ptr shaderProgram)
		{
			init(shaderProgram);
		}

		void draw() const
		{
			shaderProgram->use();
			if (texture) {
				shaderProgram->setUniform("sfq_texture", texture->textureHandle().handle());
			}
			vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
			shaderProgram->release();
		}

		void setTexture(Texture_ptr texture)
		{
			this->texture = texture;
			auto handle = texture->textureHandle();
			if (!handle.isResident())
				handle.makeResident();
		}

	private:

		void init(Program_ptr shaderProgram)
		{
			this->shaderProgram = shaderProgram;

			static const std::array<glm::vec2, 4> raw{ { glm::vec2(+1.f,-1.f), glm::vec2(+1.f,+1.f), glm::vec2(-1.f,-1.f), glm::vec2(-1.f,+1.f) } };

			vao = VertexArray_ptr(new VertexArray(), DelFunc);

			auto buffer = new Buffer();
			buffer->setData(raw, gl::GL_STATIC_DRAW); //needed for some drivers

			auto binding = vao->binding(0);
			binding->setAttribute(0);
			binding->setBuffer(buffer, 0, sizeof(glm::vec2));
			binding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
			vao->enable(0);
		}

		Program_ptr shaderProgram;
		VertexArray_ptr vao;
		Texture_ptr texture;
	};
}