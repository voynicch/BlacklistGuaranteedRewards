# Blacklist Guaranteed Rewards

**Blacklist Guaranteed Rewards** is a lightweight ASI plugin for **Need for Speed: Most Wanted (2005)** that changes the Blacklist Bonus Marker reward system.

After defeating a Blacklist rival, the plugin allows you to claim **all 6 Bonus Marker rewards** instead of being limited to only two selections.

## Current Version

**v0.1.1 — Clean Build**

This version keeps the same in-game behavior as v0.1.0 while simplifying the plugin's Windows API usage and reducing patterns that may trigger antivirus heuristics.

## Features

- Allows all **6 Bonus Markers** to remain selectable after defeating a Blacklist rival.
- Reward selection remains fully manual.
- Does **not** reveal hidden markers.
- Does **not** automatically select the Pink Slip or Unique Performance reward.
- Designed to coexist with commonly used NFSMW setups such as **Redux 3.04** and **Extra Options**.

## Requirements

- **Need for Speed: Most Wanted (2005) PC**
- **An ASI Loader is required**
  - **Ultimate ASI Loader** is recommended.
  - Any compatible ASI loader should work.

## Compatibility

Tested with:

- Need for Speed: Most Wanted (2005) PC v1.3
- Black Edition
- Redux 3.04
- Extra Options
- Ultimate ASI Loader

The v0.1.1 build was also tested after a Windows restart with **Microsoft Defender active**. No detection was observed during that test and the mod continued to work normally.

It may work with the base game, but it has not been tested.

## Antivirus / Windows Defender Notice

Because this is an **unsigned ASI plugin that modifies the game's memory at runtime**, Microsoft Defender or another antivirus product may occasionally flag, quarantine, or block the file.

This does **not automatically mean the file is malicious**. Game mods, trainers, injectors, hooks, and ASI plugins can trigger heuristic detections because some of the low-level techniques they use are also used by malicious software.

This plugin needs to change a few bytes inside the running `speed.exe` process in order to modify the Bonus Marker selection limit. To do that, it uses normal Windows memory-protection APIs such as `VirtualProtect` and writes directly to known game-memory addresses.

Antivirus engines may consider behaviors like these suspicious:

- Loading an unsigned `.asi` module into another program.
- Changing executable memory protection.
- Modifying bytes in a running game's memory.
- Creating a worker thread during game startup.
- Using fixed memory addresses for runtime patches.

### Changes made in v0.1.1

The original v0.1.0 used an unusual **importless / PEB-based Windows API resolver**. Although this was not malicious, that implementation could resemble techniques used by malware and was more likely to trigger heuristic antivirus detections.

Version **v0.1.1** removes that implementation and uses conventional Windows imports instead.

The clean build also:

- Removes the manual PEB walking / Kernel32 export resolver.
- Uses a normal Windows PE import table.
- Restores normal compiler stack protection for Visual Studio builds.
- Removes the permanent background patch loop.
- Flushes the CPU instruction cache after patching.
- Does not use a packer or obfuscation.
- Does not access the network.
- Does not download or update anything.
- Does not create persistence mechanisms.
- Does not delete user files.
- Does not contain anti-analysis or antivirus-bypass code.

These changes reduce unnecessary heuristic triggers, but **no antivirus result can be guaranteed permanently**, because antivirus signatures and heuristic rules can change over time.

### If Windows Defender blocks the file

Do **not** disable your antivirus globally.

First check:

**Windows Security → Virus & threat protection → Protection history**

Review the exact detection and affected file.

You can also inspect the public source code and verify the SHA-256 hash of the release before deciding whether you trust the file.

## Installation

1. Make sure a compatible ASI Loader is installed.
2. Copy:

   `BlacklistGuaranteedRewards.asi`

   to:

   `Need for Speed Most Wanted\scripts\`

3. Start the game normally.

## Uninstallation

Delete:

`BlacklistGuaranteedRewards.asi`

from the game's `scripts` folder.

## How It Works

The original game limits the player to choosing only two Bonus Markers after defeating a Blacklist rival.

This plugin changes the relevant Bonus Marker count values so that all six markers remain selectable.

The plugin does not determine what reward is hidden behind each marker and does not automate the player's choices.

## SHA-256

### BlacklistGuaranteedRewards.asi — v0.1.1

`4ca92c6d28a6b1c2ef256a5d7f241b5c2bc4689c18173c4f9c0c662f79eac070`

## Source Code

The complete source code and build information are available in this repository.

Public source code is provided so users, antivirus vendors, and mod-hosting staff can inspect exactly what the plugin does.

## Version History

### v0.1.1 — Clean Build

- Reworked plugin initialization to improve antivirus compatibility.
- Removed the importless / PEB-based API resolver.
- Switched to conventional Win32 imports.
- Removed the permanent background patch loop.
- Added safer runtime patch handling and instruction-cache flushing.
- Gameplay functionality remains unchanged.

### v0.1.0

- Initial public release.
- Allowed all 6 Blacklist Bonus Markers to remain selectable.

## Credits

- **Voynicch** — concept, testing, project direction and release.
- **OpenAI / ChatGPT** — AI-assisted programming and documentation.
- **ThirteenAG** — Ultimate ASI Loader, recommended external dependency.

No third-party game assets are distributed with this project.

## Disclaimer

This is an unofficial community-made modification for **Need for Speed: Most Wanted (2005)**.

Use it at your own risk and keep backups of important game files and save data.
