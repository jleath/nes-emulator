#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "nes6502.h"
#include "disassembly.h"

Nes6502Instruction operations[] = {
    { "BRK", &BRK, &immediate, 7 },{ "ORA", &ORA, &indirectX, 6 },{ "???", NULL, &implied, 2 },{ "???", NULL, &implied, 8 },{ "???", &NOP, &implied, 3 },{ "ORA", &ORA, &zero_page, 3 },{ "ASL", &ASL, &zero_page, 5 },{ "???", NULL, &implied, 5 },{ "PHP", &PHP, &implied, 3 },{ "ORA", &ORA, &immediate, 2 },{ "ASL", &ASL, &accumulator, 2 },{ "???", NULL, &implied, 2 },{ "???", &NOP, &implied, 4 },{ "ORA", &ORA, &absolute, 4 },{ "ASL", &ASL, &absolute, 6 },{ "???", NULL, &implied, 6 },
    { "BPL", &BPL, &relative, 2 },{ "ORA", &ORA, &indirectY, 5 },{ "???", NULL, &implied, 2 },{ "???", NULL, &implied, 8 },{ "???", &NOP, &implied, 4 },{ "ORA", &ORA, &zero_pageX, 4 },{ "ASL", &ASL, &zero_pageX, 6 },{ "???", NULL, &implied, 6 },{ "CLC", &CLC, &implied, 2 },{ "ORA", &ORA, &absoluteY, 4 },{ "???", &NOP, &implied, 2 },{ "???", NULL, &implied, 7 },{ "???", &NOP, &implied, 4 },{ "ORA", &ORA, &absoluteX, 4 },{ "ASL", &ASL, &absoluteX, 7 },{ "???", NULL, &implied, 7 },
    { "JSR", &JSR, &absolute, 6 },{ "AND", &AND, &indirectX, 6 },{ "???", NULL, &implied, 2 },{ "???", NULL, &implied, 8 },{ "BIT", &BIT, &zero_page, 3 },{ "AND", &AND, &zero_page, 3 },{ "ROL", &ROL, &zero_page, 5 },{ "???", NULL, &implied, 5 },{ "PLP", &PLP, &implied, 4 },{ "AND", &AND, &immediate, 2 },{ "ROL", &ROL, &accumulator, 2 },{ "???", NULL, &implied, 2 },{ "BIT", &BIT, &absolute, 4 },{ "AND", &AND, &absolute, 4 },{ "ROL", &ROL, &absolute, 6 },{ "???", NULL, &implied, 6 },
    { "BMI", &BMI, &relative, 2 },{ "AND", &AND, &indirectY, 5 },{ "???", NULL, &implied, 2 },{ "???", NULL, &implied, 8 },{ "???", &NOP, &implied, 4 },{ "AND", &AND, &zero_pageX, 4 },{ "ROL", &ROL, &zero_pageX, 6 },{ "???", NULL, &implied, 6 },{ "SEC", &SEC, &implied, 2 },{ "AND", &AND, &absoluteY, 4 },{ "???", &NOP, &implied, 2 },{ "???", NULL, &implied, 7 },{ "???", &NOP, &implied, 4 },{ "AND", &AND, &absoluteX, 4 },{ "ROL", &ROL, &absoluteX, 7 },{ "???", NULL, &implied, 7 },
    { "RTI", &RTI, &implied, 6 },{ "EOR", &EOR, &indirectX, 6 },{ "???", NULL, &implied, 2 },{ "???", NULL, &implied, 8 },{ "???", &NOP, &implied, 3 },{ "EOR", &EOR, &zero_page, 3 },{ "LSR", &LSR, &zero_page, 5 },{ "???", NULL, &implied, 5 },{ "PHA", &PHA, &implied, 3 },{ "EOR", &EOR, &immediate, 2 },{ "LSR", &LSR, &accumulator, 2 },{ "???", NULL, &implied, 2 },{ "JMP", &JMP, &absolute, 3 },{ "EOR", &EOR, &absolute, 4 },{ "LSR", &LSR, &absolute, 6 },{ "???", NULL, &implied, 6 },
    { "BVC", &BVC, &relative, 2 },{ "EOR", &EOR, &indirectY, 5 },{ "???", NULL, &implied, 2 },{ "???", NULL, &implied, 8 },{ "???", &NOP, &implied, 4 },{ "EOR", &EOR, &zero_pageX, 4 },{ "LSR", &LSR, &zero_pageX, 6 },{ "???", NULL, &implied, 6 },{ "CLI", &CLI, &implied, 2 },{ "EOR", &EOR, &absoluteY, 4 },{ "???", &NOP, &implied, 2 },{ "???", NULL, &implied, 7 },{ "???", &NOP, &implied, 4 },{ "EOR", &EOR, &absoluteX, 4 },{ "LSR", &LSR, &absoluteX, 7 },{ "???", NULL, &implied, 7 },
    { "RTS", &RTS, &implied, 6 },{ "ADC", &ADC, &indirectX, 6 },{ "???", NULL, &implied, 2 },{ "???", NULL, &implied, 8 },{ "???", &NOP, &implied, 3 },{ "ADC", &ADC, &zero_page, 3 },{ "ROR", &ROR, &zero_page, 5 },{ "???", NULL, &implied, 5 },{ "PLA", &PLA, &implied, 4 },{ "ADC", &ADC, &immediate, 2 },{ "ROR", &ROR, &accumulator, 2 },{ "???", NULL, &implied, 2 },{ "JMP", &JMP, &indirect, 5 },{ "ADC", &ADC, &absolute, 4 },{ "ROR", &ROR, &absolute, 6 },{ "???", NULL, &implied, 6 },
    { "BVS", &BVS, &relative, 2 },{ "ADC", &ADC, &indirectY, 5 },{ "???", NULL, &implied, 2 },{ "???", NULL, &implied, 8 },{ "???", &NOP, &implied, 4 },{ "ADC", &ADC, &zero_pageX, 4 },{ "ROR", &ROR, &zero_pageX, 6 },{ "???", NULL, &implied, 6 },{ "SEI", &SEI, &implied, 2 },{ "ADC", &ADC, &absoluteY, 4 },{ "???", &NOP, &implied, 2 },{ "???", NULL, &implied, 7 },{ "???", &NOP, &implied, 4 },{ "ADC", &ADC, &absoluteX, 4 },{ "ROR", &ROR, &absoluteX, 7 },{ "???", NULL, &implied, 7 },
    { "???", &NOP, &implied, 2 },{ "STA", &STA, &indirectX, 6 },{ "???", &NOP, &implied, 2 },{ "???", NULL, &implied, 6 },{ "STY", &STY, &zero_page, 3 },{ "STA", &STA, &zero_page, 3 },{ "STX", &STX, &zero_page, 3 },{ "???", NULL, &implied, 3 },{ "DEY", &DEY, &implied, 2 },{ "???", &NOP, &implied, 2 },{ "TXA", &TXA, &implied, 2 },{ "???", NULL, &implied, 2 },{ "STY", &STY, &absolute, 4 },{ "STA", &STA, &absolute, 4 },{ "STX", &STX, &absolute, 4 },{ "???", NULL, &implied, 4 },
    { "BCC", &BCC, &relative, 2 },{ "STA", &STA, &indirectY, 6 },{ "???", NULL, &implied, 2 },{ "???", NULL, &implied, 6 },{ "STY", &STY, &zero_pageX, 4 },{ "STA", &STA, &zero_pageX, 4 },{ "STX", &STX, &zero_pageY, 4 },{ "???", NULL, &implied, 4 },{ "TYA", &TYA, &implied, 2 },{ "STA", &STA, &absoluteY, 5 },{ "TXS", &TXS, &implied, 2 },{ "???", NULL, &implied, 5 },{ "???", &NOP, &implied, 5 },{ "STA", &STA, &absoluteX, 5 },{ "???", NULL, &implied, 5 },{ "???", NULL, &implied, 5 },
    { "LDY", &LDY, &immediate, 2 },{ "LDA", &LDA, &indirectX, 6 },{ "LDX", &LDX, &immediate, 2 },{ "???", NULL, &implied, 6 },{ "LDY", &LDY, &zero_page, 3 },{ "LDA", &LDA, &zero_page, 3 },{ "LDX", &LDX, &zero_page, 3 },{ "???", NULL, &implied, 3 },{ "TAY", &TAY, &implied, 2 },{ "LDA", &LDA, &immediate, 2 },{ "TAX", &TAX, &implied, 2 },{ "???", NULL, &implied, 2 },{ "LDY", &LDY, &absolute, 4 },{ "LDA", &LDA, &absolute, 4 },{ "LDX", &LDX, &absolute, 4 },{ "???", NULL, &implied, 4 },
    { "BCS", &BCS, &relative, 2 },{ "LDA", &LDA, &indirectY, 5 },{ "???", NULL, &implied, 2 },{ "???", NULL, &implied, 5 },{ "LDY", &LDY, &zero_pageX, 4 },{ "LDA", &LDA, &zero_pageX, 4 },{ "LDX", &LDX, &zero_pageY, 4 },{ "???", NULL, &implied, 4 },{ "CLV", &CLV, &implied, 2 },{ "LDA", &LDA, &absoluteY, 4 },{ "TSX", &TSX, &implied, 2 },{ "???", NULL, &implied, 4 },{ "LDY", &LDY, &absoluteX, 4 },{ "LDA", &LDA, &absoluteX, 4 },{ "LDX", &LDX, &absoluteY, 4 },{ "???", NULL, &implied, 4 },
    { "CPY", &CPY, &immediate, 2 },{ "CMP", &CMP, &indirectX, 6 },{ "???", &NOP, &implied, 2 },{ "???", NULL, &implied, 8 },{ "CPY", &CPY, &zero_page, 3 },{ "CMP", &CMP, &zero_page, 3 },{ "DEC", &DEC, &zero_page, 5 },{ "???", NULL, &implied, 5 },{ "INY", &INY, &implied, 2 },{ "CMP", &CMP, &immediate, 2 },{ "DEX", &DEX, &implied, 2 },{ "???", NULL, &implied, 2 },{ "CPY", &CPY, &absolute, 4 },{ "CMP", &CMP, &absolute, 4 },{ "DEC", &DEC, &absolute, 6 },{ "???", NULL, &implied, 6 },
    { "BNE", &BNE, &relative, 2 },{ "CMP", &CMP, &indirectY, 5 },{ "???", NULL, &implied, 2 },{ "???", NULL, &implied, 8 },{ "???", &NOP, &implied, 4 },{ "CMP", &CMP, &zero_pageX, 4 },{ "DEC", &DEC, &zero_pageX, 6 },{ "???", NULL, &implied, 6 },{ "CLD", &CLD, &implied, 2 },{ "CMP", &CMP, &absoluteY, 4 },{ "NOP", &NOP, &implied, 2 },{ "???", NULL, &implied, 7 },{ "???", &NOP, &implied, 4 },{ "CMP", &CMP, &absoluteX, 4 },{ "DEC", &DEC, &absoluteX, 7 },{ "???", NULL, &implied, 7 },
    { "CPX", &CPX, &immediate, 2 },{ "SBC", &SBC, &indirectX, 6 },{ "???", &NOP, &implied, 2 },{ "???", NULL, &implied, 8 },{ "CPX", &CPX, &zero_page, 3 },{ "SBC", &SBC, &zero_page, 3 },{ "INC", &INC, &zero_page, 5 },{ "???", NULL, &implied, 5 },{ "INX", &INX, &implied, 2 },{ "SBC", &SBC, &immediate, 2 },{ "NOP", &NOP, &implied, 2 },{ "???", &SBC, &implied, 2 },{ "CPX", &CPX, &absolute, 4 },{ "SBC", &SBC, &absolute, 4 },{ "INC", &INC, &absolute, 6 },{ "???", NULL, &implied, 6 },
    { "BEQ", &BEQ, &relative, 2 },{ "SBC", &SBC, &indirectY, 5 },{ "???", NULL, &implied, 2 },{ "???", NULL, &implied, 8 },{ "???", &NOP, &implied, 4 },{ "SBC", &SBC, &zero_pageX, 4 },{ "INC", &INC, &zero_pageX, 6 },{ "???", NULL, &implied, 6 },{ "SED", &SED, &implied, 2 },{ "SBC", &SBC, &absoluteY, 4 },{ "NOP", &NOP, &implied, 2 },{ "???", NULL, &implied, 7 },{ "???", &NOP, &implied, 4 },{ "SBC", &SBC, &absoluteX, 4 },{ "INC", &INC, &absoluteX, 7 },{ "???", NULL, &implied, 7 },
};

