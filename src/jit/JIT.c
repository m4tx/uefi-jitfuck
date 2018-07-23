#include <assert.h>

#include "JIT.h"
#include "Types.h"
#include "JITConstants.h"
#include "Constants.h"
#include "UefiIOWrapper.h"
#include "InstructionEmitters.h"

static inline void handleIncPtrChr(Byte **buf, size_t count) {
    if (count == 1) {
        emitIncReg64(buf, POINTER_REGISTER);
    } else {
        emitAddRegImm(buf, POINTER_REGISTER, (int) count);
    }
}

static inline void handleDecPtrChr(Byte **buf, size_t count) {
    if (count == 1) {
        emitDecReg64(buf, POINTER_REGISTER);
    } else {
        emitSubRegImm(buf, POINTER_REGISTER, (int) count);
    }
}

static inline void handleIncValChr(Byte **buf, size_t count) {
    if (count == 1) {
        emitIncBytePtr(buf, POINTER_REGISTER);
    } else {
        emitAddBytePtrImm(buf, POINTER_REGISTER, (Byte) (count % 256));
    }
}

static inline void handleDecValChr(Byte **buf, size_t count) {
    if (count == 1) {
        emitDecBytePtr(buf, POINTER_REGISTER);
    } else {
        emitSubBytePtrImm(buf, POINTER_REGISTER, (Byte) (count % 256));
    }
}

static inline void emitGuardedCallRax(Byte **buf) {
    emitPushReg(buf, POINTER_REGISTER);
    emitCallReg(buf, RAX);
    emitPopReg(buf, POINTER_REGISTER);
}

static inline void handleOutputValChr(Byte **buf) {
    emitMovRegRegPtr(buf, RDI, POINTER_REGISTER);
    emitMovRegImm64(buf, RAX, (long) &printChar);
    emitGuardedCallRax(buf);
}

static inline void handleInputValChr(Byte **buf) {
    emitMovRegImm64(buf, RAX, (long) &readChar);
    emitGuardedCallRax(buf);
    emitMovRegPtrReg(buf, POINTER_REGISTER, RAX);
}

static inline void
handleBlockStartChr(Byte **buf, Byte *blockStack[1000000], int *blockStackTop) {
    Byte *oldBuf = *buf;

    blockStack[(*blockStackTop)++] = *buf;

    emitCmpRegImm8(buf, POINTER_REGISTER, 0);
    // Will be rewritten at the end of the block
    emitJeRel32Off(buf, 0);

    assert(*buf - oldBuf == BLOCK_START_LEN);
}

static inline void
handleBlockEndChr(Byte **buf, Byte *blockStack[1000000], int *blockStackTop) {
    Byte *blockStartPos = blockStack[--(*blockStackTop)];
    int offset = (int) (*buf - blockStartPos + JMP_REL32OFF_LEN);
    emitJmpRel32Off(buf, -offset);

    // Update block start jump offset
    blockStartPos += BLOCK_START_LEN - JE_REL32OFF_LEN;
    emitJeRel32Off(&blockStartPos, offset - BLOCK_START_LEN);
}

CompiledFunction
jitCompile(const unsigned char *source, size_t length, void *codeBuffer,
           void *programMemory) {
    Byte *buf = codeBuffer;

    Byte *blockStack[1000000];
    int blockStackTop = 0;

    emitMovRegImm64(&buf, POINTER_REGISTER, (long) programMemory);

    for (size_t i = 0; i < length; ++i) {
        switch (source[i]) {
            case INC_PTR_CHR:
            case DEC_PTR_CHR:
            case INC_VAL_CHR:
            case DEC_VAL_CHR: {
                char op = source[i];
                size_t j = i + 1;
                while (j < length && source[j] == op) {
                    ++j;
                }

                size_t count = j - i;
                if (op == INC_PTR_CHR) {
                    handleIncPtrChr(&buf, count);
                } else if (op == DEC_PTR_CHR) {
                    handleDecPtrChr(&buf, count);
                } else if (op == INC_VAL_CHR) {
                    handleIncValChr(&buf, count);
                } else if (op == DEC_VAL_CHR) {
                    handleDecValChr(&buf, count);
                }

                i = j - 1;
                break;
            }
            case OUTPUT_VAL_CHR:
                handleOutputValChr(&buf);
                break;
            case INPUT_VAL_CHR:
                handleInputValChr(&buf);
                break;
            case BLOCK_START_CHR:
                handleBlockStartChr(&buf, blockStack, &blockStackTop);
                break;
            case BLOCK_END_CHR:
                handleBlockEndChr(&buf, blockStack, &blockStackTop);
                break;
            default:
                // A comment
                break;
        }
    }

    emitRet(&buf);

    return codeBuffer;
}
