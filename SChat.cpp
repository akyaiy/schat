#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

#include "config.h"

int main(int argc, char **argv) {
	std::ifstream file("config.json");
	if (!file.is_open()) {
        std::cerr << "Failed to open config.json" << std::endl;
        return 1;
    }
	try {
        nlohmann::json data;
        file >> data;
        std::cout << data.dump(4) << std::endl;
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
