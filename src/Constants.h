#ifndef UEFI_JITFUCK_CONSTANTS_H
#define UEFI_JITFUCK_CONSTANTS_H

#include "Types.h"

#define READ_BUFFER_SIZE 4096
#define MAX_FILE_SIZE 4096000
#define MAX_PROGRAM_SIZE 4096000
#define MEMORY_CELLS 30000

// Characters used in the language source code
#define INC_PTR_CHR '>'
#define DEC_PTR_CHR '<'
#define INC_VAL_CHR '+'
#define DEC_VAL_CHR '-'
#define OUTPUT_VAL_CHR '.'
#define INPUT_VAL_CHR ','
#define BLOCK_START_CHR '['
#define BLOCK_END_CHR ']'

#endif //UEFI_JITFUCK_CONSTANTS_H
