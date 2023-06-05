#include <usb.h>

USBDevice::USBDevice(int vid, int pid, int port) {
    this->vid  = vid;
    this->pid  = pid;
    this->port = port;
}


void USBDevice::setID(int vid, int pid, int port) {
    this->vid  = vid;
    this->pid  = pid;
    this->port = port;
}


int USBChecker::initUSB(std::map<std::string, std::string>& configMap) {
    int vid  = std::stoi(configMap["secure_usb_vid"],  nullptr, 16);
    int pid  = std::stoi(configMap["secure_usb_pid"],  nullptr, 16);
    int port = std::stoi(configMap["secure_usb_port"], nullptr, 16);
    this->secure_usb.setID(vid, pid, port);    

    vid  = std::stoi(configMap["normal_usb_vid"],  nullptr, 16);
    pid  = std::stoi(configMap["normal_usb_pid"],  nullptr, 16);
    port = std::stoi(configMap["normal_usb_port"], nullptr, 16);
    this->normal_usb.setID(vid, pid, port); 

    std::string filter_str = configMap["usb_filter"];
    std::regex pattern("0x([0-9A-Fa-f]+)");
    std::vector<std::pair<int, int>> pairs;
    auto it = std::sregex_iterator(filter_str.begin(), filter_str.end(), pattern);
    while (it != std::sregex_iterator()) {
        int first = std::stoi(it->str(), nullptr, 16);
        ++it;
        int second = std::stoi(it->str(), nullptr, 16);
        ++it;
        pairs.emplace_back(first, second);
    }
    return 0;
}


USBState USBChecker::checkUSB() {

}

