# File Compression and Uncompression Tool

This project implements a file compression and uncompression tool using the Huffman coding algorithm. The tool reduces the storage size of files without losing any information by encoding file contents into binary sequences and decoding them back to their original form.

## Features
- **Compression:** Compresses an input file into a smaller binary format.
- **Uncompression:** Decompresses the compressed file to restore the original file.
- Supports arbitrary file types with a maximum size of 10 MB.
- Efficient implementation using the Huffman coding algorithm.

## Usage
The project provides two executable programs: `compress` and `uncompress`.

### Compressing a File
```bash
$ ./compress <original_file> <compressed_file>
