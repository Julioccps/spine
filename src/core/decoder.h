#ifndef DECODER_H
#define DECODER_H

#include <stdint.h>
#include <stdio.h>

void disassemble(uint32_t inst, uint32_t pc, FILE *stream);

#endif
