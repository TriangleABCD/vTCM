#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

int main(void)
{
    libusb_device **devs;
    int r;
    ssize_t cnt;

    r = libusb_init(NULL);
    if (r < 0) {
        printf("libusb_init error: %d\n", r);
        return 1;
    }

    cnt = libusb_get_device_list(NULL, &devs);
    if (cnt < 0) {
        printf("libusb_get_device_list error: %zd\n", cnt);
        return 1;
    }

    printf("Found %zd USB devices:\n", cnt);

    for (ssize_t i = 0; i < cnt; i++) {
        struct libusb_device *dev = devs[i];
        struct libusb_device_descriptor desc;
        r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0) {
            printf("libusb_get_device_descriptor error: %d\n", r);
            continue;
        }
        printf("- Vendor ID: 0x%04x, Product ID: 0x%04x\n",
               desc.idVendor, desc.idProduct);
    }

    libusb_free_device_list(devs, 1);
    libusb_exit(NULL);

    return 0;
}
