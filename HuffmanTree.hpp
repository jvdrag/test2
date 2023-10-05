#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include "HuffmanBase.hpp"
#include <queue>
#include <unordered_map>

class HuffmanTree : public HuffmanTreeBase {
public:
    HuffmanTree();
    virtual ~HuffmanTree();
    virtual std::string compress(const std::string inputStr) override;
    virtual std::string serializeTree() const override;
    virtual std::string decompress(const std::string inputCode, const std::string serializedTree) override;

private:
    HuffmanNode* root;
    std::unordered_map<char, std::string> charCodes;

    void buildTree(const std::unordered_map<char, size_t>& frequencies);
    void generateCodes(HuffmanNode* node, const std::string& code);
    void deleteTree(HuffmanNode* node);

    // Added declarations for the helper methods
    // std::string serializeHelper(HuffmanNode* node) const;
    // HuffmanNode* deserializeHelper(const std::string& serializedTree, size_t& index);
};

#endif /* HUFFMANTREE_H */
