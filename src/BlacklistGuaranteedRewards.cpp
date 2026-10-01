// Blacklist Guaranteed Rewards v0.1.1
// Need for Speed: Most Wanted (2005) PC v1.3 / Black Edition
//
// Conventional Win32 build:
// - Uses a normal PE import table for required Kernel32 APIs.
// - No PEB walking or manual export resolution.
// - Applies the four known marker-count patches only during a short startup window.
// - Does not reveal reward locations or automatically choose rewards.

extern "C" {

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned long SIZE_T;
typedef int BOOL;
typedef void* HANDLE;
typedef void* HMODULE;
typedef void* LPVOID;
typedef const void* LPCVOID;
typedef DWORD (__stdcall *LPTHREAD_START_ROUTINE)(LPVOID);

#define DLL_PROCESS_ATTACH 1u
#define PAGE_EXECUTE_READWRITE 0x40u

#if defined(BGR_CROSS_BUILD) && defined(__clang__)
// The Linux cross-build used to produce the release binary links against a
// minimal import library. These aliases point at normal PE IAT entries; they
// do not resolve APIs dynamically at runtime.
typedef BOOL   (__stdcall *VirtualProtectFn)(LPVOID, SIZE_T, DWORD, DWORD*);
typedef HANDLE (__stdcall *CreateThreadFn)(LPVOID, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, DWORD*);
typedef void   (__stdcall *SleepFn)(DWORD);
typedef BOOL   (__stdcall *CloseHandleFn)(HANDLE);
typedef BOOL   (__stdcall *DisableThreadLibraryCallsFn)(HMODULE);
typedef BOOL   (__stdcall *FlushInstructionCacheFn)(HANDLE, LPCVOID, SIZE_T);
typedef HANDLE (__stdcall *GetCurrentProcessFn)();

extern void* bgr_iat_VirtualProtect           __asm__("__imp__VirtualProtect");
extern void* bgr_iat_CreateThread              __asm__("__imp__CreateThread");
extern void* bgr_iat_Sleep                     __asm__("__imp__Sleep");
extern void* bgr_iat_CloseHandle               __asm__("__imp__CloseHandle");
extern void* bgr_iat_DisableThreadLibraryCalls __asm__("__imp__DisableThreadLibraryCalls");
extern void* bgr_iat_FlushInstructionCache     __asm__("__imp__FlushInstructionCache");
extern void* bgr_iat_GetCurrentProcess         __asm__("__imp__GetCurrentProcess");

#define BGR_VirtualProtect(a,b,c,d) ((VirtualProtectFn)bgr_iat_VirtualProtect)((a),(b),(c),(d))
#define BGR_CreateThread(a,b,c,d,e,f) ((CreateThreadFn)bgr_iat_CreateThread)((a),(b),(c),(d),(e),(f))
#define BGR_Sleep(a) ((SleepFn)bgr_iat_Sleep)((a))
#define BGR_CloseHandle(a) ((CloseHandleFn)bgr_iat_CloseHandle)((a))
#define BGR_DisableThreadLibraryCalls(a) ((DisableThreadLibraryCallsFn)bgr_iat_DisableThreadLibraryCalls)((a))
#define BGR_FlushInstructionCache(a,b,c) ((FlushInstructionCacheFn)bgr_iat_FlushInstructionCache)((a),(b),(c))
#define BGR_GetCurrentProcess() ((GetCurrentProcessFn)bgr_iat_GetCurrentProcess)()

// clang-cl /GS normally obtains these from the MSVC runtime. The cross-build
// supplies the minimal equivalents so the resulting ASI remains CRT-free.
typedef unsigned long UINTPTR_T;
UINTPTR_T __security_cookie = 0x6A09E667u;
void __fastcall __security_check_cookie(UINTPTR_T cookie) {
    if (cookie != __security_cookie) {
        __builtin_trap();
    }
}
#else
__declspec(dllimport) BOOL   __stdcall VirtualProtect(LPVOID, SIZE_T, DWORD, DWORD*);
__declspec(dllimport) HANDLE __stdcall CreateThread(LPVOID, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, DWORD*);
__declspec(dllimport) void   __stdcall Sleep(DWORD);
__declspec(dllimport) BOOL   __stdcall CloseHandle(HANDLE);
__declspec(dllimport) BOOL   __stdcall DisableThreadLibraryCalls(HMODULE);
__declspec(dllimport) BOOL   __stdcall FlushInstructionCache(HANDLE, LPCVOID, SIZE_T);
__declspec(dllimport) HANDLE __stdcall GetCurrentProcess();

#define BGR_VirtualProtect VirtualProtect
#define BGR_CreateThread CreateThread
#define BGR_Sleep Sleep
#define BGR_CloseHandle CloseHandle
#define BGR_DisableThreadLibraryCalls DisableThreadLibraryCalls
#define BGR_FlushInstructionCache FlushInstructionCache
#define BGR_GetCurrentProcess GetCurrentProcess
#endif

struct IMAGE_DOS_HEADER_MIN {
    WORD e_magic;
    BYTE pad[58];
    DWORD e_lfanew;
};

static BOOL validate_speed_exe() {
    const BYTE* base = (const BYTE*)0x00400000u;
    const IMAGE_DOS_HEADER_MIN* dos = (const IMAGE_DOS_HEADER_MIN*)base;
    if (dos->e_magic != 0x5A4Du) return 0; // MZ
    if (dos->e_lfanew < 0x40u || dos->e_lfanew > 0x1000u) return 0;
    const DWORD signature = *(const DWORD*)(base + dos->e_lfanew);
    return signature == 0x00004550u; // PE\0\0
}

static BOOL write_byte_if_needed(DWORD address, BYTE value) {
    volatile BYTE* target = (volatile BYTE*)address;
    if (*target == value) return 1;

    DWORD oldProtect = 0;
    if (!BGR_VirtualProtect((LPVOID)address, 1u, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        return 0;
    }

    *target = value;

    DWORD ignored = 0;
    BGR_VirtualProtect((LPVOID)address, 1u, oldProtect, &ignored);
    BGR_FlushInstructionCache(BGR_GetCurrentProcess(), (LPCVOID)address, 1u);
    return 1;
}

static void apply_guaranteed_marker_patch() {
    const BYTE count = 6;

    // Standard NFSMW PC v1.3 Blacklist marker-selection immediates.
    write_byte_if_needed(0x007B3D7Du, count); // selectable marker count
    write_byte_if_needed(0x007B3E19u, count); // selectable marker count mirror
    write_byte_if_needed(0x007A7A3Cu, count); // selections remaining
    write_byte_if_needed(0x007A7ABEu, count); // HUD: select X markers from Y
}

static DWORD __stdcall patch_worker(LPVOID) {
    if (!validate_speed_exe()) return 0;

    // Let the game and other ASI plugins initialize first. Reapply during a
    // short startup window to handle common initialization-order races, then
    // exit instead of keeping a permanent background patch loop alive.
    BGR_Sleep(3000u);
    for (int attempt = 0; attempt < 8; ++attempt) {
        apply_guaranteed_marker_patch();
        if (attempt != 7) BGR_Sleep(1000u);
    }
    return 0;
}

BOOL __stdcall DllMain(HMODULE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        BGR_DisableThreadLibraryCalls(module);
        HANDLE worker = BGR_CreateThread(0, 0, patch_worker, 0, 0, 0);
        if (worker) BGR_CloseHandle(worker);
    }
    return 1;
}

} // extern "C"
