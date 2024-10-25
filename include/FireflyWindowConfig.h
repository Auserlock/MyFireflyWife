//
// Created by arch_ on 24-10-25.
//

#pragma once
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using std::string;

const string CONFIG_FILE_PATH = "data/config/main.json";

class ConfigFile {
public:
    ConfigFile();
    ~ConfigFile() { delete config_file; }
    void set(const string& key, const string& value);
    json& get() { return config; }

private:
    void write() const;
    json config;
    std::ifstream* config_file;
};
