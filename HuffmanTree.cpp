#include "HuffmanTree.hpp"

HuffmanTree::~HuffmanTree() {
    // Recursive function to delete all nodes in the tree
    std::function<void(HuffmanNode*)> deleteTree;
    deleteTree = [&](HuffmanNode* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    };

    deleteTree(root);
}


std::string HuffmanTree::compress(const std::string inputStr) {
    // Ensure the input string is not empty
    if (inputStr.empty()) {
        return "";
    }

    // Calculate the frequency of each character
    std::map<char, size_t> frequencyMap;
    for (char c : inputStr) {
        frequencyMap[c]++;
    }

    // Insert each character and its frequency into a priority queue
    HeapQueue<HuffmanNode*, HuffmanNode::Compare> heap;
    for (const auto& pair : frequencyMap) {
        heap.insert(new HuffmanNode(pair.first, pair.second));
    }

    // Build the Huffman tree using the priority queue
    while (heap.size() > 1) {
        HuffmanNode* left = heap.min();
        heap.removeMin();
        HuffmanNode* right = heap.min();
        heap.removeMin();
        size_t combinedFrequency = left->getFrequency() + right->getFrequency();
        HuffmanNode* parent = new HuffmanNode('\0', combinedFrequency);
        parent->left = left;
        parent->right = right;
        heap.insert(parent);
    }

    // Before setting a new root, delete the old Huffman tree if it exists
    if (root) {
        std::function<void(HuffmanNode*)> deleteTree;
        deleteTree = [&](HuffmanNode* node) {
            if (node) {
                deleteTree(node->left);
                deleteTree(node->right);
                delete node;
            }
        };
        deleteTree(root);
    }

    root = heap.min();

    // Generate Huffman codes
    std::map<char, std::string> huffmanCodes;
    std::function<void(HuffmanNode*, const std::string&)> generateCodes = 
    [&](HuffmanNode* node, const std::string& code) {
        if (node->isLeaf()) {
            huffmanCodes[node->getCharacter()] = code;
            return;
        }
        if (node->left) {
            generateCodes(node->left, code + "0");
        }
        if (node->right) {
            generateCodes(node->right, code + "1");
        }
    };
    generateCodes(root, "");

    // Replace each character in the input string with its Huffman code
    std::string compressedStr;
    for (char c : inputStr) {
        compressedStr += huffmanCodes[c];
    }

    return compressedStr;

}


std::string HuffmanTree::serializeTree() const {
    std::string serialized;
    std::function<void(const HuffmanNode*)> serialize = [&](const HuffmanNode* node) {
        if (!node) {
            std::cout << "Serializing Null Node" << std::endl; // Debug output for null node
            return;
        }
        if (node->isLeaf()) {
            serialized += "L" + std::string(1, node->getCharacter());
            if (node->getCharacter() == '\0') {
                std::cout << "Serializing Leaf: Null Character" << std::endl; // Debug output for null character
            } else if (node->getCharacter() == ' ') {
                std::cout << "Serializing Leaf: Space Character" << std::endl; // Debug output for space character
            } else {
                std::cout << "Serializing Leaf: " << node->getCharacter() << std::endl; // Debug output for other characters
            }
            return;
        }
        serialize(node->left);
        serialize(node->right);
        serialized += "B";
        std::cout << "Serializing Branch" << std::endl; // Debug output
    };
    serialize(root);
    std::cout << "Serialized Tree: " << serialized << std::endl; // Debug output
    return serialized;
}


std::string HuffmanTree::decompress(const std::string inputCode, const std::string serializedTree) {

    size_t index = 0;
    std::function<HuffmanNode*()> deserialize = [&]() -> HuffmanNode* {
        while (index < serializedTree.size() && serializedTree[index] == ' ') {
            index++; // Skip spaces
        }

        if (index >= serializedTree.size()) {
            return nullptr;
        }

        if (serializedTree[index] == 'L') {
            index++; // Move past 'L'
            if (index < serializedTree.size()) {
                char character = serializedTree[index++];
                std::cout << "Deserializing Leaf: " << character << " at index " << index - 1 << std::endl;
                return new HuffmanNode(character, 0);
            }
        } else if (serializedTree[index] == 'B') {
            index++; // Move past 'B'
            HuffmanNode* left = deserialize();
            HuffmanNode* right = deserialize();
            HuffmanNode* branch = new HuffmanNode('\0', 0);
            branch->left = left;
            branch->right = right;
            return branch;
        }
        return nullptr;
    };

    root = deserialize();

    HuffmanNode* currentNode = root;
    std::string decompressedStr;
    for (char bit : inputCode) {
        if (bit == '0') {
            if (!currentNode->left) {
                std::cout << "Error: Trying to traverse left, but left child is null!" << std::endl;
                std::cout << "Current bit: " << bit << ", Current char: " << currentNode->getCharacter() << std::endl;
                break;  // Exit the loop
            }
            currentNode = currentNode->left;
        } else {
            if (!currentNode->right) {
                std::cout << "Error: Trying to traverse right, but right child is null!" << std::endl;
                std::cout << "Current bit: " << bit << ", Current char: " << currentNode->getCharacter() << std::endl;
                break;  // Exit the loop
            }
            currentNode = currentNode->right;
        }
        if (currentNode->isLeaf()) {
            decompressedStr += currentNode->getCharacter();
            currentNode = root;
        }
    }
    return decompressedStr;
}


/*void HuffmanTree::printTree(const HuffmanNode* node, const std::string& prefix, bool isLeft) const {
    if (!node) return;

    std::cout << prefix;
    std::cout << (isLeft ? "|- " : "\\- ");

    if (node->isLeaf()) {
        std::cout << "Leaf: " << node->getCharacter() << " (" << node->getFrequency() << ")" << std::endl;
    } else {
        std::cout << "Branch: (" << node->getFrequency() << ")" << std::endl;
    }

    printTree(node->left, prefix + (isLeft ? "|  " : "   "), true);
    printTree(node->right, prefix + (isLeft ? "|  " : "   "), false);
}*/


