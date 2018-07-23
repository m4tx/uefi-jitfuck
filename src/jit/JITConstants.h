#ifndef UEFI_JITFUCK_JITCONSTANTS_H
#define UEFI_JITFUCK_JITCONSTANTS_H

// Some instruction lengths
#define CMP_REG_IMM8_LEN 3
#define JMP_REL32OFF_LEN 5
#define JE_REL32OFF_LEN 6
#define BLOCK_START_LEN (CMP_REG_IMM8_LEN + JE_REL32OFF_LEN)

// The register where the current cell pointer is held
#define POINTER_REGISTER RSI

#endif //UEFI_JITFUCK_JITCONSTANTS_H