uint8_t get_flag(Nes6502 *cpu, uint8_t flag) {
    return (cpu->flags & flag) != 0;
}

void set_flags(Nes6502 *cpu, uint8_t flag, int value) {
    if (value) {
        cpu->flags |= flag;
    } else {
        cpu->flags &= ~flag;
    }
}

void reset(Nes6502 *cpu) {
    cpu->a = 0;
    cpu->x = 0;
    cpu->y = 0;
    cpu->sp = 0xfd;
    cpu->flags = 0x00;
    uint8_t low = read(cpu->bus, 0xfffc);
    uint8_t high = read(cpu->bus, 0xfffd);
    cpu->pc = (high << 8) | low;
    cpu->cycles = 8;
    cpu->total_cycles += 8;
    cpu->mode = None;
    cpu->indirect_address = 0;
    cpu->absolute_address = 0;
    cpu->offset = 0;
}

int complete(Nes6502 *cpu) {
    return cpu->cycles == 0;
}

void interrupt(Nes6502 *cpu) {
    if (get_flag(cpu, INTERRUPT_DISABLE_FLAG) == 0) {
        write(cpu->bus, 0x0100 + cpu->sp, (cpu->pc >> 8) & 0x00ff);
        (cpu->sp)--;
        write(cpu->bus, 0x0100 + cpu->sp, cpu->pc & 0x00ff);
        (cpu->sp)--;
        set_flags(cpu, BREAK_FLAG, 0);
        set_flags(cpu, INTERRUPT_DISABLE_FLAG, 1);
        write(cpu->bus, 0x0100 + cpu->sp, cpu->flags);
        (cpu->sp)--;

        uint8_t low = read(cpu->bus, 0xfffe);
        uint8_t high = read(cpu->bus, 0xffff);
        cpu->pc = (high << 8) | low;
        cpu->cycles = 7;
        cpu->total_cycles += 7;
    }
}

