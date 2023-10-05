#include "HuffmanTree.hpp"

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


std::string HuffmanTree::serializeTree() const {
    return serializeHelper(root);
}

std::string HuffmanTree::serializeHelper(HuffmanNode* node) const {
    if (node == nullptr) {
        return "";
    }

    if (node->isLeaf()) {
        return "L" + std::string(1, node->getCharacter());
    } else {
        return "B" + serializeHelper(node->left) + serializeHelper(node->right);
    }
}


std::string HuffmanTree::decompress(const std::string inputCode, const std::string serializedTree) {
    size_t index = 0;
    root = deserializeHelper(serializedTree, index);

    HuffmanNode* currentNode = root;
    std::string decompressedStr;

    for (char bit : inputCode) {
        if (bit == '0') {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }

        if (currentNode->isLeaf()) {
            decompressedStr += currentNode->getCharacter();
            currentNode = root;
        }
    }

    return decompressedStr;
}

HuffmanNode* HuffmanTree::deserializeHelper(const std::string& serializedTree, size_t& index) {
    if (index >= serializedTree.size()) {
        return nullptr;
    }

    char type = serializedTree[index++];
    if (type == 'L') {
        char character = serializedTree[index++];
        return new HuffmanNode(character, 0);
    } else { // type == 'B'
        HuffmanNode* left = deserializeHelper(serializedTree, index);
        HuffmanNode* right = deserializeHelper(serializedTree, index);
        return new HuffmanNode('\0', 0, nullptr, left, right);
    }
}
