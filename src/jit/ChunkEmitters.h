#ifndef UEFI_JITFUCK_CHUNKEMITTERS_H
#define UEFI_JITFUCK_CHUNKEMITTERS_H

#include <Types.h>

void emitByte(Byte **buf, Byte x);

void emitShort(Byte **buf, short x);

void emitInt(Byte **buf, int x);

void emitLong(Byte **buf, long x);

#endif //UEFI_JITFUCK_CHUNKEMITTERS_H
