#include "ChunkEmitters.h"

void emitByte(Byte **buf, Byte x) {
    *((*buf)++) = x;
}

void emitShort(Byte **buf, short x) {
    *((short *) *buf) = x;
    *buf += 2;
}

void emitInt(Byte **buf, int x) {
    *((int *) *buf) = x;
    *buf += 4;
}

void emitLong(Byte **buf, long x) {
    *((long *) *buf) = x;
    *buf += 8;
}
