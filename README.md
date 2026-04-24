  # LSB Image Steganography

## Author

Om Bidikar

---

## About the Project

This project implements an image steganography tool in C using the Least Significant Bit (LSB) technique.

It allows encoding a secret file inside a BMP image and later decoding it back without affecting the visible quality of the image.

---

## Features

* Encode secret data into BMP images
* Decode hidden data from stego images
* Maintains original image quality
* Works with BMP file format
* Modular and structured C implementation

---

## 📁 Project Structure

Lsb_Bit_Steganography/

Source Files:

* src/ (encode.c, decode.c, test_encode.c)

Header Files:

* include/ (encode.h, decode.h, common.h, types.h)

Test Files:

* test_images/ (input image, secret file)

Screenshots:

* screenshots/

Build:

* Makefile (if available)

Documentation:

* README.md

---

## ⚙️ How to Compile

```bash
gcc src/*.c -o stego
```

---

## ▶️ How to Run

### Encode

```bash
./stego -e test_images/input.bmp test_images/secret.txt output.bmp
```

### Decode

```bash
./stego -d output.bmp decoded.txt
```

---

## 🧪 Example

* Input Image: input.bmp
* Secret File: secret.txt
* Output Image: output.bmp
* Decoded File: decoded.txt

---

## 📸 Output Screenshots

### Encoding Process

![Encoding](screenshots/Encoding.png)

### Decoding Process

![Decoding](screenshots/Decoding.png)

### Hidden Secret Text

![Secret](screenshots/Secret_text.png)

---

## 🧠 Concepts Used

* Bit Manipulation
* File Handling (Binary Files)
* BMP File Format
* LSB Encoding Technique
* Modular Programming in C

---

## ⚠️ Limitations

* Supports only BMP images
* No encryption applied
* Limited message size based on image capacity

---

## 💡 How It Works

The Least Significant Bit (LSB) technique hides data by modifying the last bit of each pixel in the image.
Since the change is minimal, it does not affect the visible quality of the image.

---

## 📘 License

This project is for learning purposes.
