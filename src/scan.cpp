#include "scan.hpp"
#include "picosha2.h"
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

std::string calculatesha256(const std::string& filepath){
    std::ifstream file(filepath, std::ios::binary);
    if(!file) return "failed to open file";

    std::vector <unsigned char> hash(picosha2::k_digest_size);
    picosha2::hash256(file,hash.begin(),hash.end());

    return picosha2::bytes_to_hex_string(hash.begin(), hash.end());
}

void startscan(const std::string& mountpath){
    std::cout<<"Scan has Started"<< mountpath <<std::endl;
    std::cout<<"----------------"<<std::endl;

    try{
        for(const auto entry : fs::recursive_directory_iterator(mountpath)){
            if(fs::is_regular_file(entry)){
                std::string path = entry.path().string();
                std::string hash = calculatesha256(path);

                std::cout << "[File]"<<entry.path().filename().string()<<std::endl;
                std::cout <<"[Hash]"<<hash << std::endl;

                if (hash == "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855") { 
                    // Bu hash boş bir dosyanın hash'idir
                    std::cout << "[!!!] Warning" << std::endl;
                }
                std::cout << "------------------------------------------" << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR]: " << e.what() << std::endl;
    }
}
     
std::string findMountPoint(const std::string& devNode) {
    std::ifstream mounts("/proc/mounts");
    std::string line;

    while (std::getline(mounts, line)) {
        if (line.find(devNode) != std::string::npos) {
            std::stringstream ss(line);
            std::string device, mountPoint, fsType;
            ss >> device >> mountPoint >> fsType;
            return mountPoint;
        }
    }
    return "";
}
    

bool mountDevice(const std::string& devNode, const std::string& targetPath) {
    
    std::string mkdirCmd = "sudo mkdir -p " + targetPath;
    system(mkdirCmd.c_str());

    
    std::string mountCmd = "sudo mount " + devNode + " " + targetPath;
    std::cout << "[DEBUG] Executing: " << mountCmd << std::endl;
    
    return (system(mountCmd.c_str()) == 0);
}