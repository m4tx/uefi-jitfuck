#include <assert.h>
#include <stdbool.h>

#include "Opcodes.h"
#include "Constants.h"
#include "JITConstants.h"
#include "InstructionEmitters.h"
#include "SpecialByteEmitters.h"
#include "ChunkEmitters.h"

void emitIncReg64(Byte **buf, enum Register reg) {
    emitREX(buf, 1, 0, 0, 0);
    emitByte(buf, INC64_OPCODE);
    emitModRM(buf, 0b11, 0, reg);
}

void emitDecReg64(Byte **buf, enum Register reg) {
    emitREX(buf, 1, 0, 0, 0);
    emitByte(buf, DEC64_OPCODE);
    emitModRM(buf, 0b11, 1, reg);
}

void emitIncBytePtr(Byte **buf, enum Register reg) {
    // Doesn't support special REX encoding for RSP and RBP
    assert(reg != RSP && reg != RBP);

    emitByte(buf, INC8_OPCODE);
    emitModRM(buf, 0, 0, reg);
}

void emitDecBytePtr(Byte **buf, enum Register reg) {
    // Doesn't support special REX encoding for RSP and RBP
    assert(reg != RSP && reg != RBP);

    emitByte(buf, DEC8_OPCODE);
    emitModRM(buf, 0, 1, reg);
}

void emitMovRegImm64(Byte **buf, enum Register reg, long imm) {
    emitREX(buf, 1, 0, 0, 0);
    emitByte(buf, (Byte) (MOV_REGIMM64_OPCODE + reg));
    emitLong(buf, imm);
}

void emitCmpRegImm8(Byte **buf, enum Register reg, Byte imm) {
    Byte *oldBuf = *buf;

    emitByte(buf, CMP_REGIMM8_OPCODE);
    emitModRM(buf, 0, 7, reg);
    emitByte(buf, imm);

    assert(*buf - oldBuf == CMP_REG_IMM8_LEN);
}

void emitJmpRel32Off(Byte **buf, int off) {
    Byte *oldBuf = *buf;

    emitByte(buf, JMP_REL32_OPCODE);
    emitInt(buf, off);

    assert(*buf - oldBuf == JMP_REL32OFF_LEN);
}

void emitJeRel32Off(Byte **buf, int off) {
    Byte *oldBuf = *buf;

    emitShort(buf, JE_REL32_OPCODE);
    emitInt(buf, off);

    assert(*buf - oldBuf == JE_REL32OFF_LEN);
}

static inline bool isByte(int value) {
    return value >= -128 && value <= 127;
}

void emitAddRegImm(Byte **buf, enum Register reg, int imm) {
    emitREX(buf, 1, 0, 0, 0);
    if (isByte(imm)) {
        emitByte(buf, ADD_REG64IMM8_OPCODE);
        emitModRM(buf, 0b11, 0, reg);
        emitByte(buf, (Byte) imm);
    } else {
        emitByte(buf, ADD_REG64IMM32_OPCODE);
        emitModRM(buf, 0b11, 0, reg);
        emitInt(buf, imm);
    }
}

void emitAddBytePtrImm(Byte **buf, enum Register reg, Byte imm) {
    // Doesn't support special REX encoding for RSP and RBP
    assert(reg != RSP && reg != RBP);

    emitByte(buf, ADD_MEMIMM8_OPCODE);
    emitModRM(buf, 0, 0, reg);
    emitByte(buf, (Byte) imm);
}

void emitSubRegImm(Byte **buf, enum Register reg, int imm) {
    emitREX(buf, 1, 0, 0, 0);
    if (isByte(imm)) {
        emitByte(buf, SUB_REG64IMM8_OPCODE);
        emitModRM(buf, 0b11, 5, reg);
        emitByte(buf, (Byte) imm);
    } else {
        emitByte(buf, SUB_REG64IMM32_OPCODE);
        emitModRM(buf, 0b11, 5, reg);
        emitInt(buf, imm);
    }
}

void emitSubBytePtrImm(Byte **buf, enum Register reg, Byte imm) {
    // Doesn't support special REX encoding for RSP and RBP
    assert(reg != RSP && reg != RBP);

    emitByte(buf, SUB_MEMIMM8_OPCODE);
    emitModRM(buf, 0, 5, reg);
    emitByte(buf, (Byte) imm);
}

void emitMovRegRegPtr(Byte **buf, enum Register reg1, enum Register reg2) {
    // Doesn't support special REX encoding for RSP and RBP
    assert(reg1 != RSP && reg1 != RBP && reg2 != RSP && reg2 != RBP);

    emitREX(buf, 1, 0, 0, 0);
    emitByte(buf, MOV_REGMEM64_OPCODE);
    emitModRM(buf, 0, reg1, reg2);
}

void emitMovRegPtrReg(Byte **buf, enum Register reg1, enum Register reg2) {
    // Doesn't support special REX encoding for RSP and RBP
    assert(reg1 != RSP && reg1 != RBP && reg2 != RSP && reg2 != RBP);

    emitREX(buf, 1, 0, 0, 0);
    emitByte(buf, MOV_MEMREG64_OPCODE);
    emitModRM(buf, 0, reg2, reg1);
}

void emitCallReg(Byte **buf, enum Register reg) {
    emitByte(buf, CALL_REG_OPCODE);
    emitModRM(buf, 0b11, 2, reg);
}

void emitPushReg(Byte **buf, enum Register reg) {
    emitByte(buf, (Byte) (PUSH_REG_OPCODE + reg));
}

void emitPopReg(Byte **buf, enum Register reg) {
    emitByte(buf, (Byte) (POP_REG_OPCODE + reg));
}

void emitRet(Byte **buf) {
    emitByte(buf, RET_OPCODE);
}
