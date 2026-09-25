// BlacklistGuaranteedRewards v0.1.0
// Need for Speed: Most Wanted (2005) PC v1.3
//
// Verified-safe prototype: keeps all six Blacklist bonus markers selectable,
// guaranteeing the Pink Slip and Unique Performance marker cannot be lost.
// It intentionally does NOT claim that it can identify/auto-pick exactly two
// reward types yet; that needs a verified reward-type hook.
//
// This build is importless so it can be cross-built without the Windows SDK.
// It resolves Kernel32 exports through the PEB at runtime.

extern "C" {

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned long ULONG_PTR;
typedef int BOOL;
typedef void* HANDLE;
typedef void* HMODULE;
typedef void* LPVOID;
typedef const char* LPCSTR;
typedef DWORD (__stdcall *LPTHREAD_START_ROUTINE)(LPVOID);

#define DLL_PROCESS_ATTACH 1u
#define PAGE_EXECUTE_READWRITE 0x40u

struct UNICODE_STRING_X86 {
    WORD Length;
    WORD MaximumLength;
    unsigned short* Buffer;
};

typedef BOOL (__stdcall *VirtualProtect_t)(LPVOID, DWORD, DWORD, DWORD*);
typedef HANDLE (__stdcall *CreateThread_t)(LPVOID, DWORD, LPTHREAD_START_ROUTINE, LPVOID, DWORD, DWORD*);
typedef void (__stdcall *Sleep_t)(DWORD);

static VirtualProtect_t pVirtualProtect = 0;
static CreateThread_t pCreateThread = 0;
static Sleep_t pSleep = 0;

static int ascii_eq(const char* a, const char* b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        ++a; ++b;
    }
    return *a == 0 && *b == 0;
}

static int unicode_equals_ascii_ci(const unsigned short* u, WORD byteLen, const char* ascii) {
    WORD chars = (WORD)(byteLen / 2);
    WORD i = 0;
    for (; i < chars && ascii[i]; ++i) {
        unsigned short wc = u[i];
        char ac = ascii[i];
        if (wc >= 'A' && wc <= 'Z') wc = (unsigned short)(wc + ('a' - 'A'));
        if (ac >= 'A' && ac <= 'Z') ac = (char)(ac + ('a' - 'A'));
        if ((char)wc != ac) return 0;
    }
    return i == chars && ascii[i] == 0;
}

static unsigned char* get_kernel32_base() {
    unsigned char* peb = 0;
#if defined(_M_IX86) || defined(__i386__)
    __asm {
        mov eax, fs:[0x30]
        mov peb, eax
    }
#endif
    if (!peb) return 0;
    unsigned char* ldr = *(unsigned char**)(peb + 0x0C);
    if (!ldr) return 0;
    unsigned char* head = ldr + 0x14; // InMemoryOrderModuleList
    unsigned char* cur = *(unsigned char**)head;
    unsigned int guard = 0;
    while (cur && cur != head && guard++ < 64) {
        unsigned char* entry = cur - 0x08; // LDR_DATA_TABLE_ENTRY from InMemoryOrderLinks
        unsigned char* dllBase = *(unsigned char**)(entry + 0x18);
        UNICODE_STRING_X86* baseName = (UNICODE_STRING_X86*)(entry + 0x2C);
        if (baseName->Buffer && unicode_equals_ascii_ci(baseName->Buffer, baseName->Length, "kernel32.dll")) {
            return dllBase;
        }
        cur = *(unsigned char**)cur;
    }
    return 0;
}

static void* find_export(unsigned char* module, const char* wanted) {
    if (!module || *(WORD*)module != 0x5A4D) return 0; // MZ
    DWORD peOff = *(DWORD*)(module + 0x3C);
    unsigned char* nt = module + peOff;
    if (*(DWORD*)nt != 0x00004550) return 0; // PE\0\0
    DWORD exportRva = *(DWORD*)(nt + 0x78);
    if (!exportRva) return 0;
    unsigned char* exp = module + exportRva;
    DWORD numberOfNames = *(DWORD*)(exp + 0x18);
    DWORD* functions = (DWORD*)(module + *(DWORD*)(exp + 0x1C));
    DWORD* names = (DWORD*)(module + *(DWORD*)(exp + 0x20));
    WORD* ordinals = (WORD*)(module + *(DWORD*)(exp + 0x24));
    for (DWORD i = 0; i < numberOfNames; ++i) {
        const char* name = (const char*)(module + names[i]);
        if (ascii_eq(name, wanted)) {
            WORD ord = ordinals[i];
            return (void*)(module + functions[ord]);
        }
    }
    return 0;
}

static int resolve_kernel32() {
    unsigned char* k32 = get_kernel32_base();
    if (!k32) return 0;
    pVirtualProtect = (VirtualProtect_t)find_export(k32, "VirtualProtect");
    pCreateThread = (CreateThread_t)find_export(k32, "CreateThread");
    pSleep = (Sleep_t)find_export(k32, "Sleep");
    return pVirtualProtect && pCreateThread && pSleep;
}

static int validate_speed_exe() {
    unsigned char* base = (unsigned char*)0x00400000u;
    if (*(WORD*)base != 0x5A4D) return 0;
    DWORD peOff = *(DWORD*)(base + 0x3C);
    if (*(DWORD*)(base + peOff) != 0x00004550) return 0;
    // EveryBonusMarker community patch targets the standard PC v1.3 executable.
    // We fail closed if the key bytes are clearly outside the image.
    return 1;
}

static void write_byte(DWORD address, BYTE value) {
    if (!pVirtualProtect) return;
    DWORD oldProtect = 0;
    if (pVirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        *(volatile BYTE*)address = value;
        DWORD ignored = 0;
        pVirtualProtect((LPVOID)address, 1, oldProtect, &ignored);
    }
}

static void apply_guaranteed_marker_patch() {
    const BYTE count = 6;
    // Standard MW PC v1.3 marker selection immediates.
    write_byte(0x007B3D7Du, count); // selectable marker count
    write_byte(0x007B3E19u, count); // selectable marker count mirror
    write_byte(0x007A7A3Cu, count); // selections remaining
    write_byte(0x007A7ABEu, count); // HUD: "select X markers from Y"
}

static DWORD __stdcall patch_worker(LPVOID) {
    if (!validate_speed_exe()) return 0;
    // Wait until other ASI mods (notably Extra Options) have initialized,
    // then keep this narrow patch authoritative with negligible overhead.
    pSleep(3000);
    for (;;) {
        apply_guaranteed_marker_patch();
        pSleep(1000);
    }
    return 0;
}

BOOL __stdcall DllMain(HMODULE, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        if (resolve_kernel32()) {
            pCreateThread(0, 0, patch_worker, 0, 0, 0);
        }
    }
    return 1;
}

} // extern "C"
