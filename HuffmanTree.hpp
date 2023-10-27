#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include "HuffmanBase.hpp"
#include "HeapQueue.hpp"
#include <map>
#include <queue>
#include <functional>
#include <string>
#include <iostream>

// HuffmanTree class definition
class HuffmanTree : public HuffmanTreeBase {
private:
    HuffmanNode* root;
    std::map<char, std::string> huffmanCodes;
    std::map<std::string, char> reverseHuffmanCodes;

    //void printTree(const HuffmanNode* node, const std::string& prefix = "", bool isLeft = true) const;


public:
    HuffmanTree() : root(nullptr) {}
    ~HuffmanTree();

    std::string compress(const std::string inputStr) override;
    std::string serializeTree() const override;
    std::string decompress(const std::string inputCode, const std::string serializedTree) override;

    bool isValidSerializedTree(const std::string& serializedTree);
};

#endif /* HUFFMANTREE_H */
