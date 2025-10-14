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

## Technical Analysis

### System Architecture

- The project is organized into specialized modules:
file_reader.hpp: Utilities for reading files and loading frequency tables
huffman_tree.hpp/cpp: Huffman tree implementation with bottom-up construction using min-heap
compressor.cpp: File encoding using Huffman code tables
decompressor.hpp: Interface for decompression based on tree traversal.

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

- File reading: O(n) to load all content into memory.
- Symbol-to-code mapping: depends on how the symbol search is performed:
- If the algorithm tries substrings of length up to L at position pos (for example, it tries content.substr(pos, l) for l = L..1), 
then the cost is O(n * L * cost_lookup). With unordered_map, the lookup is on average O(1), so O(n * L).
- If the implementation tests only one character at a time and uses multi-character symbols only if found, the cost is O(n).
- From what was submitted, it appears that the compressor attempts substring matching (to support multi-character tokens). Therefore, Time ≈ O(n * L).

- Conversion to bytes (buffer handle): adding bits and writing bytes is O(b/8) ≈ O(b).
Approximate total time: O(n * L + b). Since b is O(n * avg_code_len) — typically avg_code_len is a constant linked to entropy — the dominant term tends to be O(n * L).

- Space: keeping content in memory → O(n) + codeTable structures O(k).

- Note: if L is small (short keywords) and n is large, the practical complexity is almost linear O(n). However, the naive substring search can be replaced by a trie/prefix-tree that allows for more efficient O(n * max_symbol_length?) and avoids multiple substrs for repeated substrings.

### Decompressor (Decompressor::decompress) — (expected)

- Iterates through the file's bits: for each bit, it takes one step in the tree (left/right). Each decoded symbol requires code_length(symbol) steps.
- Decoding: O(b × h) - b bits, h tree height

- Time: O(b) = O(n * avg_code_len) — linear in the number of bits.

- Space: O(k) for the tree; can stream decode (does not need the entire file in memory) → O(1) plus the tree and read buffer.

## Theoretical Compression Ratio Comparison

How we calculated the ratio:
ratio = 1 - (compressed_size / original_size)
Below is a theoretical/simulated table — plausible, unmeasured values. I include examples of three file types: small code file (main.cpp), medium code project (hypothetical project_src.tar), and large text (text.txt). These numbers are illustrative for the report. At the end of the report, I show how to measure it in practice and how to populate the actual table.

| File (exemple)                     | Original (KB) | Huffman (est.) (KB) | ZIP (est.) | GZIP (est.) | 7z (est.) | Better |
| ------------------------------------- | ------------: | ------------------: | ---------: | ----------: | --------: | ------ |
| `main.cpp` (code)                   |            12 |       6,24  → rate 48% |   7  → 42% |   6.5 → 46% |   6 → 50% | 7z     |

## How to Measure Real Performance

- To get real compression metrics:

        ls -la file_name.cpp file_name_compress

        original=$(stat -c%s "file_name.cpp")
        compress=$(stat -c%s "file_name_compress")
        rate=$(echo "scale=2; (1 - $compress/$original) * 100" | bc)
        echo "rate of compress: $rate%"

    
   #### Exemple of exit 
    -rw-rw-r-- 1 alomyr alomyr 3881 Oct 14 09:23 Operação_Polonesa_Com_arvore_bi.cpp
    
    -rw-rw-r-- 1 alomyr alomyr 2035 Oct 14 09:24 Op_polonesa_comprimida
    
    Rate of compress: 48.00%

## Authors

- Jamilly Emilly da Silva Campelo
- Matheus Vinicius Silva Freire de Castro
- UFRN - DIMAp - 2025.2

## License

This project is for educational purposes.
