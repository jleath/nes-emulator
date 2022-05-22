#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "disassembly.h"
#include "nes6502.h"

void print_disassembly(Nes6502 *cpu, char *op) {
    switch (cpu->mode) {
        case Implied:
            printf("%3s", op);
            break;
        case Immediate:
            printf("%3s $%02x", op, cpu->data);
            break;
        case Accumulator:
            printf("%3s A", op);
        case ZeroPage:
            printf("%3s $%02x = %02x", op, cpu->absolute_address, cpu->data);
            break;
        case ZeroPageX:
            printf("%3s $00,X @ %02x = %02x", op, cpu->absolute_address, cpu->data);
            break;
        case ZeroPageY:
            printf("%3s $00,Y @ %02x = %02x", op, cpu->absolute_address, cpu->data);
            break;
        case Absolute:
            printf("%3s $%04x = %02x", op, cpu->absolute_address, cpu->data);
            break;
        case AbsoluteX:
            printf("%3s %04x,X @ %04x = %02x", op, cpu->absolute_address-cpu->offset, cpu->absolute_address, cpu->data);
            break;
        case AbsoluteY:
            printf("%3s %04x,Y @ %04x = %02x", op, cpu->absolute_address-cpu->offset, cpu->absolute_address, cpu->data);
            break;
        case Indirect:
            printf("%3s ($%04x) = %04x", op, cpu->absolute_address, cpu->indirect_address);
            break;
        case IndirectX:
            printf("%3s ($%02x,X) @ %02x = %04x = %02x", op, cpu->absolute_address-cpu->offset, cpu->absolute_address, cpu->indirect_address, cpu->data);
            break;
        case IndirectY:
            printf("%3s ($%02x,Y) @ %02x = %04x = %02x", op, cpu->absolute_address-cpu->offset, cpu->absolute_address, cpu->indirect_address, cpu->data);
            break;
        case Relative:
            printf("%3s $%04x", op, cpu->absolute_address);
            break;
    }
}