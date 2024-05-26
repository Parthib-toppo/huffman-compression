#include "huffman.h"

void buildFrequencyTable(const std::string &text, std::map<char, int> &freq) {
    for (char ch : text) {
        freq[ch]++;
    }
}

Node* buildHuffmanTree(const std::map<char, int> &freq) {
    std::priority_queue<Node*, std::vector<Node*>, compare> pq;
    for (auto pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }
    
    while (pq.size() != 1) {
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();
        int sum = left->freq + right->freq;
        pq.push(new Node('\0', sum, left, right));
    }
    
    return pq.top();
}

void generateHuffmanCodes(Node* root, const std::string &code, std::map<char, std::string> &huffmanCode) {
    if (root == nullptr) return;

    if (!root->left && !root->right) {
        huffmanCode[root->ch] = code;
    }
    
    generateHuffmanCodes(root->left, code + "0", huffmanCode);
    generateHuffmanCodes(root->right, code + "1", huffmanCode);
}

void compress(const std::string &text, const std::map<char, std::string> &huffmanCode, std::string &compressedText) {
    for (char ch : text) {
        compressedText += huffmanCode.at(ch);
    }
}

void writeCompressedFile(const std::string &filename, const std::string &compressedText, const std::map<char, std::string> &huffmanCode) {
    std::ofstream outFile(filename, std::ios::binary);
    
    size_t originalSize = compressedText.size();
    outFile.write(reinterpret_cast<const char*>(&originalSize), sizeof(originalSize));
    
    for (const auto &pair : huffmanCode) {
        outFile.put(pair.first);
        size_t codeSize = pair.second.size();
        outFile.write(reinterpret_cast<const char*>(&codeSize), sizeof(codeSize));
        outFile.write(pair.second.c_str(), codeSize);
    }
    
    outFile.put('\0');
    
    for (size_t i = 0; i < compressedText.size(); i += 8) {
        std::bitset<8> bits(compressedText.substr(i, 8));
        outFile.put(static_cast<unsigned char>(bits.to_ulong()));
    }
    
    outFile.close();
}

void decompress(const std::string &filename, std::string &decompressedText) {
    std::ifstream inFile(filename, std::ios::binary);
    
    size_t originalSize;
    inFile.read(reinterpret_cast<char*>(&originalSize), sizeof(originalSize));
    
    std::map<char, std::string> huffmanCode;
    char ch;
    while (inFile.get(ch) && ch != '\0') {
        size_t codeSize;
        inFile.read(reinterpret_cast<char*>(&codeSize), sizeof(codeSize));
        std::string code(codeSize, ' ');
        inFile.read(&code[0], codeSize);
        huffmanCode[ch] = code;
    }
    
    std::map<std::string, char> reverseHuffmanCode;
    for (const auto &pair : huffmanCode) {
        reverseHuffmanCode[pair.second] = pair.first;
    }
    
    std::string compressedText;
    unsigned char byte;
    while (inFile.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        compressedText += std::bitset<8>(byte).to_string();
    }
    
    inFile.close();
    
    std::string code;
    for (char bit : compressedText) {
        code += bit;
        if (reverseHuffmanCode.find(code) != reverseHuffmanCode.end()) {
            decompressedText += reverseHuffmanCode[code];
            code = "";
        }
        if (decompressedText.size() == originalSize) break;
    }
}
