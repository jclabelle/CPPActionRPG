#include <fstream>
#include <string>
#include <memory>
#include <iostream>
#include <filesystem>
#include "PAS_JsonUtilities.h"

/* Returns a nlohmann::json object wrapped in a unique_ptr */
std::unique_ptr<nlohmann::json> loadJsonDataFromFile(std::string DataFilePath)
{
	// I/O
	std::ifstream ifs(DataFilePath);
	if(!ifs.good())std::cout << "FAIL READING FILE: " << DataFilePath << " in loadJsonDataFromFile" << '\n';

	// Make the json
	nlohmann::json json;
	if(ifs >> json)
	{
	ifs.close();
	}
	else
	{
		std::cout << "ERROR READING IFSTREAM INTO JSON, FILE: " << DataFilePath << '\n';
	}

	std::unique_ptr<nlohmann::json> jsonMoving = std::make_unique<nlohmann::json>(json);
	return jsonMoving;
}

std::unique_ptr<nlohmann::json> loadJsonDataFromFile(std::filesystem::path DataFilePath)
{
	// I/O
	std::ifstream ifs(DataFilePath.string());
	if (!ifs.good())std::cout<<"\nFAIL READING FILE: " << DataFilePath << " in loadJsonDataFromFile" << '\n';

	// Make the json
	nlohmann::json json;
	if (ifs >> json)
	{
		ifs.close();
	}
	else
	{
		std::cout << "ERROR READING IFSTREAM INTO JSON, FILE: " << DataFilePath << '\n';
	}

	std::unique_ptr<nlohmann::json> jsonMoving = std::make_unique<nlohmann::json>(json);
	return jsonMoving;
}

nlohmann::json loadJson(std::filesystem::path::string_type DataFilePath)
{
	// I/O
	std::ifstream ifs(DataFilePath);
	if (!ifs.good())std::wcout << "\nFAIL READING FILE: " << DataFilePath << " in loadJsonDataFromFile" << '\n';

	// Make the json
	nlohmann::json json;
	if (ifs >> json)
	{
		ifs.close();
	}
	else
	{
		std::wcout << "ERROR READING IFSTREAM INTO JSON, FILE: " << DataFilePath << '\n';
	}

	return json;
}

nlohmann::json loadJson(std::filesystem::path DataFilePath)
{
	// I/O
	std::ifstream ifs(DataFilePath);
	if (!ifs.good())std::wcout << "\nFAIL READING FILE: " << DataFilePath << " in loadJsonDataFromFile" << '\n';

	// Make the json
	nlohmann::json json;
	if (ifs >> json)
	{
		ifs.close();
	}
	else
	{
		std::wcout << "ERROR READING IFSTREAM INTO JSON, FILE: " << DataFilePath << '\n';
	}

	return json;
}

nlohmann::json loadJson(std::string DataFilePath)
{
	// I/O
	std::ifstream ifs(DataFilePath);
	if (!ifs.good())std::cout << "\nFAIL READING FILE: " << DataFilePath << " in loadJsonDataFromFile" << '\n';

	// Make the json
	nlohmann::json json;
	if (ifs >> json)
	{
		ifs.close();
	}
	else
	{
		std::cout << "ERROR READING IFSTREAM INTO JSON, FILE: " << DataFilePath << '\n';
	}

	return json;
}


 