void non_maskable_interrupt(Nes6502 *cpu) {
    write(cpu->bus, 0x0100 + cpu->sp, (cpu->pc >> 8) & 0x00ff);
    (cpu->sp)--;
    write(cpu->bus, 0x0100 + cpu->sp, cpu->pc & 0x00ff);
    (cpu->sp)--;
    set_flags(cpu, BREAK_FLAG, 0);
    set_flags(cpu, INTERRUPT_DISABLE_FLAG, 1);
    write(cpu->bus, 0x0100 + cpu->sp, cpu->flags);
    (cpu->sp)--;

    uint8_t low = read(cpu->bus, 0xfffa);
    uint8_t high = read(cpu->bus, 0xfffb);
    cpu->pc = (high << 8) | low;
    cpu->cycles = 8;
    cpu->total_cycles += 8;
}

void cycle(Nes6502 *cpu) {
    if (cpu->cycles <= 0) {
        uint8_t opcode = read(cpu->bus, cpu->pc);
        printf("%04x ", cpu->pc);
        (cpu->pc)++;
        Nes6502Instruction op = operations[opcode];
        if (op.operation != NULL) {
            op.mode(cpu);
            cpu->cycles = op.cycles;
            cpu->total_cycles += op.cycles;
            print_disassembly(cpu, op.opcode, op.cycles);
            if (cpu->add_cycles) {
                cpu->cycles += cpu->add_cycles;
                cpu->total_cycles += cpu->add_cycles;
                cpu->add_cycles = 0;
            }
            op.operation(cpu);
        } else {
            printf("Invalid opcode %02x\n", opcode);
        }
    }
    (cpu->cycles)--;
}

