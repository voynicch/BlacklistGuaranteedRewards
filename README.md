# Blacklist Guaranteed Rewards

Source code for **Blacklist Guaranteed Rewards v0.1.0**, a lightweight ASI plugin for **Need for Speed: Most Wanted (2005) PC v1.3 / Black Edition**.

Nexus Mods page: https://www.nexusmods.com/needforspeedmostwanted2005/mods/330

## What it does

The plugin keeps all **6 Blacklist Bonus Markers** selectable after defeating a Blacklist rival. Reward selection remains manual; the plugin does not reveal hidden markers or automatically choose rewards.

## Source / release verification

The source used for the published v0.1.0 release is in:

`src/BlacklistGuaranteedRewards.cpp`

SHA-256 of the published `BlacklistGuaranteedRewards.asi`:

`3e56fdb87d392ad2accb16c5ddfcf11043bd32bab1df00fdb30dd55c8b0056bf`

The compiled `.asi` and intermediate `.lib` are intentionally not stored in this source repository. The binary release is distributed through Nexus Mods.

## Build

Use the **x86 Native Tools Command Prompt for Visual Studio** and run:

`build-windows.bat`

The script builds the DLL and copies it to `build\BlacklistGuaranteedRewards.asi`.

## Compatibility

- Need for Speed: Most Wanted (2005) PC v1.3 / Black Edition
- ASI Loader required
- Designed to coexist with common setups using Redux / Extra Options

## Technical notes

The plugin reapplies the four known Bonus Marker count bytes after startup so another ASI plugin does not overwrite the values later in initialization.

See `THIRD_PARTY_NOTICES.txt` for the public technical references used while researching the four marker-count addresses.

## Project status

**v0.1.0 is the stable/final baseline.**

A later experimental automation branch was abandoned after causing instability and is not part of this repository or the Nexus release.
