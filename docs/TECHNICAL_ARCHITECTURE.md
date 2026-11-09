# How this is supposed to work

Last updated: 2026-11-09

## The goal

A native Android ARM64 OpenXR game on a Meta Quest 3 and Quest 3S, built from Super Mario Odyssey 1.0.0 decomp work. Both headsets run the same chip, Snapdragon XR2 Gen 2, so the same APK should run on both. No WebXR, no browser game, no flat screen spectator app.

## What I'm starting from

- OdysseyDecomp (MonsterDruide1). A work in progress decomp of SMO 1.0.0. About 19% of the .o entries have source, 597 out of 3,126, snapshot 2026-09-29.
- The decomp itself targets the Nintendo Switch, NX64 toolchain, musl, clang and lld from OdysseyDecompToolsCache. It does not target Android.
- So the decomp is a reference and partial code, not a game engine I can drop onto Android. The port needs a new Android and OpenXR host that reuses the decompiled logic where it exists and writes the missing systems from scratch.

## What's actually in the decomp

- src/Demo/DemoPeachWedding.cpp and .h, the wedding demo actor.
- src/Scene/FirstSequenceScene.cpp, 130 lines.
- src/Sequence/HakoniwaSequence.cpp. drawMain() uses docked and handheld render buffers.
- src/Player/PlayerJudgeCameraSubjective.cpp. The subjective, first person camera hook: al::initSubjectiveCameraNoSave, isTriggerCameraSubjective(), checkNoCollisionForPlayerSubjectiveCamera.
- lib/al/Library/Camera/. CameraPoser, CameraPoseUpdater, CameraTicket, and the rest.
- Player and Cap files: PlayerCapFunction, HackCap, and so on.

## What's missing, snapshot 2026-09-29

- Topper: no source file under that name. file_list.yml has a Boss/Stacker/StackerCapWorldCtrl.o entry (exeBattle, exeDemoCamera, exeObjAppear). Whether that's Topper's code isn't confirmed, it still needs checking, or reimplementing.
- Bowser, Koopa: src/Boss/Koopa/ has Koopa.h and helpers, no Koopa.cpp.
- Most of the game logic, about 81% of the .o entries, has no source in this snapshot.

## Rendering

- The original game renders on Switch, NintendoSDK, agl, sead. Those submodules were empty in the zip extract and had to be cloned.
- The Quest 3 and 3S side needs OpenXR, stereo left and right views, 6DoF headset tracking, Touch controller tracking, proper projection matrices, 72 FPS minimum on both, and stable frame pacing.
- VR camera: third person by default, Mario visible, head look on its own, no forced rotation. First person depends on the capture: T-Rex and the tank are first person, everything else is third person. That's from me, 2026-11-09.
- Comfort: adjustable distance and height, smooth plus snap turning, recenter, optional vignette.

## Input, from the master prompt, to fit the real input code later

- Left controller: thumbstick moves, X is secondary, Y is menu.
- Right controller: thumbstick is camera, A is jump, B is secondary, grip is contextual when not holding Cappy.
- Cappy is not a button press to throw him. You move your hand, grab him with the grip button, and throw him by letting go of the grip in a throw motion. Reach out with your hand, your hand is Mario's glove, hold grip to grab Cappy, throw your arm, release grip, and Cappy flies where you threw him.
- Keep Mario's physics intact: movement, jumping, momentum, acceleration, air control, ground movement, collision, combat, and how he interacts with the world.

## Build and host

- Dev host: Muse Secure VM, Ubuntu 24.04.5, x86_64, 4 vCPU, 7.7 GiB RAM (about 0.7 GiB free at audit), 149 GB free disk in /home.
- Cross compile ARM64 Android with the Android NDK, installed under ~/workspace/android-sdk.
- The decomp build needs ninja, cmake 3.13+, ccache, llvm-objdump, a Rust toolchain, and Python modules (capstone, colorama, cxxfilt, pyelftools, watchdog, python-Levenshtein, toml). Those are in now, in a Python venv, as of 2026-11-09.
- RAM is the tight part. Big C++ builds might need low -j and ccache, and they can run out of memory if other heavy work is running at the same time.

## File flow

NSP, about 5 GB, from my Quest 3, then extract the ExeFS, then the main NSO (1.0.0), then tools/setup.py, then build and check.

RomFS assets are needed for a playable game (models, stages, audio), not just the NSO.

prod.keys (workspace/user/files/prod.keys) is only for extracting my own dump. Don't share the keys, the NSO, the NSP, or the assets.
