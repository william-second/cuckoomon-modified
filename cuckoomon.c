#include <stdio.h>
#include <windows.h>
#include "ntapi.h"
#include "hooks.h"
#include "hooking.h"

#define HOOK(library, funcname) {#library, #funcname, &New_##funcname, \
    (void **) &Old_##funcname}

static hook_t g_hooks[] = {

    //
    // File Hooks
    //

    HOOK(ntdll, NtCreateFile),
    HOOK(ntdll, NtOpenFile),
    HOOK(ntdll, NtReadFile),
    HOOK(ntdll, NtWriteFile),

    // lowest variant of MoveFile()
    HOOK(kernel32, MoveFileWithProgressW),

    // perhaps go for NtSetInformationFile() later
    HOOK(kernel32, DeleteFileW),

    //
    // Registry Hooks
    //
    // Note: Most, if not all, of the Registry API go natively from both the
    // A as well as the W versions. In other words, we have to hook all the
    // ascii *and* unicode APIs of those functions.
    //

    HOOK(advapi32, RegOpenKeyExA),
    HOOK(advapi32, RegOpenKeyExW),

    HOOK(advapi32, RegCreateKeyExA),
    HOOK(advapi32, RegCreateKeyExW),

    // Note that RegDeleteKeyEx() is available for 64bit XP/Vista+
    HOOK(advapi32, RegDeleteKeyA),
    HOOK(advapi32, RegDeleteKeyW),

    // RegEnumKeyA() calls RegEnumKeyExA(), but RegEnumKeyW() does *not*
    // call RegEnumKeyExW()
    HOOK(advapi32, RegEnumKeyW),
    HOOK(advapi32, RegEnumKeyExA),
    HOOK(advapi32, RegEnumKeyExW),

    HOOK(advapi32, RegEnumValueA),
    HOOK(advapi32, RegEnumValueW),

    HOOK(advapi32, RegSetValueExA),
    HOOK(advapi32, RegSetValueExW),

    HOOK(advapi32, RegQueryValueExA),
    HOOK(advapi32, RegQueryValueExW),

    HOOK(advapi32, RegDeleteValueA),
    HOOK(advapi32, RegDeleteValueW),
};

void set_hooks()
{
    for (int i = 0; i < ARRAYSIZE(g_hooks); i++) {
        hook_api(&g_hooks[i]);
    }
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
    if(dwReason == DLL_PROCESS_ATTACH) {
        set_hooks();
    }
    return TRUE;
}
