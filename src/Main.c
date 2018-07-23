#include <efi.h>
#include <efilib.h>
#include <efishellparm.h>
#include <stdbool.h>

#include "Constants.h"
#include "UefiIOWrapper.h"
#include "jit/JIT.h"

static void runProgram(CHAR8* source, UINTN size);

EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);
    initUefiIOWrapper(SystemTable);

    UINTN argc;
    CHAR16 **argv;
    argc = GetShellArgcArgv(ImageHandle, &argv);
    if (argc < 2) {
        Print(L"Usage: %s program.bf\n", argv[0]);
        return EFI_INVALID_PARAMETER;
    }

    EFI_STATUS status;

    EFI_GUID sfspGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_HANDLE *handles = NULL;
    UINTN handleCount = 0;
    status = uefi_call_wrapper(
            SystemTable->BootServices->LocateHandleBuffer, 5,
            ByProtocol, &sfspGuid, NULL, &handleCount, &handles);
    if (EFI_ERROR(status)) {
        Print(L"Could not acquire File System protocol\n");
        return status;
    }

    bool success = false;
    for (UINTN i = 0; i < (int) handleCount; ++i) {
        // Get FS
        EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs = NULL;
        status = uefi_call_wrapper(
                SystemTable->BootServices->HandleProtocol, 3,
                handles[i], &sfspGuid, (void **) &fs);
        if (EFI_ERROR(status)) {
            continue;
        }

        // Open volume root
        EFI_FILE_PROTOCOL *root = NULL;
        status = uefi_call_wrapper(
                fs->OpenVolume, 2,
                fs, &root);
        if (EFI_ERROR(status)) {
            continue;
        }

        // Try to open the file
        EFI_FILE_PROTOCOL *file = NULL;
        status = uefi_call_wrapper(
                root->Open, 5,
                root, &file, argv[1], EFI_FILE_MODE_READ, 0);
        if (EFI_ERROR(status)) {
            continue;
        }

        // Read the file
        UINTN bufferSize = MAX_FILE_SIZE;
        CHAR8 *buffer = (CHAR8*) AllocatePool(bufferSize);
        status = uefi_call_wrapper(
                file->Read, 3,
                file, &bufferSize, buffer);
        if (EFI_ERROR(status)) {
            continue;
        }

        // Close the volume
        status = uefi_call_wrapper(root->Close, 1, root);
        if (EFI_ERROR(status)) {
            Print(L"Could not close the volume\n");
            return status;
        }

        // Close the file
        status = uefi_call_wrapper(file->Close, 1, file);
        if (EFI_ERROR(status)) {
            Print(L"Could not close the file\n");
            return status;
        }

        runProgram(buffer, bufferSize);
        FreePool(buffer);
        success = true;

        break;
    }

    if (!success) {
        Print(L"Could not load specified file");
        return EFI_LOAD_ERROR;
    }

    return EFI_SUCCESS;
}

static void runProgram(CHAR8* source, UINTN size) {
    CHAR8 *prog = (CHAR8*) AllocatePool(MAX_PROGRAM_SIZE);
    CHAR8 *memory = (CHAR8*) AllocatePool(MEMORY_CELLS);
    ZeroMem(memory, MEMORY_CELLS);
    jitCompile(source, size, prog, memory)();
    FreePool(prog);
    FreePool(memory);
}
