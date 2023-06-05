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
    
    auto it = std::sregex_iterator(filter_str.begin(), filter_str.end(), pattern);
    while (it != std::sregex_iterator()) {
        int first = std::stoi(it->str(), nullptr, 16);
        ++it;
        int second = std::stoi(it->str(), nullptr, 16);
        ++it;
        this->filter.push_back(USBDevice(first, second));
    }
    return 0;
}


USBState USBChecker::checkUSB() {
    libusb_device** devs;
    libusb_context* ctx = nullptr;

    int r = libusb_init(&ctx);
    if (r < 0) {
        return USBState::ERROR;
    }
    int cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0) {
        libusb_exit(ctx);
        return USBState::ERROR;
    }

    std::vector<USBDevice> usb_list;

    for (int i = 0; i < cnt; ++i) {
        libusb_device *dev = devs[i];

        struct libusb_device_descriptor desc;
        r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0) {
            continue;
        }

        r = libusb_get_port_number(dev);
        if (r < 0) {
            continue;
        }

        usb_list.push_back(USBDevice(static_cast<int>(desc.idVendor), static_cast<int>(desc.idProduct), r));
    }

    libusb_free_device_list(devs, 1);
    libusb_exit(ctx);

    std::set<USBDevice> fil;

    for(auto f: this->filter) {
        fil.insert(f);
    }

    std::vector<USBDevice> fil_usb_list;

    for(auto& u: usb_list) {
        if(fil.find(u) == fil.end()) {
            fil_usb_list.push_back(u);
        }
    }

    if(fil_usb_list.size() != 1) {
        return USBState::ERROR;
    }

    auto& usb = fil_usb_list[0];

    if(usb.vid == this->secure_usb.vid && usb.pid == this->secure_usb.pid && usb.port == this->secure_usb.port) {
        return USBState::SECURE;
    } else if(usb.vid == this->normal_usb.vid && usb.pid == this->normal_usb.pid && usb.port == this->normal_usb.port) {
        return USBState::NORMAL;
    }

    return USBState::ERROR;
}

