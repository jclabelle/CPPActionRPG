#include "TextManipulationFunctions.h"

#include <iostream>
#include <string>
#include <string_view>
#include <cstdint>

std::string getNameFromFileName(std::string filename)
{
	std::string return_string;

	// Get rid of everything but the suffix-less filename
	for (auto c : filename)
	{
		if (c == '.')
			return return_string;

		return_string += c;

		if (c == '/')
			return_string.erase();
	}
	filename = "ERROR";
	return filename;
}

std::string getDirFromFilePath(std::string filepath)
{
	std::basic_string_view path_view(filepath.c_str());
	auto filename_position = path_view.find_last_of('/');
	auto excess = path_view.size() - filename_position - 1;
	path_view.remove_suffix(excess);
	std::string return_string;
	return_string = path_view;
	return return_string;
}