Bus *create_bus(int memory_size) {
    Bus *new_bus = (Bus*)malloc(sizeof(Bus));
    new_bus->ram = (uint8_t*)malloc(sizeof(uint8_t) * memory_size);
    for (int i = 0; i < memory_size; i++) {
        new_bus->ram[i] = 0;
    }
    return new_bus;
}

void destroy_bus(Bus *bus) {
    free(bus->ram);
    free(bus);
}

void write(Bus *bus, uint16_t address, uint8_t data) {
    if (address >= 0x0000 && address <= 0xffff) {
        bus->ram[address] = data;
    }
}

uint8_t read(Bus *bus, uint16_t address) {
    if (address >= 0x0000 && address <= 0xffff) {
        return bus->ram[address];
    }
}

// 6502 Addressing Modes
void accumulator(Nes6502 *cpu) {
    cpu->mode = Accumulator;
    cpu->data = cpu->a;
}

void implied(Nes6502 *cpu) {
    cpu->mode = Implied;
}

void immediate(Nes6502 *cpu) {
    cpu->mode = Immediate;
    uint8_t data = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    cpu->data = data;
}

void zero_page(Nes6502 *cpu) {
    cpu->mode = ZeroPage;
    uint8_t address = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    cpu->absolute_address = 0x00ff & address;
    cpu->data = read(cpu->bus, cpu->absolute_address);
}

void zero_pageX(Nes6502 *cpu) {
    cpu->mode = ZeroPageX;
    cpu->offset = cpu->x;
    cpu->indirect_address = (read(cpu->bus, cpu->pc));
    cpu->absolute_address = (cpu->indirect_address + cpu->offset) & 0x00ff;
    (cpu->pc)++;
    cpu->data = read(cpu->bus, cpu->absolute_address);
}

void zero_pageY(Nes6502 *cpu) {
    cpu->mode = ZeroPageY;
    cpu->offset = cpu->y;
    cpu->indirect_address = (read(cpu->bus, cpu->pc));
    cpu->absolute_address = (cpu->indirect_address + cpu->offset) & 0x00ff;
    (cpu->pc)++;
    cpu->data = read(cpu->bus, cpu->absolute_address);
}

void relative(Nes6502 *cpu) {
    cpu->mode = Relative;
    uint8_t offset = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    if (offset & 0x80) {
        cpu->absolute_address = cpu->pc - offset;
    } else {
        cpu->absolute_address = cpu->pc + offset;
    }
    cpu->data = read(cpu->bus, cpu->absolute_address);
}

void absolute(Nes6502 *cpu) {
    cpu->mode = Absolute;
    uint16_t low = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    uint16_t high = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    cpu->absolute_address = (high << 8) | low;
    cpu->data = read(cpu->bus, cpu->absolute_address);
}

void absoluteX(Nes6502 *cpu) {
    cpu->mode = AbsoluteX;
    cpu->offset = cpu->x;
    uint16_t low = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    uint16_t high = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    cpu->absolute_address = (((high << 8) | low) + cpu->x) & 0xffff;
    if ((cpu->absolute_address & 0xff00) != (high << 8) && read(cpu->bus, cpu->pc - 3) != 0x9d) {
        cpu->add_cycles = 1;
    }
    cpu->data = read(cpu->bus, cpu->absolute_address);
}

void absoluteY(Nes6502 *cpu) {
    cpu->mode = AbsoluteY;
    cpu->offset = cpu->y;
    uint16_t low = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    uint16_t high = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    cpu->absolute_address = (high << 8) | low;
    if (((cpu->absolute_address + cpu->y) & 0xff00) != (high << 8)) {
        cpu->add_cycles = 1;
    }
    cpu->data = read(cpu->bus, cpu->absolute_address + cpu->y);
}

