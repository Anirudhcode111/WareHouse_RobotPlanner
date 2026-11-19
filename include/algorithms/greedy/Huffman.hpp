#pragma once

#include <string>
#include <unordered_map>

namespace warehouse::algorithms {

struct HuffmanResult {
    std::unordered_map<char, std::string> codes;
    std::string encodedData;
};

HuffmanResult huffmanEncode(const std::string& input);

}