#include <iostream>
#include <fstream>
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

int main(int argc, char** argv) {
    std::string path{argv[1]};
    initConfig(path);
    USBChecker usbc;
    usbc.initUSB(configMap);
    auto res = usbc.checkUSB(); 
    std::cout << res << std::endl;
    return 0;
}