// Have to emulate page boundary bug in hardware
void indirect(Nes6502 *cpu) {
    cpu->mode = Indirect;
    uint16_t low = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    uint16_t high = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    cpu->absolute_address = (high << 8) | low;
    if (low == 0x00ff) {
        high = read(cpu->bus, cpu->absolute_address & 0xff00);
        low = read(cpu->bus, cpu->absolute_address);
        cpu->indirect_address = (high << 8) | low;
    } else {
        high = read(cpu->bus, cpu->absolute_address + 1);
        low = read(cpu->bus, cpu->absolute_address);
        cpu->indirect_address = (high << 8) | low;
    }
    cpu->data = read(cpu->bus, cpu->indirect_address);
}

void indirectX(Nes6502 *cpu) {
    cpu->mode = IndirectX;
    cpu->offset = cpu->x;
    uint16_t x = (uint16_t)cpu->x;
    cpu->absolute_address = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    uint16_t low = read(cpu->bus, (cpu->absolute_address + x) & 0x00ff);
    uint16_t high = read(cpu->bus, (cpu->absolute_address + x + 1) & 0x00ff);
    cpu->indirect_address = (high << 8) | low;
    cpu->data = read(cpu->bus, cpu->indirect_address);
}

void indirectY(Nes6502 *cpu) {
    cpu->mode = IndirectY;
    cpu->offset = cpu->y;
    cpu->absolute_address = read(cpu->bus, cpu->pc);
    uint16_t y = (uint16_t)cpu->y;
    (cpu->pc)++;
    uint16_t low = read(cpu->bus, cpu->absolute_address & 0x00ff);
    uint16_t high = read(cpu->bus, (cpu->absolute_address + 1) & 0x00ff);
    cpu->indirect_address = ((high << 8) | low) + y;
    if ((cpu->indirect_address & 0xff00) != (high << 8)) {
        cpu->add_cycles = 1;
    }
    cpu->data = read(cpu->bus, cpu->indirect_address);
}

// 6502 OPERATIONS
void ADC(Nes6502 *cpu) {
    uint16_t a = cpu->a;
    uint16_t addend = cpu->data;
    uint16_t new_value = addend + a;
    new_value += (uint16_t)get_flag(cpu, CARRY_FLAG);
    set_flags(cpu, CARRY_FLAG, (new_value & 0xff00) != 0);
    set_flags(cpu, ZERO_FLAG, (new_value & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x80) != 0);
    set_flags(cpu, OVERFLOW_FLAG, (((a ^ new_value) & (addend ^ new_value)) & 0x0080) != 0);
    cpu->a = (new_value & 0x00ff);
}

void AND(Nes6502 *cpu) {
    uint8_t new_value = cpu->a & cpu->data;
    set_flags(cpu, ZERO_FLAG, (new_value & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x80) != 0);
    cpu->a = new_value;
}

void ASL(Nes6502 *cpu) {
    uint8_t new_value;
    if (cpu->mode == Accumulator) {
        set_flags(cpu, CARRY_FLAG, (cpu->a & 0x80) != 0);
        new_value = cpu->a << 1;
        cpu->a = new_value;
    } else {
        set_flags(cpu, CARRY_FLAG, (cpu->data & 0x80) != 0);
        new_value = cpu->data << 1;
        write(cpu->bus, cpu->absolute_address, new_value);
    }
    set_flags(cpu, ZERO_FLAG, (new_value & 0xff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x80) != 0);
}

void BCC(Nes6502 *cpu) {
    if (!get_flag(cpu, CARRY_FLAG)) {
        uint16_t new_address = cpu->absolute_address;
        (cpu->cycles)++;
        (cpu->total_cycles)++;
        if ((new_address & 0xff00) != (cpu->pc & 0xff00)) {
            (cpu->cycles)++;
            (cpu->total_cycles)++;
        }
        cpu->pc = new_address;
    }
}

void BCS(Nes6502 *cpu) {
    if (get_flag(cpu, CARRY_FLAG)) {
        uint16_t new_address = (uint16_t)cpu->absolute_address;
        (cpu->cycles)++;
        (cpu->total_cycles)++;
        if ((new_address & 0xff00) != (cpu->pc & 0xff00)) {
            (cpu->cycles)++;
            (cpu->total_cycles)++;
        }
        cpu->pc = new_address;
    }
}

void BEQ(Nes6502 *cpu) {
    if (get_flag(cpu, ZERO_FLAG)) {
        uint16_t new_address = cpu->absolute_address;
        (cpu->cycles)++;
        (cpu->total_cycles)++;
        if ((new_address & 0xff00) != (cpu->pc & 0xff00)) {
            (cpu->cycles)++;
            (cpu->total_cycles)++;
        }
        cpu->pc = new_address;
    }
}

void BIT(Nes6502 *cpu) {
    set_flags(cpu, ZERO_FLAG, (cpu->a & cpu->data) == 0);
    set_flags(cpu, OVERFLOW_FLAG, (cpu->data & 0x40) != 0);
    set_flags(cpu, NEGATIVE_FLAG, (cpu->data & 0x80) != 0);
}

