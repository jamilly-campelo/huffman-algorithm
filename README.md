# Huffman Algorithm Project

This repository contains the implementation of a Huffman compressor and a frequency table generator for C++ source files.  
Developed for the 1st Unit Project of the course "Estruturas de Dados Básicas II" (Basic Data Structures II) - 2025.2 - UFRN - DIMAp.

## Project Structure

```
huffman-algorithm/
├── bin/                # Compiled executables (sempress, freq-table)
├── inputs/             # Input files for frequency analysis (ascii_chars.txt, cpp-keywords.txt)
├── outputs/            # Output files (e.g., frequency-table.txt)
├── src/
│   ├── sempress/       # Huffman compression/decompression source code
│   └── table/          # Frequency table generator source code
├── makefile            # Build automation
└── README.md           # Project documentation
```

## Executables

### 1. Frequency Table Generator (`freq-table`)

Generates a frequency table of C++ keywords and ASCII characters from `.cpp` files.

**Usage:**
```sh
./bin/freq-table <input_file_or_dir> [output_file]
```
- `<input_file_or_dir>`: Path to a `.cpp` file or a directory containing `.cpp` files.
- `[output_file]`: (Optional) Output file path. Defaults to `outputs/frequency-table.txt`.

### 2. Huffman Compressor (`sempress`)

Compresses and decompresses files using the Huffman algorithm.

**Usage:**
```sh
./bin/sempress <table> <input> <output>
```
- `<table>`: Path to the frequency table file (e.g., `outputs/frequency-table.txt`).
- `<input>`: Input file to compress or decompress.
- `<output>`: Output file path.

## Building the Project

To compile the project, run:

```sh
make
```

This will generate the executables in the `bin/` directory.

To clean build artifacts:

```sh
make clean
```

To rebuild everything:

```sh
make rebuild
```

## Input Files

- `inputs/ascii_chars.txt`: List of ASCII characters to track.
- `inputs/cpp-keywords.txt`: List of C++ keywords to track.

## Output Files

- `outputs/frequency-table.txt`: Generated frequency table.

## Authors

- Jamilly Emilly da Silva Campelo
- Matheus Vinicius Silva Freire de Castro
- UFRN - DIMAp - 2025.2

## License

This project is for educational purposes.
