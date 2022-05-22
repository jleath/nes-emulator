#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disassembly.h"
#include "nes6502.h"

void print_disassembly(Nes6502 *cpu, char *op, uint8_t cycles) {
    switch (cpu->mode) {
        case Implied:
            printf("%3s", op);
            break;
        case Immediate:
            printf("%3s #$%02x", op, cpu->data);
            break;
        case Accumulator:
            printf("%3s A", op);
            break;
        case ZeroPage:
            printf("%3s $%02x = %02x", op, cpu->absolute_address, cpu->data);
            break;
        case ZeroPageX:
            printf("%3s $%02x,X @ %02x = %02x", op, cpu->indirect_address, cpu->absolute_address & 0xff, cpu->data);
            break;
        case ZeroPageY:
            printf("%3s $%02x,Y @ %02x = %02x", op, cpu->indirect_address, cpu->absolute_address, cpu->data);
            break;
        case Absolute:
            if (strcmp(op, "JMP") == 0 || strcmp(op, "JSR") == 0) {
                printf("%3s $%04x", op, cpu->absolute_address);
            } else {
                printf("%3s $%04x = %02x", op, cpu->absolute_address, cpu->data);
            }
            break;
        case AbsoluteX:
            printf("%3s $%04x,X @ %04x = %02x", op, (cpu->absolute_address-cpu->offset) & 0xffff, cpu->absolute_address, cpu->data);
            break;
        case AbsoluteY:
            printf("%3s $%04x,Y @ %04x = %02x", op, cpu->absolute_address, (cpu->absolute_address+cpu->offset) & 0xffff, cpu->data);
            break;
        case Indirect:
            printf("%3s ($%04x) = %04x", op, cpu->absolute_address, cpu->indirect_address);
            break;
        case IndirectX:
            printf("%3s ($%02x,X) @ %02x = %04x = %02x", op, cpu->absolute_address, (cpu->absolute_address+cpu->offset) & 0xff, cpu->indirect_address, cpu->data);
            break;
        case IndirectY:
            printf("%3s ($%02x),Y = %04x @ %04x = %02x", op, cpu->absolute_address, (cpu->indirect_address-cpu->offset) &0xffff, cpu->indirect_address, cpu->data);
            break;
        case Relative:
            printf("%3s $%04x", op, cpu->absolute_address);
            break;
        default:
            // do nothing
            break;
    }
    printf(" a:%02x x:%02x y:%02x p:%02x sp:%02x cyc:%d\n", cpu->a, cpu->x, cpu->y, cpu->flags, cpu->sp, cpu->total_cycles - cycles);
}