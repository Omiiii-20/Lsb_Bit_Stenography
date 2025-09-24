Steganography Project (Image Encoder & Decoder)
This project implements a simple steganography tool in C that can encode a secret file into an image and later decode it back.
It uses LSB (Least Significant Bit) encoding on BMP images.

Features
Encode secret text/files into BMP images.
Decode hidden data from stego images.
Works with any BMP file format.
Modular C code with header files (types.h, encode.h, etc.).
Simple CLI usage.
Project Structure
.
├── encode.c        # Encoding logic
├── types.h         # User-defined enums and types
├── common.h        # Magic string definition
├── a.out           # Compiled executable (ignored in git)
├── secret.txt      # Example secret file
└── README.md       # Documentation
Installation
See INSTALLATION.md for full setup.

Quick version:

gcc main.c encode.c -o stego
./stego -e input.bmp secret.txt output.bmp
./stego -d output.bmp decoded.txt
Usage
See USAGE.md for detailed examples.

Encode Example
./stego -e sample.bmp secret.txt stego.bmp
Decode Example
./stego -d stego.bmp output.txt
Example
Input secret: secret.txt
Encoded image: stego.bmp
Decoded output: output.txt
Contribution
See CONTRIBUTING.md.

License
This project is licensed under the MIT License. See LICENSE for details.
