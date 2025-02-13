#pragma once
#include "nlohmann/json.hpp"

std::unique_ptr<nlohmann::json> loadJsonDataFromFile(std::string DataFilePath);

std::unique_ptr<nlohmann::json> loadJsonDataFromFile(std::filesystem::path DataFilePath);

nlohmann::json loadJson(std::filesystem::path::string_type DataFilePath);

nlohmann::json loadJson(std::filesystem::path DataFilePath);

nlohmann::json loadJson(std::string DataFilePath);