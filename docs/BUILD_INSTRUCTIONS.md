# How to build

Last updated: 2026-11-09
Status: early. The toolchain is mostly in now, the NSP is downloaded, NSO extraction has started, but there's no Android or OpenXR host code yet and no APK. Don't run a full build until the steps below are actually ready, and don't compile or sign anything yet. Signing is an end of demo step only.

## The dev machine (Muse Secure VM)

- Ubuntu 24.04.5 LTS, x86_64.
- Home (~) sticks around. Put everything durable under ~/workspace/. Not /usr, not /opt, not /tmp. /tmp is 512 MB tmpfs and it doesn't survive.

## 1. Decomp dependencies

```bash
sudo apt install python3 ninja-build cmake ccache libssl-dev llvm
# Rust toolchain: https://www.rust-lang.org/tools/install
pip install capstone colorama cxxfilt pyelftools watchdog python-Levenshtein toml
```

As of 2026-11-09 most of this is already done: Android SDK with NDK r27c, ninja, clang 18, JDK 17, Rust, and a Python venv with those modules. Still missing: OpenXR SDK and Gradle.

## 2. Clone OdysseyDecomp properly

The zip on disk is not a git repo, and its submodules were empty.

```bash
cd ~/workspace/project-plumb-vr
git clone https://github.com/MonsterDruide1/OdysseyDecomp
cd OdysseyDecomp
git submodule update --init --recursive
```

That clone is done now, submodules filled.

## 3. Get the main NSO

- You need the original Super Mario Odyssey 1.0.0 `main` NSO, dumped from your own Switch.
- Dumping the ExeFS of 1.0 is enough for setup. RomFS files help for reference and assets.
- The NSP is done here: Super Mario Odyssey 1.0.0, 5,611,009,601 bytes, downloaded 2026-11-09 around 11:08. NSO extraction from its ExeFS has started, using my own prod.keys.
- For reference, the transfer used SwissTransfer, up to 50 GB, no account, from the Quest browser. Chat upload caps at 20 MB and Yahoo caps at 25 MB, so neither of those could carry a 5 GB file.
- Don't share the NSO, NSP, RomFS, or prod.keys.

## 4. Setup and build the decomp

This only checks the decomp. It does **not** make a playable game.

```bash
tools/setup.py [path to the NSO]
tools/build.py
tools/check
```

- setup.py downloads clang, ld.lld, and the rest from OdysseyDecompToolsCache and makes build/
- setup.py checks the v1.0.0 NSO hash.

## 5. Android and OpenXR

Still to be written, once the SDK and NDK are in and the host code exists.

- Android SDK and NDK are under ~/workspace/android-sdk/
- Target: ARM64, Android, OpenXR, Quest 3 and Quest 3S, 72 FPS minimum on both. Both headsets use the same chip, Snapdragon XR2 Gen 2, so one APK should cover both.
- No WebXR. Native APK only.
- How to install from a standalone Quest 3 or 3S (sideloading) gets written once the first test APK exists.

## Limits, known

- The decomp is about 19% done (2026-09-29 snapshot). A decomp build is not a playable game.
- VM RAM is tight, about 0.7 GiB free at audit. Use low -j, ccache, and build in stages.
