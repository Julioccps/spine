#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "core/decoder.h"

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <input_file> [-t]\n", argv[0]);
		return 1;
	}

	char *input_path = NULL;
	int to_terminal = 0;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-t") == 0) {
			to_terminal = 1;
		} else {
			input_path = argv[i];
		}
	}

	if (!input_path) {
		fprintf(stderr, "Error: No input file specified.\n");
		return 1;
	}

	FILE *in = fopen(input_path, "rb");
	if (!in) {
		perror("Error opening input file");
		return 1;
	}

	FILE *out = stdout;
	char output_path[256];

	if (!to_terminal) {
		// Construct output filename: input_path + ".s"
		snprintf(output_path, sizeof(output_path), "%s.txt", input_path);
		out = fopen(output_path, "w");
		if (!out) {
			perror("Error opening output file");
			fclose(in);
			return 1;
		}
		printf("Disassembling %s to %s...\n", input_path, output_path);
	}

	uint32_t inst;
	uint32_t pc = 0xBFC00000; // Starting PC, could be configurable later

	while (fread(&inst, sizeof(uint32_t), 1, in) == 1) {
		// Handle endianness if necessary. Assuming input is Little Endian (PS2/MIPS)
		// and host is Little Endian (x86/ARM usually).
		// If not, we might need __builtin_bswap32(inst).
		// For now, raw read.

		disassemble(inst, pc, out);
		pc += 4;
	}

	if (!to_terminal) {
		printf("Done.\n");
		fclose(out);
	}
	fclose(in);

	return 0;
}
