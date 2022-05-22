#ifndef DISASSEMBLY_H
#define DISASSEMBLY_H

#include <stdint.h>

#include "nes6502.h"

void print_disassembly(Nes6502 *cpu, char *op);

#endif