#ifndef UEFI_JITFUCK_JIT_H
#define UEFI_JITFUCK_JIT_H

#include <stddef.h>

typedef void(*CompiledFunction)();

CompiledFunction
jitCompile(const unsigned char *source, size_t length, void *codeBuffer,
           void *programMemory);

#endif //UEFI_JITFUCK_JIT_H
