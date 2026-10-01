# Source verification — v0.1.1

This repository contains the source code for **Blacklist Guaranteed Rewards v0.1.1 — Clean Build**.

The release ASI is built from:

`src/BlacklistGuaranteedRewards.cpp`

## Published ASI SHA-256

`4ca92c6d28a6b1c2ef256a5d7f241b5c2bc4689c18173c4f9c0c662f79eac070`

## Build

Open the **x86 Native Tools Command Prompt for Visual Studio** and run:

`build-windows.bat`

The expected output path is:

`build\BlacklistGuaranteedRewards.asi`

Compiler and linker versions can change the exact PE layout, so a local rebuild is not guaranteed to be byte-for-byte identical unless the same toolchain is used.

## Runtime behavior

v0.1.1 uses ordinary Windows imports and does not use the PEB-walking/manual Kernel32 export resolver from v0.1.0.

The plugin:

- modifies four known NFSMW v1.3 Bonus Marker count bytes in the running `speed.exe`;
- uses `VirtualProtect` only to make those target bytes writable while patching;
- flushes the instruction cache after writes;
- creates a short-lived startup worker thread and then exits it;
- does not access the network;
- does not download or update files;
- does not create persistence;
- does not delete user files;
- does not use a packer, obfuscation, or antivirus-bypass logic.

Because it is an unsigned ASI plugin that modifies game memory at runtime, antivirus products may still flag it heuristically. Users should review the public source and the exact detection rather than disabling antivirus protection globally.
