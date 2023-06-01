#include <stdio.h>
#include <libusb-1.0/libusb.h>

// Define the vendor and product IDs of the allowed USB device
#define VENDOR_ID 0x1234
#define PRODUCT_ID 0xabcd

int main()
{
    // Initialize the libusb library
    libusb_init(NULL);

    // Get the USB device handle for the specific port
    libusb_device_handle *dev = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, PRODUCT_ID);
    if (dev == NULL) {
        printf("Error: couldn't find USB device\n");
        return 1;
    }

    // Monitor the USB port for insertion of the allowed device
    while (true) {
        libusb_device_handle *new_dev = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, PRODUCT_ID);
        if (new_dev != NULL) {
            printf("Allowed USB device inserted!\n");
            // Mount the device or do whatever you want here
            libusb_close(new_dev);
            break;
        }
        libusb_handle_events_completed(NULL, NULL);
    }

    // Clean up and exit
    libusb_close(dev);
    libusb_exit(NULL);
    return 0;
}
