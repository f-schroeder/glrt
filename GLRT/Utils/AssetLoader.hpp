#pragma once
#include <filesystem>
#include <vector>

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

	private:
		AssetLoader(); //only static access
	};
}
