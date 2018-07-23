#ifndef UEFI_JITFUCK_SPECIALBYTEEMITTERS_H
#define UEFI_JITFUCK_SPECIALBYTEEMITTERS_H

#include "Types.h"

void emitREX(Byte **buf, Byte w, Byte r, Byte x, Byte b);

void emitModRM(Byte **buf, Byte mod, Byte reg, Byte rm);

#endif //UEFI_JITFUCK_SPECIALBYTEEMITTERS_H
