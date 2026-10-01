# Changelog

## v0.1.1 — Conventional Win32 Clean Build

- Removed importless PEB walking and manual Kernel32 export resolution.
- Switched to a conventional PE import table.
- Restored normal `/GS` stack protection in the Visual Studio build.
- Added instruction-cache flushing after patch writes.
- Patch bytes are written only when a value differs.
- Replaced the permanent one-second loop with a finite startup retry window.
- Closed the worker-thread handle immediately after creation.
- Kept the same four NFSMW v1.3 marker-count addresses and the same gameplay result.

## v0.1.0

- Initial public release.
