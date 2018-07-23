#ifndef UEFI_JITFUCK_TYPES_H
#define UEFI_JITFUCK_TYPES_H

typedef unsigned char Byte;

enum Register {
    RAX = 0,
    RBX = 3,
    RCX = 1,
    RDX = 2,
    RSI = 6,
    RDI = 7,
    RBP = 5,
    RSP = 4
};

#endif //UEFI_JITFUCK_TYPES_H
