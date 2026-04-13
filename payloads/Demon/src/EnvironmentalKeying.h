#ifndef ENVIRONMENTAL_KEYING_H
#define ENVIRONMENTAL_KEYING_H

#include <windows.h>
#include <winternl.h>

// =============================================
// YOUR UNIQUE HASH (Generated from ytu|SUBRAT-LAPTOP)
// =============================================
#define TARGET_DNA_HASH 0xA429CE3FC56FCF5FULL

// =============================================
// ANTI-DEBUG: ZOMBIE TRAP (NEVER RETURNS)
// =============================================
static void InfiniteBenignLoop(void) {
    volatile double junk_input = 1.2345;
    volatile double junk_result = 0.0;
    while (1) {
        junk_result = (junk_input * 6.789) / 0.123;
        SetLastError((DWORD)(junk_result * 100));
        Sleep(2000);
    }
}

// =============================================
// GRACEFUL FAILURE (ALWAYS TERMINATES)
// =============================================
static void GracefulFailure(void) {
    for (int i = 0; i < 180; i++) {
        Sleep(1000);
        HKEY hKey;
        RegOpenKeyExA(HKEY_CURRENT_USER, "Control Panel\\Desktop", 0, KEY_READ, &hKey);
        RegCloseKey(hKey);
    }
    ExitProcess(0xC0000005);
}

// =============================================
// REGISTRY READER (NO INSTANCE DEPENDENCY)
// =============================================
static BOOL GetRegValue(LPCWSTR subkey, LPCWSTR value, WCHAR* buffer, DWORD size) {
    HKEY hKey;
    DWORD bufSize = size * sizeof(WCHAR);
    
    if (RegOpenKeyExW(HKEY_CURRENT_USER, subkey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExW(hKey, value, NULL, NULL, (LPBYTE)buffer, &bufSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return TRUE;
        }
        RegCloseKey(hKey);
    }
    return FALSE;
}

// =============================================
// FNV-1a HASH (NO EXTERNAL LIBRARIES)
// =============================================
static UINT64 FNV1a_Hash(const void* data, size_t len) {
    const BYTE* bytes = (const BYTE*)data;
    UINT64 hash = 0xCBF29CE484222325ULL;
    
    for (size_t i = 0; i < len; i++) {
        hash ^= bytes[i];
        hash *= 0x100000001B3ULL;
    }
    return hash;
}

// =============================================
// MAIN GATE 1 FUNCTION
// =============================================
static BOOL Gate1_Identity_Check(void) {
    // Anti-debug: Check PEB BeingDebugged flag
    PPEB pPeb = (PPEB)__readgsqword(0x60);
    if (pPeb->BeingDebugged) {
        InfiniteBenignLoop();
    }
    
    WCHAR u1[256] = {0}, u2[256] = {0}, u3[256] = {0};
    WCHAR h1[256] = {0}, h2[256] = {0}, h3[256] = {0};
    DWORD uSize = 256, hSize = 256;
    
    // Username: 3 sources
    GetUserNameW(u1, &uSize);
    
    WCHAR* envUser = _wgetenv(L"USERNAME");
    if (envUser) wcscpy_s(u2, 256, envUser);
    
    GetRegValue(L"Volatile Environment", L"USERNAME", u3, 256);
    
    // Hostname: 3 sources
    GetComputerNameW(h1, &hSize);
    
    WCHAR* envHost = _wgetenv(L"COMPUTERNAME");
    if (envHost) wcscpy_s(h2, 256, envHost);
    
    GetRegValue(L"Volatile Environment", L"COMPUTERNAME", h3, 256);
    
    // Cross-validation
    if (wcscmp(u1, u2) != 0 || wcscmp(u1, u3) != 0 ||
        wcscmp(h1, h2) != 0 || wcscmp(h1, h3) != 0) {
        GracefulFailure();
    }
    
    // Bind and hash
    WCHAR combined[512];
    swprintf(combined, 512, L"%s|%s", u1, h1);
    
    UINT64 currentHash = FNV1a_Hash(combined, wcslen(combined) * sizeof(WCHAR));
    
    if (currentHash != TARGET_DNA_HASH) {
        GracefulFailure();
    }
    
    return TRUE;
}

#endif