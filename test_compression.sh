#!/bin/bash

# colors for output
GREEN="\033[0;32m"
RED="\033[0;31m"
NC="\033[0m" # No color

# test files
ORIGINAL_FILE="test_original.txt"
COMPRESSED_FILE="test_compressed.bin"
UNCOMPRESSED_FILE="test_uncompressed.txt"

# create test input
echo "This is a simple test file." > $ORIGINAL_FILE

# test compress
echo "Running compression..."
./compress $ORIGINAL_FILE $COMPRESSED_FILE
if [ $? -ne 0 ]; then
    echo -e "${RED}Compression failed.${NC}"
    exit 1
else
    echo -e "${GREEN}Compression successful.${NC}"
fi

# test uncompress
echo "Running uncompression..."
./uncompress $COMPRESSED_FILE $UNCOMPRESSED_FILE
if [ $? -ne 0 ]; then
    echo -e "${RED}Uncompression failed.${NC}"
    exit 1
else
    echo -e "${GREEN}Uncompression successful.${NC}"
fi

# verify integrity
echo "Verifying integrity..."
if diff $ORIGINAL_FILE $UNCOMPRESSED_FILE > /dev/null; then
    echo -e "${GREEN}Integrity check passed: Files are identical.${NC}"
else
    echo -e "${RED}Integrity check failed: Files differ.${NC}"
    exit 1
fi

# Clean up
echo "Cleaning up..."
rm -f $ORIGINAL_FILE $COMPRESSED_FILE $UNCOMPRESSED_FILE
echo -e "${GREEN}All tests passed.${NC}"
