//
// Created by arch_ on 24-10-25.
//
#include "FireflyWindowConfig.h"
#include <iostream>
#include <ostream>
#include "Log.h"

ConfigFile::ConfigFile()
    : config_file { new std::ifstream { CONFIG_FILE_PATH } }
{
    if (!config_file->is_open()) {
        logger->error("无法打开配置文件: " + std::string(CONFIG_FILE_PATH));
        throw std::runtime_error("无法打开配置文件: " + std::string(CONFIG_FILE_PATH));
    }

    if (!(*config_file >> config)) {
        logger->error("读取配置失败");
        throw std::runtime_error("读取配置失败");
    }
}

void ConfigFile::write() const
{
    std::ofstream writeFile(CONFIG_FILE_PATH);
    if (!writeFile.is_open()) {
        logger->error("无法打开配置文件: " + CONFIG_FILE_PATH);
        throw std::runtime_error("无法打开配置文件: " + CONFIG_FILE_PATH);
    }

    writeFile << config.dump(4, ' ', true);
}

void ConfigFile::set(const string& key, const string& value)
{
    const auto data = config.find(key);
    if (data == config.end()) {
        logger->error("未找到关键字: " + key);
        throw std::runtime_error("未找到关键字: " + key);
    }
    config[key] = value; // 更新值
    try {
        write();
    } catch (std::runtime_error& e) {
        logger->error(e.what());
    }
}
