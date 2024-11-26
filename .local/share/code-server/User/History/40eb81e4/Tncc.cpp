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
    FancyInputStream inputFile(compressed_file);
    if (!inputFile.good()) {
        cerr << "Cannot open input file: " << compressed_file << endl;
        return 1;
    }

    //open output file
    FancyOutputStream outputFile(uncompressed_file);
    if (!outputFile.good()) {
        cerr << "Cannot open output file: " << uncompressed_file << endl;
        return 1;
    }

    //read header, build hmt
    /* vector<int> freqs(256,0);
    for (int i = 0; i < 256; i++) {
        freqs[i] = inputFile.read_int();
    } */

    //read num of symbols

    int countSymbols = inputFile.read_int();
    

    HCTree hmt;
    hmt.build(freqs);

    /**
     * decode compressed data
     * obj: calculate total number of symbols from freqs
     * to find num symbols to decode
     */
    int numSymbols = 0;
    for (int numfreq : freqs) { //count
        numSymbols += numfreq;
    }

    //decode
    for(int i = 0; i < numSymbols; i++) {
        unsigned char currSymbol = hmt.decode(inputFile);
        outputFile.write_byte(currSymbol);
    }

    //flush remainder bits
    outputFile.flush();

    return 0;
}