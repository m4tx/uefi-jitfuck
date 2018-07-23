#ifndef UEFI_JITFUCK_UEFIIOWRAPPER_H
#define UEFI_JITFUCK_UEFIIOWRAPPER_H

#include <efi.h>
#include <efilib.h>

void initUefiIOWrapper(EFI_SYSTEM_TABLE *SystemTable);

void printChar(char ch);

char readChar();


#endif //UEFI_JITFUCK_UEFIIOWRAPPER_H
