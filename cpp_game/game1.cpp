#include <cstdint>
#include <iostream>
#include <string>
#include <sys/statvfs.h>

void get_drive_info(const std::string& path) {
    struct statvfs stat;

    if (statvfs(path.c_str(), &stat) != 0) {
        std::cerr << "Failed to get drive info" << std::endl;
        return;
    }

    uint64_t total_space = stat.f_blocks * stat.f_frsize;
    uint64_t free_space = stat.f_bfree * stat.f_frsize;
    uint64_t available_space = stat.f_bavail * stat.f_frsize;

    std::cout << "Total space: " << total_space << " bytes" << std::endl;
    std::cout << "Free space: " << free_space << " bytes" << std::endl;
    std::cout << "Available space: " << available_space << " bytes" << std::endl;
}

int main() {
    get_drive_info("/");  // Replace with your drive's mount point or directory
    return 0;
}
