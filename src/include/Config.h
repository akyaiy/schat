#pragma once

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

#define CONFIG_FORMAT_JSON "json"

class Config {
	std::string config_path;
	std::string format;
	std::ifstream* file;
	nlohmann::json* data;
	std::ifstream* OpenFile();
	nlohmann::json* ParseJSON();
public:
	Config(const std::string& config_path, const std::string& format);
	~Config();

	nlohmann::json* GetData() { return data; }
};