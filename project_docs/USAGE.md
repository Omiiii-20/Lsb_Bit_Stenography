# Usage Guide

## Encode Secret File
```bash
./stego -e input.bmp secret.txt output.bmp
```

- `input.bmp` → Original BMP file.
- `secret.txt` → File to be hidden.
- `output.bmp` → Stego image containing secret data.

## Decode Secret File
```bash
./stego -d output.bmp decoded.txt
```

- `output.bmp` → Stego image containing hidden data.
- `decoded.txt` → File where hidden data is extracted.
