#include "HuffmanTree.hpp"
#include <stack>
#include <unordered_map>

HuffmanTree::HuffmanTree() : root(nullptr) {}

HuffmanTree::~HuffmanTree() {
    deleteTree(root);
}

void HuffmanTree::buildTree(const std::unordered_map<char, size_t>& frequencies) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, HuffmanNode::Compare> pq;

    // Create leaf nodes for each character and its frequency, then add them to the priority queue.
    for (const auto& pair : frequencies) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    // Combine nodes with the lowest frequencies until only the root node remains.
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        size_t combinedFrequency = left->getFrequency() + right->getFrequency();
        HuffmanNode* branch = new HuffmanNode('\0', combinedFrequency, nullptr, left, right);

        pq.push(branch);
    }

    root = pq.top(); pq.pop();
}


void HuffmanTree::generateCodes(HuffmanNode* node, const std::string& code) {
    if (node->isLeaf()) {
        charCodes[node->getCharacter()] = code;
        return;
    }

    if (node->left != nullptr) {
        generateCodes(node->left, code + "0");
    }
    if (node->right != nullptr) {
        generateCodes(node->right, code + "1");
    }
}


void HuffmanTree::deleteTree(HuffmanNode* node) {
    if (node == nullptr) {
        return;
    }

    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

std::string HuffmanTree::compress(const std::string inputStr) {
    std::unordered_map<char, size_t> frequencies;
    for (char c : inputStr) {
        frequencies[c]++;
    }

    buildTree(frequencies);
    generateCodes(root, "");

    std::string compressedStr;
    for (char c : inputStr) {
        compressedStr += charCodes[c];
    }

    return compressedStr;
}

std::string HuffmanTree::serializeTree() const {
    std::string serialized;
    std::stack<HuffmanNode*> nodes;
    nodes.push(root);

    while (!nodes.empty()) {
        HuffmanNode* current = nodes.top();
        nodes.pop();

        if (current->isLeaf()) {
            serialized += "L" + std::string(1, current->getCharacter());
        } else {
            serialized += "B";
            nodes.push(current->right);
            nodes.push(current->left);
        }
    }

    return serialized;
}
std::string HuffmanTree::decompress(const std::string inputCode, const std::string serializedTree) {
    // Reconstruct the Huffman Tree
    std::stack<HuffmanNode*> nodes;
    for (size_t i = 0; i < serializedTree.size(); ++i) {
        char c = serializedTree[i];
        if (c == 'B') {
            nodes.push(new HuffmanNode('\0', 0));
        } else { // c == 'L'
            nodes.push(new HuffmanNode(serializedTree[++i], 0));
        }

        if (nodes.size() >= 2) {
            HuffmanNode* right = nodes.top(); nodes.pop();
            HuffmanNode* left = nodes.top(); nodes.pop();
            HuffmanNode* parent = new HuffmanNode('\0', 0, nullptr, left, right);
            nodes.push(parent);
        }
    }
    root = nodes.top(); nodes.pop();

    // Decode the input code
    HuffmanNode* currentNode = root;
    std::string decompressedStr;
    for (char bit : inputCode) {
        currentNode = (bit == '0') ? currentNode->left : currentNode->right;

        if (currentNode->isLeaf()) {
            decompressedStr += currentNode->getCharacter();
            currentNode = root;
        }
    }

    return decompressedStr;
}
