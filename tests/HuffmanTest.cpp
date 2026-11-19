
#include "algorithms/greedy/Huffman.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace warehouse::algorithms;

int main() {
    // Test 1: Empty input
    auto empty = huffmanEncode("");
    assert(empty.codes.empty());
    assert(empty.encodedData.empty());

    // Test 2: Single character
    auto single = huffmanEncode("AAAAA");
    assert(single.codes.size() == 1);
    assert(single.encodedData.size() == 5);

    // Test 3: Multiple characters
    std::string input = "AAAABBBCCDA";
    auto result = huffmanEncode(input);

    assert(!result.codes.empty());
    assert(!result.encodedData.empty());

    // Test 4: Every input character has a code
    for (char ch : input) {
        assert(result.codes.find(ch) != result.codes.end());
    }

    // Test 5: Verify prefix-free property
    for (const auto& a : result.codes) {
        for (const auto& b : result.codes) {
            if (a.first != b.first) {
                assert(b.second.find(a.second) != 0);
            }
        }
    }

    // Test 6: Decode using generated codes
    std::string decoded;
    std::string current;

    for (char bit : result.encodedData) {
        current += bit;

        for (const auto& entry : result.codes) {
            if (entry.second == current) {
                decoded += entry.first;
                current.clear();
                break;
            }
        }
    }

    assert(decoded == input);
    assert(current.empty());

    std::cout << "All Huffman Coding tests passed!\n";
    return 0;
}
