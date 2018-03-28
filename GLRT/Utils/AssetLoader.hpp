#pragma once
#include "../GLRT_Dependencies.hpp"

#include <filesystem>
#include <ppl.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace glrt {

	namespace fs = std::experimental::filesystem;
	class AssetLoader
	{
	public:

		static std::string find(std::string filename, std::vector<fs::path> searchPaths = {
			fs::current_path(),
			fs::current_path().parent_path().parent_path().parent_path() /= "glrt"
		})
		{
			for (auto& dir : searchPaths)
				if(exists(dir) && !is_empty(dir))
					for (auto& file : fs::recursive_directory_iterator(dir))
						if (!is_empty(dir) && !fs::is_directory(file) && 
							file.path().filename().string().find(filename) != std::string::npos)
							return fs::absolute(file.path()).string();					
			throw std::runtime_error("AssetLoader could not find a file with the name " + filename);
		}

		class ImageData
		{
		public:
			int width, height, numChannels;
			std::vector<glm::vec4> data;

			glm::vec4 operator()(int x, int y) { return data[y * width + x]; }

			ImageData(std::string filename)
			{
				load(filename);
			}

			void load(std::string filename)
			{
				stbi_set_flip_vertically_on_load(true);
				unsigned char* raw_data = stbi_load(filename.c_str(), &width, &height, &numChannels, 4);
				data.resize(width * height, glm::vec4(0.f));

				concurrency::parallel_for(size_t(0), size_t(width * height), [&](size_t i) {
					size_t index = i * 4;
					data[i] = glm::vec4(raw_data[index], raw_data[index+1], raw_data[index+2], raw_data[index+3]) / 255.f;
				});
				stbi_image_free(raw_data);
			}
		};

	protected:
		AssetLoader() {} //only static access
	};
}
