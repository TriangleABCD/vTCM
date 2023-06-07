#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <map>

#include "usb.h"
#include "sm3.h"
#include "sm4.h"

std::map<std::string, std::string> configMap;


std::string sm4_key = 
    "00010011100100000100000000000000"
    "00001110000000000000000100000100"
    "00000000000000001110000000000000"
    "00010000010000000000000000001100"
;

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

void print(std::string s) {
    int k = 0;
    for(auto& c: s) {
        std::cout << c;
        ++k;
        if(k % 32 == 0) {
            std::cout << "\n";
        }
    }
    std::cout << "##\n";
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
    SM3 sm3(str);
    std::string hash = sm3.getHash();
    return hash;
}


int main(int argc, char** argv) {
    std::string path{"config.txt"};
    initConfig(path);
    
    std::fstream hashFile(configMap["hash_path"]);

    if(configMap["init"] == "true") {
        std::string hash = getSM3Hash();

        SM4 sm4(hash.substr(0, 128), sm4_key);
        sm4.genRK();
        sm4.encrypt();
        std::string encrypt_hash = sm4.getY();
        sm4.setMsg(hash.substr(128));
        sm4.encrypt();
        encrypt_hash += sm4.getY();

        hashFile << encrypt_hash;
        return 0;
    }
    
    std::string ans_hash;
    hashFile >> ans_hash;

    SM4 sm4(ans_hash.substr(0,128), sm4_key);
    sm4.genRK();
    sm4.decrypt();
    std::string hash = sm4.getY();
    sm4.setMsg(ans_hash.substr(128));
    sm4.decrypt();
    hash += sm4.getY();

    std::string cur_hash = getSM3Hash();

    if(hash != cur_hash) {
        goto boom;
    }

    printf("\033[33m OK\n");


    // USBChecker usbc;
    // usbc.initUSB(configMap);
    // auto res = usbc.checkUSB(); 
    // std::cout << res << std::endl;
    return 0;

boom:
    printf("\033[31m The system has been hacked !!!\n");
    return 0;
}
