#pragma once
#include "../GLRT_Dependencies.hpp"

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

		Geometry(int activeBuffers)
		{
			if (activeBuffers & geo_Buffer_Index)	indexBuffer = Buffer_ptr(new globjects::Buffer, DelFunc);
			if (activeBuffers & geo_Buffer_Vertex)	vertexBuffer = Buffer_ptr(new globjects::Buffer, DelFunc);
			if (activeBuffers & geo_Buffer_Normals) normalsBuffer = Buffer_ptr(new globjects::Buffer, DelFunc);
			if (activeBuffers & geo_Buffer_UV)		uvBuffer = Buffer_ptr(new globjects::Buffer, DelFunc);
		}

		void uploadToGPU() const
		{
			if (indexBuffer)	indexBuffer->setStorage<gl::GLuint>(indices, gl::BufferStorageMask::GL_NONE_BIT);
			if (vertexBuffer)	vertexBuffer->setStorage<glm::vec4>(vertices, gl::BufferStorageMask::GL_NONE_BIT);
			if (normalsBuffer)	normalsBuffer->setStorage<glm::vec4>(normals, gl::BufferStorageMask::GL_NONE_BIT);
			if (uvBuffer)		uvBuffer->setStorage<glm::vec2>(uvs, gl::BufferStorageMask::GL_NONE_BIT);
		}		
	};
}
