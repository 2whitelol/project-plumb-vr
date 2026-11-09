# Project Plumb VR — Handoff for Manus

This file is the shared handoff so Manus can continue Project Plumb VR when Muse's weekly limit runs out. Muse and Manus do NOT share a live environment — Manus runs its own cloud computer. The shared ground is this repo plus this file. Read this file first, then the repo docs, then pick up the next step below.

## Project
Native Super Mario Odyssey (1.0.0) VR port for Meta Quest 3 and Quest 3S. Android ARM64, OpenXR, installable APK. No WebXR, no Unity, no PC required. Demo scope is BOTH parts together: Part 1 opening Bowser encounter → fall → first meeting with Cappy, and Part 2 wedding interruption → Cap Kingdom → end after defeating Topper. Long-term goal: the full game.

## Public repo (the shared source of truth)
https://github.com/2whitelol/project-plumb-vr
- MIT license, holder "Project Plumb VR".
- Contains guides/docs only so far (README, CONTRIBUTING, docs/ x6). No VR/OpenXR code is published yet — code uploads are spaced out in real stages as milestones are actually built/tested. Do NOT backdate commits or fake history.
- Discord: https://discord.gg/XUtAtkdumP (server ID 1569389936359112725)

## Current state (2026-11-09, verified by Muse)
- OdysseyDecomp clone (HEAD 696db2ff), setup.py done, build.py passed (970/970 steps), tools/check OK.
- Decomp matching: 19,185 matching (25.866% by count) and rising — Cappy 9/9, Koopa/Bowser 19/19, intro camera 12/13, KoopaChurch 24/24, plus autonomous matching rounds. Local clone only, no PR submitted, nothing pushed to OdysseyDecomp.
- Milestone 3 test APK BUILT: vr-foundation/build-manual/plumbvr-foundation-test.apk, 3.4 MB, package com.plumbvr.foundation, arm64-v8a, OpenXR 1.1.65, stereo rotating cube driven by head poses. Built by hand with NDK clang + aapt2 — Gradle daemon fails on Muse's VM for every invocation. Debug-signed only. NEVER run on a Quest yet — needs sideloading and testing on real hardware.
- RomFS extracted locally from White's own SMO 1.0.0 dump: 4,636 files, 5.2 GiB. ObjectData 3.6 GB (models), SoundData 1.24 GB (music/audio). Asset catalogues in extracted/asset-inventory/ (player-models, boss-models, music-audio, stages-textures, ui-misc).
- Stacker == Topper is still UNVERIFIED. Strong circumstantial case by elimination (CapThrower=Rango, BombTail=Hariet, FireBlower=Spewart, leaving Stacker), but do NOT assert it in code, PRs, or docs until RomFS stage data proves it.

## Rules (from White)
- Quality over speed. Take time, work hardest, make it as best as it could be.
- Use OdysseyDecomp code as the base — do NOT build from scratch. Use it as a submodule/dependency, never copy its files into the repo (it has no LICENSE file).
- Cappy controls: grab with grip button, throw by releasing grip in a throw motion. NOT a button press to throw. Player's hands are Mario's gloves, inside-out body tracking.
- Target hardware: Quest 3 AND Quest 3S (same Snapdragon XR2 Gen 2, same APK).
- APK signing is an end-of-demo step only. Do NOT compile/sign the final demo APK yet.
- NEVER publish or share: prod.keys, ROM/NSP/NSO/NCA, RomFS/ExeFS, or Nintendo models/audio/textures/assets. The repo must stay clean of them.
- Space out code uploads in real stages as milestones are actually built/tested. Do NOT backdate commits or fake a 2-year history.

## Next steps for Manus
1. Clone the public repo, read README + docs/.
2. Continue decomp matching in the OdysseyDecomp clone (matching functions one by one, verify with tools/check, update file_list.yml) — demo-relevant areas first: ScenarioStartCamera big functions, Cap Kingdom NPCs/objects, PlayerCap/HackCap.
3. Continue Milestone 3 → Milestone 4: get the test APK running on a real Quest (sideload, logcat), then start game integration.
4. Report results back to White with verified facts only — what matched, what built, what was actually tested on hardware, and what is still blocked.

## Toolchain (Muse's VM, for reference — Manus has its own)
OpenXR SDK 1.1.65, Gradle 8.14.3, Temurin JDK 17.0.21, Android SDK NDK r27c, static ninja 1.13.1 / ccache 4.10.2. VM: Ubuntu 24.04.5, 7.7 GiB RAM, no GPU — cannot run a Quest APK or the game visually; Quest screenshots from White are the visual test route.
