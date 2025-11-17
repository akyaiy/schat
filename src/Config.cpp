#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

#include "Config.h"

Config::Config(const std::string &config_path, const std::string &format) {
	this->config_path = config_path;
	this->format = format;
	if (this->format == CONFIG_FORMAT_JSON) {
		this->data = this->ParseJSON();
	} else {
		throw std::runtime_error("Unsupported config format: " + this->format);
	}
}

std::ifstream* Config::OpenFile() {
	std::ifstream* file = new std::ifstream(this->config_path);
	if (!file->is_open()) {
		throw std::runtime_error("Failed to open config file: " + this->config_path);
	}
	return file;
}

nlohmann::json* Config::ParseJSON() {
	this->file = this->OpenFile();
	try {
		this->data = new nlohmann::json();
		*this->file >> *this->data;
		return data;
	} catch (const nlohmann::json::parse_error& e) {
		throw std::runtime_error("JSON parse error: " + std::string(e.what()));
	}
}

Config::~Config() {
	this->file->close();
	delete this->file;
	this->data->clear();
	delete this->data;
}


