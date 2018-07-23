#ifndef UEFI_JITFUCK_OPCODES_H
#define UEFI_JITFUCK_OPCODES_H

#define REX_PREFIX 0b0100
#define INC8_OPCODE 0xFE
#define INC64_OPCODE 0xFF
#define DEC8_OPCODE 0xFE
#define DEC64_OPCODE 0xFF
#define MOV_REGIMM64_OPCODE 0xB8
#define CMP_REGIMM8_OPCODE 0x80
#define JMP_REL32_OPCODE 0xE9
#define JE_REL32_OPCODE ((short) 0x840F)
#define ADD_REG64IMM8_OPCODE 0x83
#define ADD_REG64IMM32_OPCODE 0x81
#define ADD_MEMIMM8_OPCODE 0x80
#define SUB_REG64IMM8_OPCODE 0x83
#define SUB_REG64IMM32_OPCODE 0x81
#define SUB_MEMIMM8_OPCODE 0x80
#define MOV_MEMREG64_OPCODE 0x89
#define MOV_REGMEM64_OPCODE 0x8B
#define CALL_REG_OPCODE 0xFF
#define PUSH_REG_OPCODE 0x50
#define POP_REG_OPCODE 0x58
#define RET_OPCODE 0xC3

#endif //UEFI_JITFUCK_OPCODES_H