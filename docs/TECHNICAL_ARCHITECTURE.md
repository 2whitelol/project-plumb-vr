# PROJECT PLUMB VR — TECHNICAL_ARCHITECTURE.md

Last updated: 2026-11-09

## Goal
Native Android ARM64 OpenXR game on Meta Quest 3, built from Super Mario Odyssey 1.0.0 decompilation work. No WebXR, no browser game, no flat-screen spectator app.

## Source base
- OdysseyDecomp (MonsterDruide1): WIP decomp of SMO 1.0.0, ~19% of .o entries have source (597/3,126, snapshot 2026-09-29)
- Target platform for the decomp itself is Nintendo Switch (NX64 toolchain, musl, clang/lld from OdysseyDecompToolsCache). It does NOT target Android.
- That means the decomp is reference + partial code, not a drop-in Android game engine. Porting requires a new Android/OpenXR host that reuses decompiled logic where it exists and reimplements missing systems.

## Key systems in the decomp (present)
- src/Demo/DemoPeachWedding.cpp/.h — wedding demo actor
- src/Scene/FirstSequenceScene.cpp (130 lines)
- src/Sequence/HakoniwaSequence.cpp — drawMain() uses docked/handheld render buffers
- src/Player/PlayerJudgeCameraSubjective.cpp — subjective/first-person camera hook: al::initSubjectiveCameraNoSave, isTriggerCameraSubjective(), checkNoCollisionForPlayerSubjectiveCamera
- lib/al/Library/Camera/ — CameraPoser, CameraPoseUpdater, CameraTicket, etc.
- Player/Cap files: PlayerCapFunction, HackCap, etc.

## Known gaps (snapshot 2026-09-29)
- Topper / Stacker: no source file by name. file_list.yml has Boss/Stacker/StackerCapWorldCtrl.o (exeBattle, exeDemoCamera, exeObjAppear) — needs decompiling or reimplementing
- Bowser / Koopa: src/Boss/Koopa/ has Koopa.h and helpers, no Koopa.cpp
- Most game logic (~81% of .o entries) has no source in this snapshot

## Rendering
- Original game rendering is Switch/NintendoSDK/agl/sead based. Those submodules are empty in the current extract and must be cloned.
- Quest 3 target needs: OpenXR, stereoscopic left/right views, 6DoF headset tracking, Touch controller tracking, proper projection matrices, 72 FPS minimum target, stable frame pacing.
- VR camera: third-person by default, Mario visible, independent head look, no forced rotation. Capture-dependent first-person: T-Rex and tank first-person, all other captures third-person (L, 2026-11-09).
- Comfort: adjustable distance/height, smooth + snap turning, recenter, optional vignette.

## Input (master prompt, to adapt to actual input architecture)
- Left controller: thumbstick move, X secondary, Y menu
- Right controller: thumbstick camera, A jump, B secondary, trigger Cappy, grip contextual
- Preserve Mario physics: movement, jumping, momentum, acceleration, air control, ground movement, collision, combat, environmental interaction.

## Build / host environment
- Dev host: Muse Secure VM, Ubuntu 24.04.5, x86_64, 4 vCPU, 7.7 GiB RAM (~0.7 GiB free at audit), 149 GB free disk in /home
- Cross-compile ARM64 Android via Android NDK (to install under ~/workspace/android-sdk)
- Decomp build needs: ninja, cmake 3.13+, ccache, llvm-objdump, Rust toolchain, Python modules (capstone, colorama, cxxfilt, pyelftools, watchdog, python-Levenshtein, toml)
- RAM is the main constraint: large C++ builds may need low -j / ccache, and may OOM if run in parallel with other work.

## File flow
NSP (~5 GB, Quest 3) → extract ExeFS → main NSO (1.0.0) → tools/setup.py → build/check
RomFS assets are needed for a playable game (models, stages, audio), not just the NSO.
prod.keys (workspace/user/files/prod.keys) is used only to extract L's own dump. Do not redistribute keys, NSO, NSP, or assets.
