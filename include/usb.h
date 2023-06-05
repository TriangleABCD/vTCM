#ifndef USB_H
#define USB_H

#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include <vector>
#include <string>

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
};

struct USBChecker {
    USBDevice normal_usb;
    USBDevice secure_usb;

    std::vector<USBDevice> filter;

    int normal_port;
    int secure_port;
    
    int initUSB(std::string config_path);

    USBState checkUSB();
};

#endif