void BMI(Nes6502 *cpu) {
    if (get_flag(cpu, NEGATIVE_FLAG)) {
        uint16_t new_address = cpu->absolute_address;
        (cpu->cycles)++;
        (cpu->total_cycles)++;
        if ((new_address & 0xff00) != (cpu->pc & 0xff00)) {
            (cpu->cycles)++;
            (cpu->total_cycles)++;
        }
        cpu->pc = new_address;
    }
}

void BNE(Nes6502 *cpu) {
    if (!get_flag(cpu, ZERO_FLAG)) {
        uint16_t new_address = cpu->absolute_address;
        (cpu->cycles)++;
        (cpu->total_cycles)++;
        if ((new_address & 0xff00) != (cpu->pc & 0xff00)) {
            (cpu->cycles)++;
            (cpu->total_cycles)++;
        }
        cpu->pc = new_address;
    }
}

void BPL(Nes6502 *cpu) {
    if (!get_flag(cpu, NEGATIVE_FLAG)) {
        uint16_t new_address = cpu->absolute_address;
        (cpu->cycles)++;
        (cpu->total_cycles)++;
        if ((new_address & 0xff00) != (cpu->pc & 0xff00)) {
            (cpu->cycles)++;
            (cpu->total_cycles)++;
        }
        cpu->pc = new_address;
    }
}

void BRK(Nes6502 *cpu) {
    set_flags(cpu, INTERRUPT_DISABLE_FLAG, 1);
    set_flags(cpu, BREAK_FLAG, 1);
    write(cpu->bus, 0x0100 + cpu->sp, (cpu->pc >> 8) & 0x00ff);
    (cpu->sp)--;
    write(cpu->bus, 0x0100 + cpu->sp, cpu->pc & 0x00ff);
    (cpu->sp)--;
    write(cpu->bus, 0x0100 + cpu->sp, cpu->flags);
    (cpu->sp)--;
    set_flags(cpu, BREAK_FLAG, 0);
    cpu->pc = (uint16_t)read(cpu->bus, 0xfffe) | ((uint16_t)read(cpu->bus, 0xffff) << 8);
}

void BVC(Nes6502 *cpu) {
    if (!get_flag(cpu, OVERFLOW_FLAG)) {
        uint16_t new_address = cpu->absolute_address;
        (cpu->cycles)++;
        (cpu->total_cycles)++;
        if ((new_address & 0xff00) != (cpu->pc & 0xff00)) {
            (cpu->cycles)++;
            (cpu->total_cycles)++;
        }
        cpu->pc = new_address;
    }
}

void BVS(Nes6502 *cpu) {
    if (get_flag(cpu, OVERFLOW_FLAG)) {
        uint16_t new_address = cpu->absolute_address;
        (cpu->cycles)++;
        (cpu->total_cycles)++;
        if ((new_address & 0xff00) != (cpu->pc & 0xff00)) {
            (cpu->cycles)++;
            (cpu->total_cycles)++;
        }
        cpu->pc = new_address;
    }
}

void CLC(Nes6502 *cpu) {
    set_flags(cpu, CARRY_FLAG, 0);
}

void CLD(Nes6502 *cpu) {
    set_flags(cpu, DECIMAL_FLAG, 0);
}

void CLI(Nes6502 *cpu) {
    set_flags(cpu, INTERRUPT_DISABLE_FLAG, 0);
}

void CLV(Nes6502 *cpu) {
    set_flags(cpu, OVERFLOW_FLAG, 0);
}

