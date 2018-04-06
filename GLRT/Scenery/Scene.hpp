#pragma once

#include "../GLRT_Dependencies.hpp"
#include "Node.hpp"
#include "Camera.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <ppl.h>

namespace glrt
{
	class Scene;
	using Scene_ptr = std::shared_ptr<Scene>;

	class Scene
	{
	public:
		Camera_ptr camera;
		std::vector<Node_ptr> nodes;

		static Scene_ptr fromFile(std::string filename)
		{
			Scene_ptr resultScene = std::make_shared<Scene>();

			Assimp::Importer importer;
			unsigned int assimp_settings = (
				//aiProcess_CalcTangentSpace |
				aiProcess_GenSmoothNormals |
				aiProcess_JoinIdenticalVertices |
				//aiProcess_ImproveCacheLocality |
				//aiProcess_LimitBoneWeights |
				//aiProcess_RemoveRedundantMaterials |
				//aiProcess_SplitLargeMeshes |
				aiProcess_Triangulate |
				aiProcess_GenUVCoords |
				//aiProcess_SortByPType |
				//aiProcess_FindDegenerates |
				//aiProcess_FindInvalidData |
				0);
			importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0f);

			auto scene = importer.ReadFile(filename, assimp_settings);
			if (!scene)
			{
				globjects::critical() << "Scene loading failed: " << filename << '\n'
				<< "Reason: " << importer.GetErrorString() << '\n';
				std::cin.ignore();
				throw std::runtime_error("Scene loading failed.");
			}

			for (size_t i = 0; i < scene->mNumMeshes; ++i)
			{
				auto mesh = scene->mMeshes[i];

				int activeBuffers = 
					Geometry::geo_Buffer_None
					| (int(mesh->HasPositions()) ? Geometry::geo_Buffer_Index : 0)
					| (int(mesh->HasPositions()) ? Geometry::geo_Buffer_Vertex : 0)
					| (int(mesh->HasNormals()) ? Geometry::geo_Buffer_Normals : 0)
					| (int(mesh->HasTextureCoords(0)) ? Geometry::geo_Buffer_UV : 0);

				auto geo = std::make_shared<Geometry>(activeBuffers);

				if (mesh->HasPositions()) geo->vertices.resize(mesh->mNumVertices);
				if (mesh->HasNormals()) geo->normals.resize(mesh->mNumVertices);
				if (mesh->HasTextureCoords(0)) geo->uvs.resize(mesh->mNumVertices);

				concurrency::parallel_for(size_t(0), size_t(mesh->mNumVertices), [&](size_t v) {
						if (mesh->HasPositions()) geo->vertices[v] = glm::vec4(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z, 1.0f);
						if (mesh->HasNormals()) geo->normals[v] = glm::vec4(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z, 0.0f);
						if (mesh->HasTextureCoords(0)) geo->uvs[v] = glm::vec2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				});

				/*#pragma omp parallel for
				for (int v = 0; v < mesh->mNumVertices; v++)
				{
					if (mesh->HasPositions()) geo->vertices[v] = glm::vec4(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z, 1.0f);
					if (mesh->HasNormals()) geo->normals[v] = glm::vec4(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z, 0.0f);
					if (mesh->HasTextureCoords(0)) geo->uvs[v] = glm::vec2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				}*/

				if (mesh->HasPositions()) {
					geo->indices.resize(mesh->mNumFaces * 3);

					concurrency::parallel_for(size_t(0), size_t(mesh->mNumFaces), [&](size_t f) {
						geo->indices[f * 3 + 0] = mesh->mFaces[f].mIndices[0];
						geo->indices[f * 3 + 1] = mesh->mFaces[f].mIndices[1];
						geo->indices[f * 3 + 2] = mesh->mFaces[f].mIndices[2];
					});

					/*#pragma omp parallel for
					for (int f = 0; f < mesh->mNumFaces; ++f) {
						geo->indices[f * 3 + 0] = mesh->mFaces[f].mIndices[0];
						geo->indices[f * 3 + 1] = mesh->mFaces[f].mIndices[1];
						geo->indices[f * 3 + 2] = mesh->mFaces[f].mIndices[2];
					}*/
				}

				geo->uploadToGPU();

				Node_ptr node = std::make_shared<Node>();
				node->geometry = geo;
				resultScene->addNode(node);

				//TODO: Material
			}
			return resultScene;
		}

		Scene(){}

		void addNode(Node_ptr node)
		{
			nodes.push_back(node);
		}

		void draw()
		{
			std::for_each(nodes.begin(), nodes.end(), [](Node_ptr& node) { node->draw(); });
		}
	};
}
