#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <bitset>

struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f, Node *l = nullptr, Node *r = nullptr) 
        : ch(c), freq(f), left(l), right(r) {}
};

struct compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

void buildFrequencyTable(const std::string &text, std::map<char, int> &freq);
Node* buildHuffmanTree(const std::map<char, int> &freq);
void generateHuffmanCodes(Node* root, const std::string &code, std::map<char, std::string> &huffmanCode);
void compress(const std::string &text, const std::map<char, std::string> &huffmanCode, std::string &compressedText);
void writeCompressedFile(const std::string &filename, const std::string &compressedText, const std::map<char, std::string> &huffmanCode);
void decompress(const std::string &filename, std::string &decompressedText);

#endif
