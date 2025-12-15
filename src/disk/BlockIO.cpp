#include "BlockIO.h"
#include <iostream>

// Constructor initializes block size
BlockIO::BlockIO(size_t blockSize)
    : blockSize(blockSize) {}

// Open input file in binary mode
bool BlockIO::openInput(const std::string& inputPath) {
    inputFile.open(inputPath, std::ios::binary);
    return inputFile.is_open();
}

// Open output file in binary mode
bool BlockIO::openOutput(const std::string& outputPath) {
    outputFile.open(outputPath, std::ios::binary);
    return outputFile.is_open();
}

// Read one block from input file
bool BlockIO::readBlock(std::vector<unsigned char>& buffer) {
    if (!inputFile.is_open() || inputFile.eof()) {
        return false;
    }

    buffer.resize(blockSize);

    // Read up to blockSize bytes
    inputFile.read(reinterpret_cast<char*>(buffer.data()), blockSize);

    // Get actual number of bytes read
    std::streamsize bytesRead = inputFile.gcount();

    if (bytesRead <= 0) {
        return false;
    }

    // Resize buffer to actual bytes read
    buffer.resize(static_cast<size_t>(bytesRead));
    return true;
}

// Write a block to output file
bool BlockIO::writeBlock(const std::vector<unsigned char>& buffer) {
    if (!outputFile.is_open()) {
        return false;
    }

    outputFile.write(
        reinterpret_cast<const char*>(buffer.data()),
        buffer.size()
    );

    return outputFile.good();
}

// Close both files
void BlockIO::closeFiles() {
    if (inputFile.is_open()) {
        inputFile.close();
    }
    if (outputFile.is_open()) {
        outputFile.close();
    }
}