void CMP(Nes6502 *cpu) {
    uint16_t comp_result = (uint16_t)cpu->a - cpu->data;
    set_flags(cpu, CARRY_FLAG, cpu->a >= cpu->data);
    set_flags(cpu, ZERO_FLAG, (comp_result & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, comp_result & 0x0080);
}

void CPX(Nes6502 *cpu) {
    uint16_t comp_result = (uint16_t)cpu->x - cpu->data;
    set_flags(cpu, CARRY_FLAG, cpu->x >= cpu->data);
    set_flags(cpu, ZERO_FLAG, (comp_result & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, comp_result & 0x0080);
}

void CPY(Nes6502 *cpu) {
    uint16_t comp_result = (uint16_t)cpu->y - cpu->data;
    set_flags(cpu, CARRY_FLAG, cpu->y >= cpu->data);
    set_flags(cpu, ZERO_FLAG, (comp_result & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (comp_result & 0x0080) != 0);
}

void DEC(Nes6502 *cpu) {
    uint16_t new_value = cpu->data - 1;
    set_flags(cpu, ZERO_FLAG, (new_value & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x0080) != 0);
    write(cpu->bus, cpu->absolute_address, new_value & 0x00ff);
}

void DEX(Nes6502 *cpu) {
    uint16_t new_value = cpu->x - 1;
    set_flags(cpu, ZERO_FLAG, (new_value & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x0080) != 0);
    cpu->x = new_value & 0x00ff;
}

void DEY(Nes6502 *cpu) {
    uint16_t new_value = cpu->y - 1;
    set_flags(cpu, ZERO_FLAG, (new_value & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x0080) != 0);
    cpu->y = new_value & 0x00ff;
}

void EOR(Nes6502 *cpu) {
    uint8_t new_value = cpu->a ^ cpu->data;
    set_flags(cpu, ZERO_FLAG, (new_value & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x80) != 0);
    cpu->a = new_value;
}

void INC(Nes6502 *cpu) {
    uint16_t new_value = cpu->data + 1;
    set_flags(cpu, ZERO_FLAG, (new_value & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x0080) != 0);
    write(cpu->bus, cpu->absolute_address, new_value & 0x00ff);
}

void INX(Nes6502 *cpu) {
    uint16_t new_value = cpu->x + 1;
    set_flags(cpu, ZERO_FLAG, (new_value & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x0080) != 0);
    cpu->x = new_value & 0x00ff;
}

void INY(Nes6502 *cpu) {
    uint16_t new_value = cpu->y + 1;
    set_flags(cpu, ZERO_FLAG, (new_value & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x0080) != 0);
    cpu->y = new_value & 0x00ff;
}

void JMP(Nes6502 *cpu) {
    if (cpu->mode == Indirect) {
        cpu->pc = cpu->indirect_address;
    } else {
        cpu->pc = cpu->absolute_address;
    }
}

void JSR(Nes6502 *cpu) {
    (cpu->pc)--;
    write(cpu->bus, 0x0100 + cpu->sp, (cpu->pc >> 8) & 0x00ff);
    (cpu->sp)--;
    write(cpu->bus, 0x0100 + cpu->sp, cpu->pc & 0x00ff);
    (cpu->sp)--;

    cpu->pc = cpu->absolute_address;
}

void LDA(Nes6502 *cpu) {
    cpu->a = cpu->data;
    set_flags(cpu, ZERO_FLAG, cpu->a == 0);
    set_flags(cpu, NEGATIVE_FLAG, (cpu->a & 0x80) != 0);
}

void LDX(Nes6502 *cpu) {
    cpu->x = cpu->data;
    set_flags(cpu, ZERO_FLAG, cpu->x == 0);
    set_flags(cpu, NEGATIVE_FLAG, (cpu->x & 0x80) != 0);
}

void LDY(Nes6502 *cpu) {
    cpu->y = cpu->data;
    set_flags(cpu, ZERO_FLAG, cpu->y == 0);
    set_flags(cpu, NEGATIVE_FLAG, (cpu->y & 0x80) != 0);
}

void LSR(Nes6502 *cpu) {
    uint8_t new_value;
    if (cpu->mode == Accumulator) {
        set_flags(cpu, CARRY_FLAG, (cpu->a & 0x01) != 0);
        new_value = cpu->a >> 1;
        new_value = new_value & 0x7f;
        cpu->a = new_value;
    } else {
        set_flags(cpu, CARRY_FLAG, (cpu->data & 0x01) != 0);
        new_value = cpu->data >> 1;
        new_value = new_value & 0x7f;
        write(cpu->bus, cpu->absolute_address, new_value);
    }
    set_flags(cpu, ZERO_FLAG, (new_value & 0xff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x80) != 0);
}

void NOP(Nes6502 *cpu) {
    return;
}

void ORA(Nes6502 *cpu) {
    uint8_t new_value = cpu->a | cpu->data;
    set_flags(cpu, ZERO_FLAG, new_value == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x80) != 0);
    cpu->a = new_value;
}

void PHA(Nes6502 *cpu) {
    write(cpu->bus, 0x0100 + cpu->sp, cpu->a);
    (cpu->sp)--;
}

void PHP(Nes6502 *cpu) {
    // Break flag is set to 1 before the push to the stack
    set_flags(cpu, BREAK_FLAG, 1);
    write(cpu->bus, 0x0100 + cpu->sp, cpu->flags | UNUSED_FLAG);
    (cpu->sp)--;
}

void PLA(Nes6502 *cpu) {
    (cpu->sp)++;
    uint8_t data = read(cpu->bus, 0x0100 + cpu->sp);
    set_flags(cpu, ZERO_FLAG, data == 0);
    set_flags(cpu, NEGATIVE_FLAG, (data & 0x80) != 0);
    cpu->a = data;
}

void PLP(Nes6502 *cpu) {
    (cpu->sp)++;
    uint8_t break_flag = get_flag(cpu, BREAK_FLAG);
    uint8_t unused_flag = get_flag(cpu, UNUSED_FLAG);
    uint8_t data = read(cpu->bus, 0x0100 + cpu->sp);
    cpu->flags = data;
    set_flags(cpu, BREAK_FLAG, break_flag);
    set_flags(cpu, UNUSED_FLAG, unused_flag);
}

void ROL(Nes6502 *cpu) {
    uint8_t old_carry = get_flag(cpu, CARRY_FLAG);
    uint8_t new_carry;
    uint8_t new_value;
    if (cpu->mode == Accumulator) {
        new_carry = ((cpu->a & 0x80) >> 7) & 0x1;
        cpu->a = cpu->a << 1;
        cpu->a = cpu->a | old_carry;
        new_value = cpu->a;
    } else {
        new_carry = ((cpu->data & 0x80) >> 7) & 0x1;
        new_value = cpu->data << 1;
        new_value = new_value | old_carry;
        write(cpu->bus, cpu->absolute_address, new_value);
    }
    set_flags(cpu, ZERO_FLAG, new_value == 0);
    set_flags(cpu, CARRY_FLAG, new_carry);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x80) != 0);
}

