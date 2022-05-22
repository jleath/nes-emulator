#ifndef NES6502_H
#define NES6502_H

#include <stdint.h>

#define CARRY_FLAG 0x01
#define ZERO_FLAG 0x02
#define INTERRUPT_DISABLE_FLAG 0x04
#define DECIMAL_FLAG 0x08
#define UNUSED_FLAG 0x10
#define BREAK_FLAG 0x20
#define OVERFLOW_FLAG 0x40
#define NEGATIVE_FLAG 0x80

enum AddressingMode{
  Accumulator,
  Implied,
  Immediate,
  ZeroPage,
  ZeroPageX,
  ZeroPageY,
  Relative,
  Absolute,
  AbsoluteX,
  AbsoluteY,
  Indirect,
  IndirectX,
  IndirectY,
  None,
};

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
    int total_cycles;
    enum AddressingMode mode;
    uint16_t indirect_address;
    uint16_t absolute_address;
    uint8_t offset;
    uint8_t data;
    int add_cycles;
} Nes6502;

typedef struct Nes6502Instruction {
    char *opcode;
    void (*operation)(Nes6502*);
    void (*mode)(Nes6502*);
    uint8_t cycles;
} Nes6502Instruction;

void cycle(Nes6502 *cpu);
int complete(Nes6502 *cpu);
void reset(Nes6502 *cpu);
void interrupt(Nes6502 *cpu);
void non_maskable_interrupt(Nes6502 *cpu);
uint8_t get_flag(Nes6502 *cpu, uint8_t flag);
void set_flag(Nes6502 *cpu, uint8_t flag);

// Addressing Modes
void implied(Nes6502 *cpu);
void accumulator(Nes6502 *cpu);
void immediate(Nes6502 *cpu);
void zero_page(Nes6502 *cpu);
void zero_pageX(Nes6502 *cpu);
void zero_pageY(Nes6502 *cpu);
void relative(Nes6502 *cpu);
void absolute(Nes6502 *cpu);
void absoluteX(Nes6502 *cpu);
void absoluteY(Nes6502 *cpu);
void indirect(Nes6502 *cpu);
void indirectX(Nes6502 *cpu);
void indirectY(Nes6502 *cpu);

// Operations
void ADC(Nes6502 *cpu);
void AND(Nes6502 *cpu);
void ASL(Nes6502 *cpu);
void BCC(Nes6502 *cpu);
void BCS(Nes6502 *cpu);
void BEQ(Nes6502 *cpu);
void BIT(Nes6502 *cpu);
void BMI(Nes6502 *cpu);
void BNE(Nes6502 *cpu);
void BPL(Nes6502 *cpu);
void BRK(Nes6502 *cpu);
void BVC(Nes6502 *cpu);
void BVS(Nes6502 *cpu);
void CLC(Nes6502 *cpu);
void CLD(Nes6502 *cpu);
void CLI(Nes6502 *cpu);
void CLV(Nes6502 *cpu);
void CMP(Nes6502 *cpu);
void CPX(Nes6502 *cpu);
void CPY(Nes6502 *cpu);
void DEC(Nes6502 *cpu);
void DEX(Nes6502 *cpu);
void DEY(Nes6502 *cpu);
void EOR(Nes6502 *cpu);
void INC(Nes6502 *cpu);
void INX(Nes6502 *cpu);
void INY(Nes6502 *cpu);
void JMP(Nes6502 *cpu);
void JSR(Nes6502 *cpu);
void LDA(Nes6502 *cpu);
void LDX(Nes6502 *cpu);
void LDY(Nes6502 *cpu);
void LSR(Nes6502 *cpu);
void NOP(Nes6502 *cpu);
void ORA(Nes6502 *cpu);
void PHA(Nes6502 *cpu);
void PHP(Nes6502 *cpu);
void PLA(Nes6502 *cpu);
void PLP(Nes6502 *cpu);
void ROL(Nes6502 *cpu);
void ROR(Nes6502 *cpu);
void RTI(Nes6502 *cpu);
void RTS(Nes6502 *cpu);
void SBC(Nes6502 *cpu);
void SEC(Nes6502 *cpu);
void SED(Nes6502 *cpu);
void SEI(Nes6502 *cpu);
void STA(Nes6502 *cpu);
void STX(Nes6502 *cpu);
void STY(Nes6502 *cpu);
void TAX(Nes6502 *cpu);
void TAY(Nes6502 *cpu);
void TSX(Nes6502 *cpu);
void TXA(Nes6502 *cpu);
void TXS(Nes6502 *cpu);
void TYA(Nes6502 *cpu);

#endif