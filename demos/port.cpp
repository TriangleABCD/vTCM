#include <iostream>
#include <libusb-1.0/libusb.h>

#define VENDOR_ID  0x14cd
#define PRODUCT_ID 0x1212

int main(int argc, char* argv[])
{
    libusb_device_handle* handle = nullptr;
    int result = libusb_init(nullptr);
    if (result < 0)
    {
        std::cerr << "Error initializing libusb: " << libusb_error_name(result) << std::endl;
        return -1;
    }
    handle = libusb_open_device_with_vid_pid(nullptr, VENDOR_ID, PRODUCT_ID);
    if (handle == nullptr)
    {
        std::cerr << "Error finding USB device: " << libusb_error_name(result) << std::endl;
        libusb_exit(nullptr);
        return -1;
    }
    libusb_device* device = libusb_get_device(handle);
    libusb_device_descriptor descriptor = {};
    result = libusb_get_device_descriptor(device, &descriptor);
    if (result < 0)
    {
        std::cerr << "Error getting device descriptor: " << libusb_error_name(result) << std::endl;
        libusb_close(handle);
        libusb_exit(nullptr);
        return -1;
    }
    uint8_t port_number = libusb_get_port_number(libusb_get_device(handle));
    std::cout << "USB device is connected to port " << static_cast<int>(port_number) << std::endl;
    libusb_close(handle);
    libusb_exit(nullptr);
    return 0;
}
