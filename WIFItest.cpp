#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <iwlib.h>

// Function to scan for available Wi-Fi networks
std::vector<std::string> scanWifiNetworks(const char* interfaceName) {
    std::vector<std::string> networks;

    // Initialize wireless context
    wireless_scan_head scanResult;
    iwrange range;
    int sock = iw_sockets_open();

    if (sock < 0) {
        std::cerr << "Error opening socket." << std::endl;
        return networks;
    }

    if (iw_get_range_info(sock, interfaceName, &range) < 0) {
        std::cerr << "Error getting range information." << std::endl;
        iw_sockets_close(sock);
        return networks;
    }

    // Perform the scan
    if (iw_scan(sock, const_cast<char*>(interfaceName), range.we_version_compiled, &scanResult) < 0) {
        std::cerr << "Error scanning for networks." << std::endl;
        iw_sockets_close(sock);
        return networks;
    }

    // Iterate through the scan results
    wireless_scan* scanPtr = scanResult.result;
    while (scanPtr != nullptr) {
        char ssid[IW_ESSID_MAX_SIZE + 1];
        memset(ssid, 0, sizeof(ssid));

        // Copy the ESSID (network name) to ssid
        iw_ether_ntop(&scanPtr->bssid, ssid);

        // Add the ESSID to the list of networks
        networks.push_back(ssid);

        scanPtr = scanPtr->next;
    }

    iw_sockets_close(sock);

    return networks;
}

int main() {
    const char* interfaceName = "wlan0"; // Replace with your wireless interface name
    std::vector<std::string> wifiNetworks = scanWifiNetworks(interfaceName);

    std::cout << "Available Wi-Fi Networks:" << std::endl;
    for (const std::string& network : wifiNetworks) {
        std::cout << network << std::endl;
    }

    return 0;
}
