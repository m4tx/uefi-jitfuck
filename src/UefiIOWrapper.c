#include <efi.h>
#include <assert.h>

#include "Constants.h"
#include "UefiIOWrapper.h"

static EFI_SYSTEM_TABLE *GlobalSystemTable = NULL;

char readBuffer[READ_BUFFER_SIZE];
unsigned readBufferPos = 0;
unsigned readBufferReturnPos = 0;

void initUefiIOWrapper(EFI_SYSTEM_TABLE *SystemTable) {
    GlobalSystemTable = SystemTable;
}

void printChar(char ch) {
    assert(GlobalSystemTable != NULL);

    Print(L"%c", ch);
}

// Wrapper over ConIn->ReadKeyStroke that mimics the behavior of stdio's
// getchar() function
char readChar() {
    assert(GlobalSystemTable != NULL);

    // If a whole line was already returned, read a new one
    if (readBufferPos == readBufferReturnPos) {
        readBufferPos = 0;
        readBufferReturnPos = 0;

        EFI_INPUT_KEY key;

        char ch;
        do {
            EFI_STATUS status = uefi_call_wrapper(
                    GlobalSystemTable->ConIn->ReadKeyStroke, 2,
                    GlobalSystemTable->ConIn, &key);
            while (status == EFI_NOT_READY) {
                WaitForSingleEvent(GlobalSystemTable->ConIn->WaitForKey, 0);
                status = uefi_call_wrapper(
                        GlobalSystemTable->ConIn->ReadKeyStroke, 2,
                        GlobalSystemTable->ConIn, &key);
            }

            ch = (char) key.UnicodeChar;
            if (ch == '\b') {
                // Backspace
                if (readBufferPos > 0) {
                    --readBufferPos;
                    // Move the cursor left
                    Print(L"%c", ch);
                }
            } else {
                readBuffer[readBufferPos++] = ch;
                // Print the character
                Print(L"%c", ch);
            }
        } while (ch != '\r');

        // Overwrite '\r' with '\n'
        readBuffer[readBufferPos - 1] = '\n';
        // Go to a new line if enter was pressed
        Print(L"\n");
    }

    return readBuffer[readBufferReturnPos++];
}
