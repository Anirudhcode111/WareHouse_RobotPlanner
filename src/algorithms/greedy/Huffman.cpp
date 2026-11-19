#include "algorithms/greedy/Huffman.hpp"

#include <queue>
#include <memory>
#include <vector>

namespace warehouse::algorithms {
namespace {

struct Node {
    char character;
    int frequency;

    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    Node(char ch, int freq)
        : character(ch), frequency(freq) {}
};

struct Compare {
    bool operator()(
        const std::shared_ptr<Node>& a,
        const std::shared_ptr<Node>& b
    ) const {
        return a->frequency > b->frequency;
    }
};

void generateCodes(
    const std::shared_ptr<Node>& node,
    const std::string& code,
    std::unordered_map<char, std::string>& codes
) {
    if (!node) return;

    if (!node->left && !node->right) {
        codes[node->character] = code.empty() ? "0" : code;
        return;
    }

    generateCodes(node->left, code + "0", codes);
    generateCodes(node->right, code + "1", codes);
}

}

HuffmanResult huffmanEncode(const std::string& input) {
    HuffmanResult result;

    if (input.empty()) {
        return result;
    }

    std::unordered_map<char, int> frequency;

    for (char ch : input) {
        frequency[ch]++;
    }

    std::priority_queue<
        std::shared_ptr<Node>,
        std::vector<std::shared_ptr<Node>>,
        Compare
    > minHeap;

    for (const auto& entry : frequency) {
        minHeap.push(
            std::make_shared<Node>(entry.first, entry.second)
        );
    }

    while (minHeap.size() > 1) {
        auto left = minHeap.top();
        minHeap.pop();

        auto right = minHeap.top();
        minHeap.pop();

        auto parent = std::make_shared<Node>(
            '\0',
            left->frequency + right->frequency
        );

        parent->left = left;
        parent->right = right;

        minHeap.push(parent);
    }

    generateCodes(minHeap.top(), "", result.codes);

    for (char ch : input) {
        result.encodedData += result.codes.at(ch);
    }

    return result;
}

}