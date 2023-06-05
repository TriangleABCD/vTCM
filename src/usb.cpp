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


int USBChecker::initUSB(const std::map<std::string, std::string>& configMap) {
    
}


USBState USBChecker::checkUSB() {

}

