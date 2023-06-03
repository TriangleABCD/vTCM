#include <usb.h>

USBDevice::USBDevice(int vid = 0, int pid = 0, int port = 0) {
    this->vid  = vid;
    this->pid  = pid;
    this->port = port;
}
    
void USBDevice::setID(int vid = 0, int pid = 0, int port = 0) {
    this->vid  = vid;
    this->pid  = pid;
    this->port = port;
}
 
int USBChecker::initUSB(std::string config_path) {

}

USBState USBChecker::checkUSB() {
    libusb_init(nullptr);

    int r = libusb_init(nullptr);
    if(r < 0) {
        return USBState::ERROR; 
    }

    libusb_device_handle* handle = libusb_open_device
}

