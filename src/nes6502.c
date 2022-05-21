#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "nes6502.h"

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

void cycle(Nes6502 *cpu) {
    if (cpu->cycles == 0) {
        // read a byte from memory at pc
        cpu->pc++;
        // decode opcode and get data using addressing mode
        // execute instruction
        cpu->cycles--;
    }
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
uint16_t accumulator(Nes6502 *cpu) {
    return cpu->a;
}

uint16_t immediate(Nes6502 *cpu) {
    return (cpu->pc)++;
}

uint16_t zero_page(Nes6502 *cpu) {
    uint8_t address = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    return 0x00ff & address;

}

uint16_t zero_pageX(Nes6502 *cpu) {
    uint8_t address = read(cpu->bus, cpu->pc) + cpu->x;
    (cpu->pc)++;
    return 0x00ff & address;
}

uint16_t zero_pageY(Nes6502 *cpu) {
    uint8_t address = read(cpu->bus, cpu->pc) + cpu->y;
    (cpu->pc)++;
    return 0x00ff & address;
}

/* Need to confirm this one, I don't know if branch is relative to incremented
   pc, or the pc that points to the current byte 
*/
uint16_t relative(Nes6502 *cpu) {
    uint8_t address = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    if (address & 0x80) {
        return cpu->pc - address;
    }
    return cpu->pc + address;
    (cpu->pc)++;
}

uint16_t absolute(Nes6502 *cpu) {
    uint16_t low = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    uint16_t high = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    return (high << 8) | low;
}

uint16_t absoluteX(Nes6502 *cpu) {
    uint16_t low = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    uint16_t high = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    uint16_t address = (high << 8) | low;
    address += cpu->x;
    if ((address & 0xff00) != (high << 8)) {
        cpu->cycles++;
    }
    return address;
}

uint16_t absoluteY(Nes6502 *cpu) {
    uint16_t low = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    uint16_t high = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    uint16_t address = (high << 8) | low;
    address += cpu->y;
    if ((address & 0xff00) != (high << 8)) {
        cpu->cycles++;
    }
    return address;
}

// Have to emulate page boundary bug in hardware
uint16_t indirect(Nes6502 *cpu) {
    uint16_t low = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    uint16_t high = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    uint16_t address = (high << 8) | low;
    if (low == 0x00ff) {
        return (read(cpu->bus, address & 0xff00) << 8) | read(cpu->bus, address);
    } else {
        return (read(cpu->bus, address + 1) << 8) | read(cpu->bus, address + 0);
    }
}

uint16_t indirectX(Nes6502 *cpu) {
    uint16_t x = (uint16_t)cpu->x;
    uint16_t address = read(cpu->bus, cpu->pc);
    (cpu->pc)++;
    uint16_t low = read(cpu->bus, (address + x) & 0x00ff);
    uint16_t high = read(cpu->bus, (address + x + 1) & 0x00ff);
    return (high << 8) | low;
}

uint16_t indirectY(Nes6502 *cpu) {
    uint16_t address = read(cpu->bus, cpu->pc);
    uint16_t y = (uint16_t)cpu->y;
    (cpu->pc)++;
    uint16_t low = read(cpu->bus, address & 0x00ff);
    uint16_t high = read(cpu->bus, (address + 1) & 0x00ff);
    address = ((high << 8) | low) + y;
    if ((address & 0xff00) != (high << 8)) {
        (cpu->cycles)++;
    }
    return address;
}

// 6502 OPERATIONS
void ADC(Nes6502 *cpu, uint8_t data) {
    uint16_t a = cpu->a;
    uint16_t addend = data;
    uint16_t new_value = addend + a;
    new_value += (uint16_t)get_flag(cpu, CARRY_FLAG);
    set_flags(cpu, CARRY_FLAG, (new_value & 0xff00) != 0);
    set_flags(cpu, ZERO_FLAG, (new_value & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x80) != 0);
    set_flags(cpu, OVERFLOW_FLAG, ~((a ^ addend) & (a ^ new_value)) & 0x0080);
    cpu->a = (new_value & 0x00ff);
}

void AND(Nes6502 *cpu, uint8_t data) {
    uint8_t new_value = cpu->a & data;
    set_flags(cpu, ZERO_FLAG, (new_value & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x80) != 0);
    cpu->a = new_value;
}

void ASL(Nes6502 *cpu, int mem_address) {
    uint8_t new_value;
    if (mem_address == -1) {
        set_flags(cpu, CARRY_FLAG, (cpu->a & 0x80) != 0);
        new_value = cpu->a << 1;
        cpu->a = new_value;
    } else {
        uint8_t curr_value = read(cpu->bus, mem_address);
        set_flags(cpu, CARRY_FLAG, (curr_value & 0x80) != 0);
        new_value = curr_value << 1;
        write(cpu->bus, mem_address, new_value);
    }
    set_flags(cpu, ZERO_FLAG, (new_value & 0xff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x80) != 0);
}

void BCC(Nes6502 *cpu, uint8_t offset) {
    if (!get_flag(cpu, CARRY_FLAG)) {
        uint16_t new_address = cpu->pc + (uint16_t)offset;
        cpu->cycles++;
        if ((new_address & 0xff00) != (cpu->pc & 0xff00)) {
            cpu->cycles++;
        }
        cpu->pc = new_address;
    }
}

void BCS(Nes6502 *cpu, uint8_t offset) {
    if (get_flag(cpu, CARRY_FLAG)) {
        uint16_t new_address = cpu->pc + (uint16_t)offset;
        cpu->cycles++;
        if ((new_address & 0xff00) != (cpu->pc & 0xff00)) {
            cpu->cycles++;
        }
        cpu->pc = new_address;
    }
}

void BEQ(Nes6502 *cpu, uint8_t offset) {
    if (get_flag(cpu, ZERO_FLAG)) {
        uint16_t new_address = cpu->pc + (uint16_t)offset;
        cpu->cycles++;
        if ((new_address & 0xff00) != (cpu->pc & 0xff00)) {
            cpu->cycles++;
        }
        cpu->pc = new_address;
    }
}

void BIT(Nes6502 *cpu, uint16_t mem_address) {
    uint8_t data = read(cpu->bus, mem_address);
    set_flags(cpu, ZERO_FLAG, (cpu->a & data) == 0);
    set_flags(cpu, OVERFLOW_FLAG, (data & 0x40) != 0);
    set_flags(cpu, NEGATIVE_FLAG, (data & 0x80) != 0);
}

void BMI(Nes6502 *cpu, uint8_t offset) {
    if (get_flag(cpu, NEGATIVE_FLAG)) {
        uint16_t new_address = cpu->pc + (uint16_t)offset;
        cpu->cycles++;
        if ((new_address & 0xff00) != (cpu->pc & 0xff00)) {
            cpu->cycles++;
        }
        cpu->pc = new_address;
    }
}

void BNE(Nes6502 *cpu, uint8_t offset) {
    if (!get_flag(cpu, ZERO_FLAG)) {
        uint16_t new_address = cpu->pc + (uint16_t)offset;
        cpu->cycles++;
        if ((new_address & 0xff00) != (cpu->pc & 0xff00)) {
            cpu->cycles++;
        }
        cpu->pc = new_address;
    }
}

void BPL(Nes6502 *cpu, uint8_t offset) {
    if (!get_flag(cpu, NEGATIVE_FLAG)) {
        uint16_t new_address = cpu->pc + (uint16_t)offset;
        cpu->cycles++;
        if ((new_address & 0xff00) != (cpu->pc & 0xff00)) {
            cpu->cycles++;
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

void BVC(Nes6502 *cpu, uint8_t offset) {
    if (!get_flag(cpu, OVERFLOW_FLAG)) {
        uint16_t new_address = cpu->pc + (uint16_t)offset;
        cpu->cycles++;
        if ((new_address & 0xff00) != (cpu->pc & 0xff00)) {
            cpu->cycles++;
        }
        cpu->pc = new_address;
    }
}

void BVS(Nes6502 *cpu, uint8_t offset) {
    if (get_flag(cpu, OVERFLOW_FLAG)) {
        uint16_t new_address = cpu->pc + (uint16_t)offset;
        cpu->cycles++;
        if ((new_address & 0xff00) != (cpu->pc & 0xff00)) {
            cpu->cycles++;
        }
        cpu->pc = new_address;
    }
}

void CLC(Nes6502 *cpu) {
    set_flags(cpu, CARRY_FLAG, 0);
}

void CLD(Nes6502 *cpu) {
    // Decimal Mode not yet implemented, do nothing
    return;
}

void CLI(Nes6502 *cpu) {
    set_flags(cpu, INTERRUPT_DISABLE_FLAG, 0);
}

void CLV(Nes6502 *cpu) {
    set_flags(cpu, OVERFLOW_FLAG, 0);
}

void CMP(Nes6502 *cpu, uint16_t mem_address) {
    uint16_t data = (uint16_t)read(cpu->bus, mem_address);
    uint16_t comp_result = (uint16_t)cpu->a - data;
    set_flags(cpu, CARRY_FLAG, cpu->a >= data);
    set_flags(cpu, ZERO_FLAG, (comp_result & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, comp_result & 0x0080);
}

void CPX(Nes6502 *cpu, uint16_t mem_address) {
    uint16_t data = (uint16_t)read(cpu->bus, mem_address);
    uint16_t comp_result = (uint16_t)cpu->x - data;
    set_flags(cpu, CARRY_FLAG, cpu->x >= data);
    set_flags(cpu, ZERO_FLAG, (comp_result & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, comp_result & 0x0080);
}

void CPY(Nes6502 *cpu, uint16_t mem_address) {
    uint16_t data = (uint16_t)read(cpu->bus, mem_address);
    uint16_t comp_result = (uint16_t)cpu->y - data;
    set_flags(cpu, CARRY_FLAG, cpu->y >= data);
    set_flags(cpu, ZERO_FLAG, (comp_result & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (comp_result & 0x0080) != 0);
}

void DEC(Nes6502 *cpu, uint16_t mem_address) {
    uint16_t data = (uint16_t)read(cpu->bus, mem_address);
    uint16_t new_value = data - 1;
    set_flags(cpu, ZERO_FLAG, (new_value & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x0080) != 0);
    write(cpu->bus, mem_address, new_value & 0x00ff);
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

void EOR(Nes6502 *cpu, uint8_t data) {
    uint8_t new_value = cpu->a ^ data;
    set_flags(cpu, ZERO_FLAG, (new_value & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x80) != 0);
    cpu->a = new_value;
}

void INC(Nes6502 *cpu, uint16_t mem_address) {
    uint16_t data = read(cpu->bus, mem_address);
    uint16_t new_value = data + 1;
    set_flags(cpu, ZERO_FLAG, (new_value & 0x00ff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x0080) != 0);
    write(cpu->bus, mem_address, new_value & 0x00ff);
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

void JMP(Nes6502 *cpu, uint16_t mem_address) {
    cpu->pc = mem_address;
}

void JSR(Nes6502 *cpu, uint16_t mem_address) {
    (cpu->pc)--;
    write(cpu->bus, 0x0100 + cpu->sp, (cpu->pc >> 8) & 0x00ff);
    (cpu->sp)--;
    write(cpu->bus, 0x0100 + cpu->sp, cpu->pc & 0x00ff);
    (cpu->sp)--;

    cpu->pc = mem_address;
}

void LDA(Nes6502 *cpu, uint8_t data) {
    cpu->a = data;
    set_flags(cpu, ZERO_FLAG, cpu->a == 0);
    set_flags(cpu, NEGATIVE_FLAG, (cpu->a & 0x80) != 0);
}

void LDX(Nes6502 *cpu, uint8_t data) {
    cpu->x = data;
    set_flags(cpu, ZERO_FLAG, cpu->x == 0);
    set_flags(cpu, NEGATIVE_FLAG, (cpu->x & 0x80) != 0);
}

void LDY(Nes6502 *cpu, uint8_t data) {
    cpu->y = data;
    set_flags(cpu, ZERO_FLAG, cpu->y == 0);
    set_flags(cpu, NEGATIVE_FLAG, (cpu->y & 0x80) != 0);
}

void LSR(Nes6502 *cpu, int mem_address) {
    uint8_t new_value;
    if (mem_address == -1) {
        set_flags(cpu, CARRY_FLAG, (cpu->a & 0x01) != 0);
        new_value = cpu->a >> 1;
        new_value = new_value & 0x7f;
        cpu->a = new_value;
    } else {
        uint8_t curr_value = read(cpu->bus, mem_address);
        set_flags(cpu, CARRY_FLAG, (curr_value & 0x01) != 0);
        new_value = curr_value >> 1;
        new_value = new_value & 0x7f;
        write(cpu->bus, mem_address, new_value);
    }
    set_flags(cpu, ZERO_FLAG, (new_value & 0xff) == 0);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x80) != 0);
}

void NOP(Nes6502 *cpu) {
    return;
}

void ORA(Nes6502 *cpu, uint8_t data) {
    uint8_t new_value = cpu->a | data;
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
    write(cpu->bus, 0x0100 + cpu->sp, cpu->flags | BREAK_FLAG);
    set_flags(cpu, BREAK_FLAG, 0);
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
    uint8_t data = read(cpu->bus, 0x0100 + cpu->sp);
    cpu->flags = data;
}

void ROL(Nes6502 *cpu, int mem_address) {
    uint8_t old_carry = get_flag(cpu, CARRY_FLAG);
    uint8_t new_carry;
    uint8_t new_value;
    if (mem_address == -1) {
        new_carry = ((cpu->a & 0x80) >> 7) & 0x1;
        cpu->a = cpu->a << 1;
        cpu->a = cpu->a | old_carry;
        new_value = cpu->a;
    } else {
        uint8_t old_value = read(cpu->bus, mem_address);
        new_carry = ((old_value & 0x80) >> 7) & 0x1;
        new_value = old_value << 1;
        new_value = new_value | old_carry;
        write(cpu->bus, mem_address, new_value);
    }
    set_flags(cpu, ZERO_FLAG, new_value == 0);
    set_flags(cpu, CARRY_FLAG, new_carry);
    set_flags(cpu, NEGATIVE_FLAG, (new_value & 0x80) != 0);
}

void ROR(Nes6502 *cpu, int mem_address) {
    uint8_t old_carry = get_flag(cpu, CARRY_FLAG);
    uint8_t new_carry;
    uint8_t new_value;
    if (mem_address == -1) {
        new_carry = cpu->a & 0x1;
        cpu->a = cpu->a >> 1;
        cpu->a = cpu->a | (old_carry << 7);
        new_value = cpu->a;
    } else {
        uint8_t old_value = read(cpu->bus, mem_address);
        new_carry = old_value & 0x1;
        new_value = old_value >> 1;
        new_value = new_value | (old_carry << 7);
        write(cpu->bus, mem_address, new_value);
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
    cpu->flags = flags & ~BREAK_FLAG;
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