#include <libudev.h>
#include <iostream>
#include <string>
#include <unistd.h> 
#include "scan.hpp"



int main() {
    
    struct udev* udev = udev_new();
    if (!udev) {
        std::cerr << "udev başlatılamadı!" << std::endl;
        return 1;
    }

   
    struct udev_monitor* mon = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(mon, "block", "partition");
    udev_monitor_enable_receiving(mon);

    std::cout << "--- Arch Linux USB Scanner Aktif ---" << std::endl;
    std::cout << ">>> Cihaz bekleniyor (USB takıldığında algılanacak)..." << std::endl;

    while (true) {
        
        struct udev_device* dev = udev_monitor_receive_device(mon);
        if (dev) {
            const char* action = udev_device_get_action(dev);
            
            
            if (action && std::string(action) == "add") {
                const char* devnode = udev_device_get_devnode(dev);
                
                if (devnode) {
                    std::cout << "\n[!] Disk Bölümü Algılandı: " << devnode << std::endl;
                    
                   
                    std::cout << "Mount noktası kontrol ediliyor..." << std::endl;
                    sleep(3); 

                    
                    std::string mountPath = findMountPoint(devnode);

                    
                    if (mountPath.empty()) {
                        std::cout << "[*] Cihaz bağlı değil. Manuel bağlama deneniyor..." << std::endl;
                        std::string manualTarget = "/mnt/usb_scanner_target";
                        
                        if (mountDevice(devnode, manualTarget)) {
                            mountPath = manualTarget;
                        }
                    }

                   
                    if (!mountPath.empty()) {
                        std::cout << "[+] Tarama Dizini: " << mountPath << std::endl;
                        startscan(mountPath);
                        
                        
                    } else {
                        std::cout << "[X] Hata: Cihaz bağlama noktası bulunamadı/oluşturulamadı." << std::endl;
                    }
                }
            }
            
            udev_device_unref(dev);
        }
    }

    udev_unref(udev);
    return 0;
}