#include <cstdint>      
#include <string_view>  
#include <string>       
#include <fstream>      
#include <iostream>     
#include <bitset>       

uint32_t ipToInt(std::string_view str) {
    uint32_t ans = 0;
    size_t start = 0;
    uint32_t shift = 24;

    for (size_t i = 0; i < 4; ++i) {
        size_t end = str.find('.', start);
        if (end == std::string_view::npos) {
            end = str.length();  // Last part of the IP
        }

        // Parse the byte part of the IP address
        uint32_t byte = std::stoi(std::string(str.substr(start, end - start)));
        ans |= (byte << shift);  // Shift and OR to accumulate the result
        shift -= 8;
        start = end + 1;
    }

    return ans;
}

uint32_t find(const std::string& name) {
    std::ifstream iff(name);
    uint32_t size;
    // Using a more reasonable bitset size (max IP address value is 2^32 - 1)
    std::bitset<4294967296ULL> bits;  // 2^32 = 4,294,967,296

    if (!iff.is_open()) {
        std::cerr << "Cannot open the file" << std::endl;
        return 1;
    }

    iff >> size;  // Read the number of IPs
    std::string Ip;
    for (uint32_t i = 0; i < size; ++i) {
        iff >> Ip;
        std::string_view ipView(Ip);
        uint32_t ipInt = ipToInt(ipView);
        bits.set(ipInt);  // Set the bit corresponding to this IP
    }

    iff.close();  // Explicitly close the file
    return static_cast<uint32_t>(bits.count());  // Count the set bits (unique IPs)
}
