#pragma once
#include "../GLRT_Dependencies.hpp"
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>

namespace glrt
{
	class Geometry;
	using Geometry_ptr = std::shared_ptr<Geometry>;

	class Geometry
	{
	public:
		static const int geo_Buffer_None	= 0x00;
		static const int geo_Buffer_Index	= 0x01;
		static const int geo_Buffer_Vertex	= 0x02;
		static const int geo_Buffer_Normals = 0x04;
		static const int geo_Buffer_UV		= 0x08;

		std::vector<uint32_t> indices;
		std::vector<glm::vec4> vertices;
		std::vector<glm::vec4> normals;
		std::vector<glm::vec2> uvs;

		Buffer_ptr indexBuffer;
		Buffer_ptr vertexBuffer;
		Buffer_ptr normalsBuffer;
		Buffer_ptr uvBuffer;

		VertexArray_ptr vao;

		Geometry(int activeBuffers)
		{
			vao = VertexArray_ptr(new VertexArray(), DelFunc);

			if (activeBuffers & geo_Buffer_Vertex) {
				vertexBuffer = Buffer_ptr(new globjects::Buffer, DelFunc);
				auto binding = vao->binding(0);
				binding->setAttribute(0);
				binding->setBuffer(vertexBuffer.get(), 0, sizeof(glm::vec4));
				binding->setFormat(4, gl::GL_FLOAT, gl::GL_FALSE, 0);
				vao->enable(0);
			}
			if (activeBuffers & geo_Buffer_Normals) {
				normalsBuffer = Buffer_ptr(new globjects::Buffer, DelFunc);
				auto binding = vao->binding(1);
				binding->setAttribute(1);
				binding->setBuffer(normalsBuffer.get(), 0, sizeof(glm::vec4));
				binding->setFormat(4, gl::GL_FLOAT, gl::GL_TRUE, 0);
				vao->enable(1);
			}
			if (activeBuffers & geo_Buffer_UV) {
				uvBuffer = Buffer_ptr(new globjects::Buffer, DelFunc);
				auto binding = vao->binding(2);
				binding->setAttribute(2);
				binding->setBuffer(uvBuffer.get(), 0, sizeof(glm::vec2));
				binding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
				vao->enable(2);
			}
			if (activeBuffers & geo_Buffer_Index) {
				indexBuffer = Buffer_ptr(new globjects::Buffer, DelFunc);
			}
		}

		void uploadToGPU() const
		{
			if (indexBuffer) {
				indexBuffer->setStorage<gl::GLuint>(indices, gl::BufferStorageMask::GL_NONE_BIT);
				//gl::glVertexArrayElementBuffer(vao->id(), indexBuffer->id());
			}
			if (vertexBuffer)	vertexBuffer->setStorage<glm::vec4>(vertices, gl::BufferStorageMask::GL_NONE_BIT);
			if (normalsBuffer)	normalsBuffer->setStorage<glm::vec4>(normals, gl::BufferStorageMask::GL_NONE_BIT);
			if (uvBuffer)		uvBuffer->setStorage<glm::vec2>(uvs, gl::BufferStorageMask::GL_NONE_BIT);
		}		

		void draw()
		{
			if (indexBuffer) {
				vao->drawElements(gl::GL_TRIANGLES, indices.size(), gl::GL_UNSIGNED_INT, indices.data());
			} else {
				vao->drawArrays(gl::GL_TRIANGLES, 0, vertices.size());
			}
		}
	};
}
