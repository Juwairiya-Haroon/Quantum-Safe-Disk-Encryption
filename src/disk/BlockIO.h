#ifndef BLOCKIO_H
#define BLOCKIO_H

#include <fstream>
#include <vector>
#include <string>

/*s
 * BlockIO
 * -------
 * Handles block-level file input/output.
 * Reads and writes files in fixed-size chunks (blocks).
 * Does NOT perform encryption or decryption.
 */
class BlockIO {
private:
    std::ifstream inputFile;     // Input file stream
    std::ofstream outputFile;    // Output file stream
    size_t blockSize;            // Size of each block in bytes

public:
    // Constructor: sets block size (default = 4096 bytes)
    explicit BlockIO(size_t blockSize = 4096);

    // Opens file for reading (binary mode)
    bool openInput(const std::string& inputPath);

    // Opens file for writing (binary mode)
    bool openOutput(const std::string& outputPath);

    // Reads one block from input file into buffer
    // Returns false when no more data is available
    bool readBlock(std::vector<unsigned char>& buffer);

    // Writes a block of data to output file
    bool writeBlock(const std::vector<unsigned char>& buffer);

    // Closes input and output files safely
    void closeFiles();
};

#endif // BLOCKIO_H
