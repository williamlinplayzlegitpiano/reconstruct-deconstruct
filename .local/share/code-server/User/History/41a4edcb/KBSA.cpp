#include <iostream>
#include "HCTree.hpp"
#include "Helper.hpp"

using namespace std;

int main(int argc, char* argv[]) {

    //check correct arg count
    if (argc != 3) {
        cerr << "Usage: ./compress <orgignal_file> <compressed_file>" << endl;
        return 1;
    }

    //intialize file to store input and output
    const char* original_file = argv[1];
    const char* compressed_file = argv[2];

    //open input file
    FancyInputStream inputFile(original_file);
    if (!inputFile.good()) {
        cerr << "Cannot open input file: " << original_file << endl;
        return 1;
    }

    //open output file
    FancyOutputStream outputFile(compressed_file);
    if (!outputFile.good()) {
        cerr << "Cannot open output file: " << compressed_file << endl;
        return 1;
    }

    /**
     * count frequency, using freqs as an empty vector
     * and counting frequencies in inputfile
     */

    //intialize vector size 256, each index = possible byte value
    vector<int> freqs(256,0);

    int scanByte;
    while ((scanByte = inputFile.read_byte()) != -1) {
        freqs[scanByte]++;
    }

    //reset inputFile after usage
    inputFile.reset();

    //build hmt
    HCTree hmt;
    hmt.build(freqs);

    //store header via writing freqs
    for (int i = 0; i < 256; i++) {
        outputFile.write_int(freqs[i]);
    }

    //encode input file by reading each byte
    unsigned char symbol;
    while ((scanByte = inputFile.read_byte()) != -1) {
        symbol = static_cast<unsigned char>(scanByte);
        hmt.encode(symbol, outputFile);
    }

    outputFile.flush();//flush remainder bits

    return 0;

}