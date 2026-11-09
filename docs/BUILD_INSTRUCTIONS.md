# PROJECT PLUMB VR — BUILD_INSTRUCTIONS.md

Last updated: 2026-11-09
Status: PRELIMINARY — toolchain not installed yet, no NSO yet. Do not run a full build until Milestones 1–2 are complete.

## Dev host (Muse Secure VM)
- Ubuntu 24.04.5 LTS, x86_64
- Home (~) persists. Install everything durable under ~/workspace/ — NOT /usr, /opt, or /tmp (512 MB tmpfs, ephemeral).

## 1. Install decomp dependencies
```bash
sudo apt install python3 ninja-build cmake ccache libssl-dev llvm
# Rust toolchain: https://www.rust-lang.org/tools/install
pip install capstone colorama cxxfilt pyelftools watchdog python-Levenshtein toml
```

## 2. Clone OdysseyDecomp properly (the zip on disk is NOT a git repo and has empty submodules)
```bash
cd ~/workspace/project-plumb-vr
git clone https://github.com/MonsterDruide1/OdysseyDecomp
cd OdysseyDecomp
git submodule update --init --recursive
```

## 3. Get the main NSO
- You need the original Super Mario Odyssey 1.0.0 `main` NSO, dumped from your own Switch.
- Dumping the ExeFS of 1.0 is sufficient for setup; RomFS files help for reference/assets.
- See BLOCKERS.md for transfer: full NSP is ~5 GB, chat/email cannot carry it. SwissTransfer (up to 50 GB, no account) from the Quest browser is the current simplest Quest-only option.
- Do not redistribute the NSO, NSP, RomFS, or prod.keys.

## 4. Setup + build the decomp (verification only — this does NOT make a playable game)
```bash
tools/setup.py [path to the NSO]
tools/build.py
tools/check
```
- setup.py downloads clang/ld.lld etc. from OdysseyDecompToolsCache and creates build/
- setup.py verifies the v1.0.0 NSO hash.

## 5. Android / OpenXR (to be filled in once SDK/NDK are installed)
- Install Android SDK + NDK + JDK under ~/workspace/android-sdk/
- Target: ARM64, Android, OpenXR, Quest 3, 72 FPS minimum
- No WebXR. Native APK only.
- Install instructions from a standalone Quest (sideloading) to be added once the first test APK exists.

## Known limitations
- Decomp is ~19% complete (2026-09-29 snapshot). A decomp build is not a playable game.
- VM RAM is tight (~0.7 GiB free at audit) — use low -j, ccache, staged builds.
