#include <stdio.h>

#include "nes6502.h"

void print_test_result(char *test_name, uint16_t expected, uint16_t got) {
    if (expected != got) {
        printf("%s Failed: Expected $%04x, got $%04x\n", test_name, expected, got);
    }
}

int main(void) {
    Bus *bus = create_bus(64*1024);
    Nes6502 cpu;
    cpu.bus = bus;
    cpu.cycles = 0;
    cpu.pc = 0;
    cpu.a = 0;
    cpu.x = 0;
    cpu.y = 0;

    // Testing addressing modes
    // Accumulator
    cpu.a = 42;
    print_test_result("Accumulator", 42, accumulator(&cpu));

    // Immediate
    cpu.pc = 0;
    print_test_result("Immediate", 0, immediate(&cpu));

    // Zero Page
    write(bus, 0x12, 0x42);
    cpu.pc = 0x12;
    print_test_result("Zero Page", 0x42, zero_page(&cpu));

    // Zero Page X
    cpu.x = 0x0f;
    cpu.pc = 0;
    write(bus, 0x00, 0x80);
    print_test_result("Zero Page X", 0x8f, zero_pageX(&cpu));
    cpu.x = 0xff;
    cpu.pc = 0;
    print_test_result("Zero Page X", 0x7f, zero_pageX(&cpu));

    // Zero Page Y
    cpu.y = 0x0f;
    cpu.pc = 0;
    write(bus, 0x00, 0x80);
    print_test_result("Zero Page Y", 0x8f, zero_pageY(&cpu));
    cpu.y = 0xff;
    cpu.pc = 0;
    print_test_result("Zero Page Y", 0x7f, zero_pageY(&cpu));

    // Absolute
    cpu.pc = 0;
    write(bus, 0, 0x34);
    write(bus, 1, 0x12);
    print_test_result("Absolute", 0x1234, absolute(&cpu));

    // Absolute X
    cpu.pc = 0;
    cpu.x = 0x92;
    write(bus, 0, 0x00);
    write(bus, 1, 0x20);
    print_test_result("Absolute X", 0x2092, absoluteX(&cpu));

    // Absolute Y
    cpu.pc = 0;
    cpu.y = 0x92;
    write(bus, 0, 0x00);
    write(bus, 1, 0x20);
    print_test_result("Absolute Y", 0x2092, absoluteY(&cpu));

    // Indirect
    cpu.pc = 0;
    write(bus, 0, 0x34);
    write(bus, 1, 0x12);
    write(bus, 0x1234, 0x78);
    write(bus, 0x1235, 0x56);
    print_test_result("Indirect", 0x5678, indirect(&cpu));
    cpu.pc = 0;
    write(bus, 0, 0xff);
    write(bus, 1, 0x12);
    write(bus, 0x12ff, 0x78);
    write(bus, 0x1200, 0x56);
    print_test_result("Indirect (bugged)", 0x5678, indirect(&cpu));

    // Indirect X
    cpu.pc = 0;
    write(bus, 0, 0x51);
    cpu.x = 0xe9;
    write(bus, 0x3104, 0x81);
    write(bus, 0x3a, 0x04);
    write(bus, 0x3b, 0x31);
    print_test_result("Indirect X", 0x3104, indirectX(&cpu));


    // Indirect Y
    cpu.pc = 0;
    write(bus, 0, 0xa4);
    write(bus, 0xa4, 0x51);
    write(bus, 0xa5, 0x3f);
    cpu.y = 0xe9;
    write(bus, 0x3104, 0x81);
    write(bus, 0x3a, 0x04);
    write(bus, 0x3b, 0x31);
    print_test_result("Indirect Y", 0x403a, indirectY(&cpu));
}