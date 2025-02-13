#pragma once
#include <string>

// Gets you KeyA name from KeyA filename - eliminates the file suffix
std::string getNameFromFileName(std::string filename);

// Find directory from full path to KeyA file. ex: c:/files/file.txt returns c:/files/
std::string getDirFromFilePath(std::string filepath);