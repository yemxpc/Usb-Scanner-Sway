#include <libudev.h>
#include <iostream>
#include <string>
#include <unistd.h> 
#include "scan.hpp"

int main() {
    
    struct udev* udev = udev_new();
    if (!udev) {
        std::cerr << "Error: Failed to initialize udev!" << std::endl;
        return 1;
    }

    // Monitor block devices for new partitions
    struct udev_monitor* mon = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(mon, "block", "partition");
    udev_monitor_enable_receiving(mon);

    std::cout << "--- Arch Linux USB Scanner Active ---" << std::endl;
    std::cout << ">>> Waiting for devices (Plug in a USB to start)..." << std::endl;

    while (true) {
        
        struct udev_device* dev = udev_monitor_receive_device(mon);
        if (dev) {
            const char* action = udev_device_get_action(dev);
            
            // Triggered when a new device is plugged in
            if (action && std::string(action) == "add") {
                const char* devnode = udev_device_get_devnode(dev);
                
                if (devnode) {
                    std::cout << "\n[!] Partition Detected: " << devnode << std::endl;
                    std::cout << "[*] Checking mount point..." << std::endl;
                    
                    // Giving the system a few seconds to settle
                    sleep(3); 

                    std::string mountPath = findMountPoint(devnode);

                    // If not auto-mounted, try manual mount
                    if (mountPath.empty()) {
                        std::cout << "[*] Device not mounted. Attempting manual mount..." << std::endl;
                        std::string manualTarget = "/mnt/usb_scanner_target";
                        
                        if (mountDevice(devnode, manualTarget)) {
                            mountPath = manualTarget;
                        }
                    }

                    if (!mountPath.empty()) {
                        std::cout << "[+] Scan Directory: " << mountPath << std::endl;
                        std::cout << "[>] Starting SHA-256 analysis..." << std::endl;
                        
                        // Ensure the function name matches your scan.hpp (startScan or startscan)
                        startscan(mountPath);
                        
                    } else {
                        std::cout << "[X] Error: Could not find or create a mount point." << std::endl;
                    }
                }
            }
            
            udev_device_unref(dev);
        }
    }

    udev_unref(udev);
    return 0;
}
