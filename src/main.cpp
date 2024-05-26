#include "huffman.h"
#include <sstream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>\n";
        return 1;
    }

    std::string inputFilePath = argv[1];
    std::string outputFilePath = argv[2];

    std::string text;
    std::ifstream inputFile(inputFilePath);
    if (inputFile) {
        std::stringstream buffer;
        buffer << inputFile.rdbuf();
        text = buffer.str();
    }
    inputFile.close();
    
    std::map<char, int> freq;
    buildFrequencyTable(text, freq);
    
    Node* root = buildHuffmanTree(freq);
    
    std::map<char, std::string> huffmanCode;
    generateHuffmanCodes(root, "", huffmanCode);
    
    std::string compressedText;
    compress(text, huffmanCode, compressedText);
    
    writeCompressedFile(outputFilePath, compressedText, huffmanCode);
    
    return 0;
}
