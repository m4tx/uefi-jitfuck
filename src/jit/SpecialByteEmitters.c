#include <assert.h>

#include "Opcodes.h"
#include "SpecialByteEmitters.h"
#include "ChunkEmitters.h"

void emitREX(Byte **buf, Byte w, Byte r, Byte x, Byte b) {
    assert((w & ~1u) == 0);
    assert((r & ~1u) == 0);
    assert((x & ~1u) == 0);
    assert((b & ~1u) == 0);

    Byte toEmit = (Byte)
            ((REX_PREFIX << 4u) | (w << 3u) | (r << 2u) | (x << 1u) | b);
    emitByte(buf, toEmit);
}

void emitModRM(Byte **buf, Byte mod, Byte reg, Byte rm) {
    assert((mod & ~0b11u) == 0);
    assert((reg & ~0b111u) == 0);
    assert((rm & ~0b111u) == 0);

    emitByte(buf, (mod << 6u) | (reg << 3u) | rm);
}
