#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "nes6502.h"
#include "code_reader.h"

void print_test_result(char *test_name, uint16_t expected, uint16_t got) {
    if (expected != got) {
        printf("%s Failed: Expected $%04x, got $%04x\n", test_name, expected, got);
    }
}

uint8_t convert_hex(uint8_t byte)  {
    if (byte >= '0' && byte <= '9') {
        return byte - '0';
    } else if (byte >= 'A' && byte <= 'F') {
        return byte - 'A' + 10;
    } else if (byte >= 'a' && byte <= 'f') {
        return byte - 'a' + 10;
    } else {
        return -1;
    }
}

uint8_t translate_code(char *input, uint8_t *output) {
    uint8_t curr_val;
    int curr_hex = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == ' ') {
            output[curr_hex] = curr_val;
            curr_val = 0;
            curr_hex++;
        } else {
            curr_val = (curr_val << 4) | (convert_hex(input[i]) & 0x0f);
        }
    }
    output[curr_hex] = curr_val;
    return curr_hex + 1;
}

int main(void) {
    Bus *bus = create_bus(64*1024);
    Nes6502 cpu;
    cpu.bus = bus;

    cpu.bus->ram[0xfffc] = 0x00;
    cpu.bus->ram[0xfffd] = 0xc0;

    reset(&cpu);

    // char *filename = "nestest.nes";
    // off_t file_size = get_file_size(filename);
    // if (file_size == -1) {
    //     printf("Unable to get file size of %s\n", filename);
    //     return -1;
    // }
    // uint8_t *code_buffer = read_program_file(filename, file_size);
    // if (code_buffer == NULL) {
    //     printf("failed to open and read file %s\n\n", filename);
    //     return -2;
    // }

    // // off_t pc = 0;
    // // while (pc < file_size) {
    // //     printf("%02x ", code_buffer[pc]);
    // //     if (pc % 30 == 0 && pc != 0) {
    // //         printf("\n");
    // //     }
    // //     pc++;
    // // }
    // // printf("\n");
    // temp cycle adjustment just to match test output
    cpu.total_cycles = 7;
    // reset flags to match test
    cpu.flags = 0x24;
    uint8_t values[3];
    while (!complete(&cpu)) {
        cycle(&cpu);
    }
    int instructions_read = 0;
    while (instructions_read < 8991) {
        instructions_read++;
        char input[9];
        int c;
        int curr_char = 0;
        while ((c = getchar()) != '\n' && c != EOF) {
            if (curr_char < 8) {
                input[curr_char] = c;
                curr_char++;
            }
        }
        input[curr_char] = '\0'; 
        int num_bytes = translate_code(input, values);
        uint16_t p = cpu.pc;
        for (int i = 0; i < num_bytes; i++) {
            cpu.bus->ram[p] = values[i];
            p++;
        }
        do {
            cycle(&cpu);
        } while (!complete(&cpu));
    }
    // free(code_buffer);
    return 0;
}