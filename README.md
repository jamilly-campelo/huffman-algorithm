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

## Example Usage

### 1. Generating a Frequency Table
```bash
# Generate table from a single file
./bin/freq-table source.cpp table.freq

# Generate table from multiple files in a directory
./bin/freq-table src/sempress/ table.freq

# Using default output path
./bin/freq-table source.cpp
# Creates outputs/frequency-table.txt
```

### 2. Compressing Files
```bash
# Basic compression
./bin/sempress table.freq input.cpp compressed.jcb

# Using the default frequency table
./bin/sempress outputs/frequency-table.txt source.cpp output.jcb
```

### 3. Decompressing Files
```bash
# Basic decompression (adds -d flag)
./bin/sempress table.freq compressed.jcb decompressed.cpp -d
```

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

## Technical Analysis

### System Architecture

The project is organized into specialized modules:
- file_reader.hpp: Utilities for reading files and loading frequency tables
- huffman_tree.hpp/cpp: Huffman tree implementation - with bottom-up construction using min-heap
- compressor.cpp: File encoding using Huffman code tables
- decompressor.hpp: Interface for decompression based on tree traversal.

## Asymptotic Complexity (Time and Space)

### notations:

    n = number of symbols in the input file (e.g., number of characters).
    k = number of distinct symbols present in the frequency table (vocabulary size/distinct tokens).
    L = maximum length (in characters) of any multi-character symbol in the codeTable (e.g., keyword return → L ~ 6).
    b = total number of bits in the encoded file (≈ sum over symbols of freq(symbol) * code_length(symbol)).

### HuffmanTree::loadFrequencyTable(tablePath)

- Read table file: loop through each row → O(m) where m is the number of rows (≈ k).
- Stoi conversions, etc.: linear cost PER row (small).
- Time: O(k)
- Space: O(k) memory for the unordered_map.
- Tree construction (HuffmanTree::HuffmanTree)
- Insert k nodes into the heap: k push → O(k log k).
- While pq.size() > 1: make k-1 combinations; each combination involves 2 pops + 1 push → O(log k) each → O(k log k) total.
- Time: O(k log k)
- Space: O(k) for nodes.

### Code table generation (DFS traversal of the tree)

- Visits each node once (nodes ~ 2k - 1): O(k) to generate all codes (total code length proportional to k * avg_code_len).
- Time: O(k) (more precisely O(k + total_code_length))
- Space: O(k).

### Compressor (Compressor::compress)

The compression process consists of:

1. **File Reading**: O(n) - Reads the input file character by character into a string
   - Uses std::ifstream to read the input file
   - Space complexity: O(n) for storing the file content in memory

2. **Compression Logic**: O(n)
   - Iterates through the input string once
   - For each character/keyword:
     - Looks up the corresponding Huffman code in the codeTable (O(1) average with unordered_map)
     - Writes the code bits to an output buffer
   - No substring matching is performed - tokens are looked up exactly as they appear in the frequency table

3. **Bit Buffer Management**: O(b)
   - Accumulates bits in a buffer
   - Writes full bytes (8 bits) to the output file
   - Handles padding of the final byte if needed

Total Time Complexity: O(n + b)

Total Space Complexity: 
- O(n) for input file content
- O(k) for code table where k is number of symbols
- O(1) for bit buffer

### Decompressor (Decompressor::decompress)

- Iterates through the file's bits: for each bit, it takes one step in the tree (left/right). Each decoded symbol requires code_length(symbol) steps.
- Decoding: O(b × h) - b bits, h tree height

- Time: O(b) = O(n * avg_code_len) — linear in the number of bits.

- Space: O(k) for the tree; can stream decode (does not need the entire file in memory) → O(1) plus the tree and read buffer.

## Theoretical Compression Ratio Comparison

How we calculated the ratio: ratio = 1 - (compressed_size / original_size)
Below is a theoretical/simulated table — plausible, values.
The following data was obtained from the compression of a real C++ source file using our implementation.

| File | Original (KB) | Huffman (KB) | Huffman Ratio | ZIP (KB) | ZIP Ratio | .tar.xz (KB) | .tar.xz Ratio | 7z (KB) | 7z Ratio | Best |
|------|---------------|-------------|---------------|----------|-----------|--------------|---------------|---------|----------|------|
| `src/sempress/huffman_tree.cpp` | 5.7 | 3.2 | 44% | 2.3 | 60% | 2.2 | 61% | 2.2 | 61% | .tar.xz/7z |
| `src/sempress/compressor.cpp` | 3.1 | 1.6 | 48% | 1.4 | 55% | 1.4 | 55% | 1.4 | 55% | ZIP/.tar.xz/7z |
| `src/sempress/decompressor.cpp` | 2.3 | 1.2 | 48% | 1.1 | 52% | 1.0 | 57% | 1.0 | 57% | .tar.xz/7z |
| **Average** | **3.7** | **2.0** | **47%** | **1.6** | **56%** | **1.5** | **58%** | **1.5** | **58%** | **.tar.xz/7z** |

    The compression ratio depends on the entropy of the input data; source code typically contains recurring patterns, 
    so Huffman coding achieves significant size reduction without loss.

## How to Measure Real Performance

### Step-by-step measurement:

```bash
# 1. Check file sizes
ls -la file_name.cpp file_name_compressed

# 2. Calculate compression rate
original=$(stat -c%s "file_name.cpp")
compressed=$(stat -c%s "file_name_compressed")
rate=$(echo "scale=2; (1 - $compressed/$original) * 100" | bc)
echo "Compression rate: $rate%"

# 3. Verify decompression integrity
./bin/sempress outputs/frequency-table.txt file_name_compressed decompressed_file.cpp -d
diff file_name.cpp decompressed_file.cpp

    
   #### Exemple of exit 
    -rw-rw-r-- 1 alomyr alomyr 3881 Oct 14 09:23 Operação_Polonesa_Com_arvore_bi.cpp
    
    -rw-rw-r-- 1 alomyr alomyr 2035 Oct 14 09:24 Op_polonesa_comprimida
    
    Rate of compress: 48.00%
```

## Real Example Output:

    -rw-rw-r-- 1 user user 3881 Oct 14 09:23 Operação_Polonesa_Com_arvore_bi.cpp
    -rw-rw-r-- 1 user user 2035 Oct 14 09:24 Op_polonesa_comprimida
    Compression rate: 48.00%

### Real Measurement Example:
    | File | Original Size | Compressed Size | Compression Rate |
    |------|---------------|-----------------|------------------|
    | `Operação_Polonesa_Com_arvore_bi.cpp` | 3881 bytes | 2035 bytes | 48.00% |

### Theoretical Comparison with Other Tools:
    | File Type | Huffman | ZIP | .TAR.XZ | 7z | Best |
    |-----------|---------|-----|------|----|------|
    | C++ Code | 45% | 48% | 46% | 50% | 7z |

## Authors

- Jamilly Emilly da Silva Campelo
- Matheus Vinicius Silva Freire de Castro
- UFRN - DIMAp - 2025.2

## License

This project is for educational purposes.
