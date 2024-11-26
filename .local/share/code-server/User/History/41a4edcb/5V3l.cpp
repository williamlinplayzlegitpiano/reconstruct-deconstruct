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

    //check if input is empty
    bool isEmpty = true;
    for (int freq : freqs) {
        if (freq > 0) {
            isEmpty = false;
            break;
        }
    }

    if (isEmpty) {
        return 0;
    }

    //reset inputFile after usage
    inputFile.reset();

    //build hmt
    HCTree hmt;
    hmt.build(freqs);

    //store header via writing freqs

    //find max freq
    int maxfreq = *std::max_element(freqs.begin(), freqs.end());

    //find smallest data type needed to hold freq
    bool useByte = maxfreq <= 255; //use 1 byte
    bool useShort = maxfreq <= 65535; //use 2 byte

    //write flag showing size of freq
    if (useByte) {
        outputFile.write_byte(1); //1 byte freq
    } else if (useShort) {
        outputFile.write_byte(2);
    } else {
        outputFile.write_byte(4);
    }

    //count nonzero freq for efficiency
    int numSymbols = 0;
    for (int numFreq : freqs) {
        if (numFreq > 0) {
            numSymbols++;
        }
    }

    //write num of symbols
    outputFile.write_int(numSymbols);

    //keep nonzero freq and respective symbol
    for (int i = 0; i < 256; i++) {
        if (freqs[i] > 0) {
            //symbol always 4 bytes
            outputFile.write_byte(static_cast<unsigned char>(i)); //symbol
            if (useByte) {
                outputFile.write_byte(static_cast<unsigned char>(freqs[i]));
            } else if (useShort) {
                uint16_t shortfreq = static_cast<uint16_t>(freqs[i]);
                outputFile.write_bytes(reinterpret_cast<char*>(&shortfreq), 2);
            } else {
                outputFile.write_int(freqs[i]); //freq  
            }
        }
    }

    inputFile.reset(); //reset to beginning
    //encode input file by reading each byte
    while ((scanByte = inputFile.read_byte()) != -1) {
        unsigned char symbol = static_cast<unsigned char>(scanByte); //cast scanByte to retrieve symbol, store in hmt
        hmt.encode(symbol, outputFile);
    }

    outputFile.flush();//flush remainder bits
    return 0;  
}

