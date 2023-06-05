#ifndef USB_H
#define USB_H

#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <regex>
#include <iostream>

enum USBState {
    NORMAL,
    SECURE,
    ERROR   
};

struct USBDevice {
    int vid;
    int pid;
    int port;

    USBDevice(int vid = 0, int pid = 0, int port = 0);
    
    void setID(int vid = 0, int pid = 0, int port = 0);

    bool operator< (const USBDevice& usb) const {
        if(this->vid == usb.vid) {
            if(this->pid == usb.pid) {
                return true;
            }
            return this->pid < usb.pid;
        }
        return this->vid < usb.vid;
    }
};

struct USBChecker {
    USBDevice normal_usb;
    USBDevice secure_usb;

    std::vector<USBDevice> filter;

    int normal_port;
    int secure_port;
    
    int initUSB(std::map<std::string, std::string>& configMap);

    USBState checkUSB();
};

#endif
