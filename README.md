# SPINE (Switch PS2 Interface for Native Execution)

**SPINE** is a static translation layer aiming to bring PlayStation 2 software to the Nintendo Switch. The name stands for "Switch PS2 Interface for Native Execution" or recursively "SPine Is Not Emulation".

Currently, the project is in its early stages, featuring a basic MIPS disassembler core.

## Features

- **MIPS Disassembler:** Decodes binary files containing MIPS instructions and outputs assembly code.
- **Output Options:** Supports writing to a `.txt` file or printing directly to the terminal.

## Building

To build the project, ensure you have `gcc` and `make` installed, then run:

```bash
make
```

This will produce the `spine` executable.

## Usage

Run the executable with a binary input file.

### Disassemble to File
By default, SPINE will create a new file with the `.s` extension (e.g., `game.bin.s`) containing the disassembly.

```bash
./spine <input_file>
```

### Disassemble to Terminal
Use the `-t` flag to print the disassembly output directly to stdout.

```bash
./spine <input_file> -t
```

## Structure

- `src/core/`: Core emulation/translation logic (currently decoder).
- `src/utils/`: Utility functions.
- `src/main.c`: Entry point and CLI argument handling.
