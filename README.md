# libbarcode
A small Code 128 barcode scanning, generation, and printing C library.

## Background
This project was started to provide specialised barcode printing and scanning
software with an NGO dentist in Kyrgyzstan operating in partnership with
[Interserve](https://interserve.org.au/).

Its aim is to provide barcode generation from up strings up to 20 characters
long, versatile barcode printing services, and decoding services.

## Roadmap
- [x] Code 128 barcode generation from a 20-character string
- [x] Generate Code 128 barcode SVGs
- [x] Print barcodes (PostScript format)
  - [x] Print in user-defined sizes
  - [x] Print multiple different barcodes on one page
- [ ] Read barcodes from an internal representation
- [ ] Add Java bindings

## Usage
**Note:** Compiled with GNU Make using Clang on macOS and Linux, and MSVC on Windows.

Include the `symb.h` and `graphic.h` headers and initialise with `init_barcode()` before doing any of the following.

### Barcode Generation
To generate the internal representation of a barcode, a Code128 struct, `c128_encode`
accepts an array of `uchar` (`unsigned char`) (*Array, NOT a string* - remove the null terminator),
the length of the array (`int`), and a double pointer to the destination Code128
struct. Memory is allocated during encoding so ideally it should be unassigned.

`c128_svg` accepts a pointer to a Code128 struct containing the internal representation
of the barcode and a pointer to the destination string for the SVG. Memory is allocated
in the function so this should also be unassigned.

The resulting SVG can then be written to file and viewed or used in some other way.

## Example
See `src/main.c` for a PostScript example.

## License
This project is licensed under the Mozilla Public License Version 2.0. See [LICENSE](../blob/master/LICENSE) for more information.
