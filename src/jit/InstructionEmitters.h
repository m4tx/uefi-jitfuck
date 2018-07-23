#ifndef UEFI_JITFUCK_INSTRUCTIONEMITTERS_H
#define UEFI_JITFUCK_INSTRUCTIONEMITTERS_H

#include "Types.h"

/// inc reg
void emitIncReg64(Byte **buf, enum Register reg);

/// dec reg
void emitDecReg64(Byte **buf, enum Register reg);

/// inc BYTE [reg]
void emitIncBytePtr(Byte **buf, enum Register reg);

/// dec BYTE [reg]
void emitDecBytePtr(Byte **buf, enum Register reg);

/// mov reg, imm
void emitMovRegImm64(Byte **buf, enum Register reg, long imm);

/// cmp reg, imm
void emitCmpRegImm8(Byte **buf, enum Register reg, Byte imm);

/// jmp +off
void emitJmpRel32Off(Byte **buf, int off);

/// je +off
void emitJeRel32Off(Byte **buf, int off);

/// add reg, imm
void emitAddRegImm(Byte **buf, enum Register reg, int imm);

/// add BYTE [reg], imm
void emitAddBytePtrImm(Byte **buf, enum Register reg, Byte imm);

/// sub reg, imm
void emitSubRegImm(Byte **buf, enum Register reg, int imm);

/// sub BYTE [reg], imm
void emitSubBytePtrImm(Byte **buf, enum Register reg, Byte imm);

/// mov reg1, [reg2]
void emitMovRegRegPtr(Byte **buf, enum Register reg1, enum Register reg2);

/// mov [reg1], reg2
void emitMovRegPtrReg(Byte **buf, enum Register reg1, enum Register reg2);

/// call reg
void emitCallReg(Byte **buf, enum Register reg);

/// push reg
void emitPushReg(Byte **buf, enum Register reg);

/// pop reg
void emitPopReg(Byte **buf, enum Register reg);

/// ret
void emitRet(Byte **buf);

#endif //UEFI_JITFUCK_INSTRUCTIONEMITTERS_H
