# Installation Guide

## Prerequisites
- GCC compiler (Linux/Windows)
- Basic knowledge of C programming
- BMP image file to use as input

## Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/steganography.git
   cd steganography
   ```

2. Compile the code:
   ```bash
   gcc main.c encode.c -o stego
   ```

3. Run encoding:
   ```bash
   ./stego -e input.bmp secret.txt output.bmp
   ```

4. Run decoding:
   ```bash
   ./stego -d output.bmp decoded.txt
   ```
