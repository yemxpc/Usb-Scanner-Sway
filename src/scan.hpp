#ifndef SCAN_HPP
#define SCAN_HPP

#include <string>
#include <vector>
#include <filesystem>

struct scanresults{
    std::string filepath;
    std::string filehash;
    bool suspicious;
};


bool mountDevice(const std::string& devNode, const std::string& targetPath);
std::string findMountPoint(const std::string& devNode);
void startscan(const std::string& mountpath);
std::string calculatesha256(const std::string& filepath);

#endif