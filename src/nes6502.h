#ifndef NES6502_H
#define NES6502_H

#include <stdint.h>

#define CARRY_FLAG 0x01
#define ZERO_FLAG 0x02
#define INTERRUPT_DISABLE_FLAG 0x04
#define BREAK_FLAG 0x20
#define OVERFLOW_FLAG 0x40
#define NEGATIVE_FLAG 0x80

typedef struct Bus {
    uint16_t memory_size;
    uint8_t *ram;
} Bus;

Bus *create_bus(int memory_size);
void destroy_bus(Bus *bus);
void write(Bus *bus, uint16_t address, uint8_t data);
uint8_t read(Bus *bus, uint16_t address);

typedef struct Nes6502 {
    uint8_t a;
    uint8_t x;
    uint8_t y;
    uint8_t sp;
    uint8_t flags;
    uint16_t pc;
    Bus *bus;
    uint8_t cycles;
} Nes6502;

void cycle(Nes6502 *cpu);
void reset(Nes6502 *cpu);
void interrupt(Nes6502 *cpu);
void non_maskable_interrupt(Nes6502 *cpu);
uint8_t get_flag(Nes6502 *cpu, uint8_t flag);
void set_flag(Nes6502 *cpu, uint8_t flag);

// Addressing Modes
uint16_t accumulator(Nes6502 *cpu);
uint16_t immediate(Nes6502 *cpu);
uint16_t zero_page(Nes6502 *cpu);
uint16_t zero_pageX(Nes6502 *cpu);
uint16_t zero_pageY(Nes6502 *cpu);
uint16_t relative(Nes6502 *cpu);
uint16_t absolute(Nes6502 *cpu);
uint16_t absoluteX(Nes6502 *cpu);
uint16_t absoluteY(Nes6502 *cpu);
uint16_t indirect(Nes6502 *cpu);
uint16_t indirectX(Nes6502 *cpu);
uint16_t indirectY(Nes6502 *cpu);

// Operations
void ADC(Nes6502 *cpu, uint8_t data);
void AND(Nes6502 *cpu, uint8_t data);
void ASL(Nes6502 *cpu, int mem_address);
void BCC(Nes6502 *cpu, uint8_t offset);
void BCS(Nes6502 *cpu, uint8_t offset);
void BEQ(Nes6502 *cpu, uint8_t offset);
void BIT(Nes6502 *cpu, uint16_t mem_address);
void BMI(Nes6502 *cpu, uint8_t offset);
void BNE(Nes6502 *cpu, uint8_t offset);
void BPL(Nes6502 *cpu, uint8_t offset);
void BRK(Nes6502 *cpu);
void BVC(Nes6502 *cpu, uint8_t offset);
void BVS(Nes6502 *cpu, uint8_t offset);
void CLC(Nes6502 *cpu);
void CLD(Nes6502 *cpu);
void CLI(Nes6502 *cpu);
void CLV(Nes6502 *cpu);
void CMP(Nes6502 *cpu, uint16_t mem_address);
void CPX(Nes6502 *cpu, uint16_t mem_address);
void CPY(Nes6502 *cpu, uint16_t mem_address);
void DEC(Nes6502 *cpu, uint16_t mem_address);
void DEX(Nes6502 *cpu);
void DEY(Nes6502 *cpu);
void EOR(Nes6502 *cpu, uint8_t data);
void INC(Nes6502 *cpu, uint16_t mem_address);
void INX(Nes6502 *cpu);
void INY(Nes6502 *cpu);
void JMP(Nes6502 *cpu, uint16_t mem_address);
void JSR(Nes6502 *cpu, uint16_t mem_address);
void LDA(Nes6502 *cpu, uint8_t data);
void LDX(Nes6502 *cpu, uint8_t data);
void LDY(Nes6502 *cpu, uint8_t data);
void LSR(Nes6502 *cpu, int mem_address);
void NOP(Nes6502 *cpu);
void ORA(Nes6502 *cpu, uint8_t data);
void PHA(Nes6502 *cpu);
void PHP(Nes6502 *cpu);
void PLA(Nes6502 *cpu);
void PLP(Nes6502 *cpu);
void ROL(Nes6502 *cpu, int mem_address);
void ROR(Nes6502 *cpu, int mem_address);
void RTI(Nes6502 *cpu);
void RTS(Nes6502 *cpu);

#endif