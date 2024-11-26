#include <iostream>
#include "HCTree.hpp"
#include "Helper.hpp"

using namespace std;

int readInt(FancyInputStream & in, bool & success) {
    int result = 0;
    success = true;
    for (int i = 0; i < 4; i++) { //assemble byte into int
        int byte = in.read_byte(); 
        if (byte == -1) { //if read returns -1, set success to false, and exit
            success = false;
            return 0;
        }
        result = (result << 8) | (byte & 0xFF);
    }
    return result;
}

int main(int argc, char* argv[]) {

    //check correct arg count
    if (argc != 3) {
        cerr << "Usage: ./compress <orgignal_file> <compressed_file>" << endl;
        return 1;
    }

    //intialize file to store input and output
    const char* compressed_file = argv[1];
    const char* uncompressed_file = argv[2];

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

    //read num of symbols, initialize freq vector

    bool success;
    //int countSymbols = inputFile.read_int();
    
    if (!success) return 0;

    //check if compressed file is empty
    if (countSymbols == 0) return 0;

    vector<int> freqs(256, 0);

    //read symbol and respective freq
    for (int i = 0; i < countSymbols; i++) {
        int symbol = inputFile.read_byte(); //symbol

        if (symbol == -1) {
            cerr << "error reading header" << endl;
            return 1;
        }

        //int freq = inputFile.read_int();  //freq (important: symbol first to read correct data)
        int freq = readInt(inputFile, success);
        if (!success) {
            cerr << "error reading header" << endl;
            return 1;
        }

        freqs[symbol] = freq;
    }

    HCTree hmt;
    hmt.build(freqs);

    /**
     * decode compressed data
     * obj: calculate total number of symbols from freqs
     * to find num symbols to decode
     */
    int totalSymbols = 0;
    for (int numfreq : freqs) { //count
        totalSymbols += numfreq;
    }

    //decode
    for(int i = 0; i < totalSymbols; i++) {
        unsigned char currSymbol = hmt.decode(inputFile);
        outputFile.write_byte(currSymbol);
    }

    //flush remainder bits
    outputFile.flush();
    return 0;
}