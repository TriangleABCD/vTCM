#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <map>

#include "usb.h"

std::map<std::string, std::string> configMap;

int initConfig(std::string config_path) {
    std::ifstream configFile(config_path);
    std::string line;
    while(getline(configFile, line)) {
        size_t pos = line.find("=");
        if(pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            key.erase(0, key.find_first_not_of(" "));
            key.erase(key.find_last_not_of(" ") + 1);
            value.erase(0, value.find_first_not_of(" "));
            value.erase(value.find_last_not_of(" ") + 1);
            configMap[key] = value;
        } else {
            continue;
        }
    }
    configFile.close();
    return 0;
}

std::vector<std::string> readDirectory(const std::string& directoryPath) {
    std::vector<std::string> result;
    for(const auto& file : std::filesystem::directory_iterator(directoryPath)) {
        if(file.is_directory()) {
            auto subFiles = readDirectory(file.path().string());
            for(const auto& subFile : subFiles) {
                result.push_back(file.path().filename().string() + "/\n" + subFile);
            }
        } else if(file.is_regular_file()) {
            std::ifstream ifs(file.path(), std::ios::binary);
            if(ifs.good()) {
                std::string content(std::istreambuf_iterator<char>(ifs), {});
                content = file.path().filename().string() + ":\n" + content;
                result.push_back(content);
            }
        }
    }
    return result;
}

std::string sm3(std::string s) {
    return "42";
}

std::string getSM3Hash() {
    auto secure = readDirectory(configMap["secure_container_path"]);
    auto normal = readDirectory(configMap["normal_container_path"]);

    std::string secure_str = "";
    std::string normal_str = "";
    for(auto& s: secure) {
        secure_str += s;
    }
    for(auto& s: normal) {
        normal_str += s;
    }

    std::string str = secure_str + normal_str;
    std::string hash = sm3(str);
    return hash;
}

int main(int argc, char** argv) {
    std::string path{"config.txt"};
    initConfig(path);
    std::string ans_hash = "114";
    std::fstream hashFile(configMap["hash_path"]);
    ans_hash = getSM3Hash();

    hashFile << ans_hash;

    USBChecker usbc;
    usbc.initUSB(configMap);
    auto res = usbc.checkUSB(); 
    std::cout << res << std::endl;
    return 0;
}