void ROR(Nes6502 *cpu) {
    uint8_t old_carry = get_flag(cpu, CARRY_FLAG);
    uint8_t new_carry;
    uint8_t new_value;
    if (cpu->mode == Accumulator) {
        new_carry = cpu->a & 0x1;
        cpu->a = cpu->a >> 1;
        cpu->a = cpu->a | (old_carry << 7);
        new_value = cpu->a;
    } else {
        new_carry = cpu->data & 0x1;
        new_value = cpu->data >> 1;
        new_value = new_value | (old_carry << 7);
        write(cpu->bus, cpu->absolute_address, new_value);
    }
    set_flags(cpu, ZERO_FLAG, new_value == 0);
    set_flags(cpu, CARRY_FLAG, new_carry);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x80) != 0);
}

void RTI(Nes6502 *cpu) {
    (cpu->sp)++;
    uint8_t flags = read(cpu->bus, 0x0100 + cpu->sp);
    (cpu->sp)++;
    uint16_t low = read(cpu->bus, 0x0100 + cpu->sp);
    (cpu->sp)++;
    uint16_t high = read(cpu->bus, 0x0100 + cpu->sp);
    uint8_t break_flag = get_flag(cpu, BREAK_FLAG);
    uint8_t unused_flag = get_flag(cpu, UNUSED_FLAG);
    cpu->flags = flags;
    set_flags(cpu, BREAK_FLAG, break_flag);
    set_flags(cpu, UNUSED_FLAG, unused_flag);
    cpu->pc = low | (high << 8);
}

void RTS(Nes6502 *cpu) {
    (cpu->sp)++;
    uint8_t low = read(cpu->bus, 0x0100 + cpu->sp);
    (cpu->sp)++;
    uint8_t high = read(cpu->bus, 0x0100 + cpu->sp);
    cpu->pc = low | (high << 8);
    (cpu->pc)++;
}

void SBC(Nes6502 *cpu) {
    uint16_t a = (uint16_t)cpu->a;
    uint16_t to_subtract = (uint16_t)cpu->data ^ 0x00ff;
    uint16_t new_value = a + to_subtract + (uint16_t)get_flag(cpu, CARRY_FLAG);
    set_flags(cpu, CARRY_FLAG, new_value & 0xff00);
    set_flags(cpu, ZERO_FLAG, (new_value & 0x00ff) == 0);
    set_flags(cpu, OVERFLOW_FLAG, (new_value ^ a) & (new_value ^ to_subtract) & 0x0080);
    set_flags(cpu, NEGATIVE_FLAG, new_value & 0x0080);
    cpu->a = new_value & 0x00ff;
}

void SEC(Nes6502 *cpu) {
    set_flags(cpu, CARRY_FLAG, 1);
}

void SED(Nes6502 *cpu) {
    set_flags(cpu, DECIMAL_FLAG, 1);
}

void SEI(Nes6502 *cpu) {
    set_flags(cpu, INTERRUPT_DISABLE_FLAG, 1);
}

void STA(Nes6502 *cpu) {
    if (cpu->mode == IndirectX || cpu->mode == IndirectY) {
        write(cpu->bus, cpu->indirect_address, cpu->a);
    } else {
        write(cpu->bus, cpu->absolute_address, cpu->a);
    }
}

void STX(Nes6502 *cpu) {
    write(cpu->bus, cpu->absolute_address, cpu->x);
}

void STY(Nes6502 *cpu) {
    write(cpu->bus, cpu->absolute_address, cpu->y);
}

void TAX(Nes6502 *cpu) {
    cpu->x = cpu->a;
    set_flags(cpu, ZERO_FLAG, cpu->x == 0);
    set_flags(cpu, NEGATIVE_FLAG, (cpu->x & 0x80) != 0);
}

void TAY(Nes6502 *cpu) {
    cpu->y = cpu->a;
    set_flags(cpu, ZERO_FLAG, cpu->y == 0);
    set_flags(cpu, NEGATIVE_FLAG, (cpu->y & 0x80) != 0);
}

void TSX(Nes6502 *cpu) {
    cpu->x = cpu->sp;
    set_flags(cpu, ZERO_FLAG, cpu->x == 0);
    set_flags(cpu, NEGATIVE_FLAG, (cpu->x & 0x80) != 0);
}

void TXA(Nes6502 *cpu) {
    cpu->a = cpu->x;
    set_flags(cpu, ZERO_FLAG, cpu->a == 0);
    set_flags(cpu, NEGATIVE_FLAG, (cpu->a & 0x80) != 0);
}

void TXS(Nes6502 *cpu) {
    cpu->sp = cpu->x;
}

void TYA(Nes6502 *cpu) {
    cpu->a = cpu->y;
    set_flags(cpu, ZERO_FLAG, cpu->a == 0);
    set_flags(cpu, NEGATIVE_FLAG, (cpu->a & 0x80) != 0);
}