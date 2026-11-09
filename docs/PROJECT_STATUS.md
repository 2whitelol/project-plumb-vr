# PROJECT PLUMB VR — PROJECT_STATUS.md

Last updated: 2026-11-09
Owner: White (formerly L, changed 2026-11-09; Quest 3 and Quest 3S, no PC) / Developer: Aidan (Muse Secure VM)

## Current milestone
Milestone 1 — ENVIRONMENT AUDIT: DONE (2026-11-09)
Milestone 2 — REPOSITORY ANALYSIS: DONE 2026-11-09 — NSO extracted, setup.py succeeded, build dir configured. tools/build.py + tools/check DONE 2026-11-09 (build exit 0, check "OK").
Milestone 3 — ANDROID VR FOUNDATION: TEST APK BUILT 2026-11-09 — native OpenXR test APK at workspace/project-plumb-vr/vr-foundation/build-manual/plumbvr-foundation-test.apk (3,547,793 B, SHA-256 226fb7b1…, package com.plumbvr.foundation, arm64-v8a, libopenxr_loader.so 1.1.65 inside, apksigner verify OK). Built manually (aapt2 + NDK clang + zipalign + apksigner) because the Gradle daemon is broken on this VM (see DEVELOPMENT_LOG.md). NOT yet run on a Quest — install/logcat test still to do on real hardware.
Toolchain prep (Milestone 3 groundwork): DONE 2026-11-09, with one correction (see below): the VM was replaced after prep, wiping apt-installed tools; durable ninja 1.13.1 + ccache 4.10.2 now live in ~/workspace/tools-bin. OpenXR SDK 1.1.65 + Gradle 8.14.3 + durable Temurin JDK 17.0.21 installed 2026-11-09 — Milestone 3 toolchain is now complete.

## Environment audit — verified 2026-11-09, toolchain installed 2026-11-09
- VM: Ubuntu 24.04.5 LTS, x86_64, AMD EPYC 9D25, 4 vCPU visible (runtime shared CPU 1.5), 7.7 GiB RAM total, ~0.5 GiB available, 1 GiB swap
- Disk: /home 150 GB, 146 GB free after prep (3.2 GB used). /tmp is 512 MB tmpfs — do NOT build there.
- Home directory persists between sessions. Files outside ~ (e.g. /usr, /opt, /etc) are ephemeral — Android SDK, Python venv, and Rust live under ~/workspace/ or ~/.cargo; apt installs (ninja/clang/JDK/ccache) are system-wide and will need reinstalling after a VM replacement.
- Shell: bash, root, terminal access: YES
- Network: YES — GitHub and Google download servers reachable (via egress proxy; curl works, Java sdkmanager does NOT — see below)
- Installed and version-verified 2026-11-09:
  - git 2.43.0, python3 3.12.3, cmake 3.28.3, make 4.3, gcc/g++ 13.3.0, node 24.22.1 (pre-existing)
  - ninja 1.11.1, clang/LLVM 18.1.3, OpenJDK 17.0.20.1 (java + javac), ccache 4.9.1, libssl-dev (apt)
  - Rust rustc/cargo 1.99.1 via rustup (~/.cargo, ~/.rustup, 614 MB)
  - Python modules in ~/workspace/project-plumb-vr/venv (42 MB): capstone 5.0.7, colorama, cxxfilt, pyelftools, watchdog, python-Levenshtein, toml — all import OK
  - Android SDK ~/workspace/android-sdk (2.4 GB): command-line tools 11076708, platform-tools 37.0.1 (adb 1.0.41), build-tools 34.0.0 (aapt2 2.19), platforms android-34 ext12, NDK r27c 27.2.12479018 (clang 18.0.3)
  - ARM64 cross-compile VERIFIED: NDK clang --target=aarch64-linux-android34 produced an ELF 64-bit ARM aarch64 PIE binary (/system/bin/linker64)
  - OpenXR SDK 1.1.65 (Khronos, release-1.1.65) at ~/workspace/openxr-sdk: headers generated + installed at install/include/openxr/openxr.h, host loader install/lib/libopenxr_loader.so.1.1.65 (ELF x86-64, built with gcc 13.3, -j1, Release), Android loader AAR openxr_loader_for_android-1.1.65.aar with jni/arm64-v8a/libopenxr_loader.so for Quest 3/3S
  - Gradle 8.14.3 at ~/workspace/gradle/gradle-8.14.3 (Kotlin 2.0.21, Groovy 3.0.24, JVM 17.0.21 Temurin); chosen over current Gradle 9.9.0 because AGP 8.x does not support Gradle 9
  - Eclipse Temurin JDK 17.0.21+9 (durable) at ~/workspace/jdk-17 — JAVA_HOME=~/workspace/jdk-17 (apt OpenJDK 17 was wiped by the VM replacement)
- STILL MISSING: nothing for Milestone 3 toolchain. clang/LLVM 18 (apt) is still gone — loader built with gcc 13.3 instead; cmake 3.28.3 present now but apt/ephemeral, re-check after any VM replacement.
- Known issue: sdkmanager cannot fetch the remote repo (Java proxy tunneling NoSuchElementException, even with --proxy flags). Workaround used: packages downloaded directly with curl from dl.google.com (URLs from repository2-1.xml) and unzipped into place. Side effects: platform-tools r37.0.2 zip 404s on Google's server, so platform-tools-latest (37.0.1) was used; NDK has no package.xml so sdkmanager --list_installed does not list it (the other three packages do list).
- CORRECTION 2026-11-09: the VM was replaced after the prep work, and all apt-installed tools from prep (ninja 1.11.1, clang/LLVM 18.1.3, OpenJDK 17, ccache 4.9.1, libssl-dev) were gone — only ~/workspace and ~/.cargo persisted. apt-get update hung >8 min and was killed. Replaced with durable static binaries in ~/workspace/tools-bin: ninja 1.13.1, ccache 4.10.2 (both verified). Android SDK, venv, Rust, and the decomp clone all survived (they live under ~/workspace / ~/.cargo). Anything installed via apt must be re-checked after a VM replacement.

## Repository — verified 2026-11-09
- Source on disk: ~/workspace/project-plumb-vr/OdysseyDecomp-master/ — zip extract, NOT a git repo (kept untouched)
- Fresh clone 2026-11-09: ~/workspace/project-plumb-vr/OdysseyDecomp, master @ 696db2ff "MapObj: Implement `MoonWorldBell` (#1210)", 189 MB, 451 .cpp files in src/
- Submodules FILLED IN 2026-11-09 (git submodule update --init --recursive, exit 0, 983 files): lib/sead efc9f22, lib/NintendoSDK 097321c, lib/NintendoSDK-NEX 21314b3, lib/agl 07eeda7, lib/eui b30f4a1, toolchain/musl 37fd56c, tools/common 3ee5333, plus nested tools/common/asm-differ 79a8bbe
- Snapshot (zip): 2026-09-29, 2,244 files
- file_list.yml: 3,126 .o entries, 597 with matching source (~19% decompiled by that file-count method)
- README: "this will not produce a playable game" — building the decomp is NOT a playable game
- Setup requires: original 1.0.0 `main` NSO. `tools/setup.py [path to NSO]`, then `tools/build.py`, then `tools/check`
- Setup.py verifies NSO hashes for v1.0.0 (compressed e21692d9…, uncompressed 18ece865…)

## NSO extraction + setup — DONE 2026-11-09 (verified)
- NSP on VM: workspace/browser_downloads/sess-3704057155/browser-download-20261109T170111.866001991Z-0-Super_Mario_Odyssey__0100000000010000__v0_.nsp, 5,611,009,601 bytes, PFS0, 8 files.
- main NSO: ~/workspace/project-plumb-vr/extracted/exefs/main, 15,388,240 bytes, NSO0 magic, sha256 e21692d90f8fd2def2d2d22d983d62ac81df3b8b3c762d1f2dca9d9ab7b3053a — exact match to setup.py COMPRESSED_V10_HASH. Extracted with nstool 1.9.2 (`-k <prod.keys> --tik <tik> -t nca --part0`) from program NCA 0f26bd42cae0e4cefda4b5bbf7ae3d50.nca; hactool 1.4.0 (built from source, patched to skip newer 34-hex-char master_kek_source entries) extracted the PFS0 but cannot read .tik files. prod.keys contents never read/copied/printed.
- setup.py: exit 0. data/main.elf (31 MB, sha256 b8f8b542c1ee6bd3eb70c9ccebb52b69c9f7ced5f7cd8aebed56ec8fe53b3aa5 — exact V10_ELF_HASH match), data/main.uncompressed.nso (31 MB, sha256 18ece865061704d551fe456e0600c604c26345ecb38dcbe328a24d5734b3b4eb — exact UNCOMPRESSED_V10_HASH match). Build dir configured: build/build.ninja present, Clang 3.9.1 (decomp toolchain), RelWithDebInfo, Ninja generator. tools/build.py NOT run yet.
- Disk after extraction + setup: 137 GB free. RAM ~0.68 GiB available — build with low -j + ccache.

## Demo scope — CONFIRMED 2026-11-09
- BOTH together — Part 1: opening Bowser encounter aboard his ship → fall → first meeting with Cappy. Part 2: wedding interruption → Cap Kingdom → end after defeating Topper.
- Target hardware: Quest 3 AND Quest 3S (same Snapdragon XR2 Gen 2, same APK) — 2026-11-09, from White.
- Capture-camera design (2026-11-09, from L): first-person depends on capture — T-Rex first-person, tank first-person, rest third-person
- Controls (master prompt, corrected 2026-11-09): left stick move, X secondary, Y menu; right stick camera, A jump, B secondary; Cappy is NOT a button throw — grab Cappy with grip, throw by releasing grip in a throw motion; grip contextual when not holding Cappy.
- APK signing: end-of-demo step only — do NOT compile/sign yet.

## Files held
- Decomp zip: workspace/user/files/OdysseyDecomp-master.zip
- Fresh clone: workspace/project-plumb-vr/OdysseyDecomp (with submodules)
- prod.keys: workspace/user/files/prod.keys (uploaded 2026-11-09, contents NOT read or copied)
- NSP: workspace/browser_downloads/sess-3704057155/browser-download-20261109T170111.866001991Z-0-Super_Mario_Odyssey__0100000000010000__v0_.nsp — verified 5,611,009,601 bytes, on this VM since ~11:08 2026-11-09. Extracted PFS0 at workspace/project-plumb-vr/extracted/pfs0/ (8 files); ExeFS at workspace/project-plumb-vr/extracted/exefs/ (main, main.npdm, rtld, sdk, subsdk0).
- Logos: workspace/user/media_library/image/Project_Plumb_VR_Globe_Logo.png (black bg) and …__1.png (white bg), 1672x941

## Decomp build + check — DONE 2026-11-09 (verified)
- tools/build.py: exit 0, 970/970 steps, 17:29:50–17:41:59 UTC (~12 min), -j1, ccache cold (968/968 misses), zero errors in the log. Output: build/odyssey, 53 MB, ELF 64-bit ARM aarch64 with debug_info (verification artifact only — not a playable game, not an APK).
- tools/check: exit 0, "OK" (control-tested: fails without data/main.elf, so it genuinely compared against the original). file_list_progress.py: 19,296 decompiled (26.014%), 18,996 matching (25.611% by count | 15.767% by size), 47 minor / 252 major non-matching.
- Milestone 2 DONE. Log: workspace/project-plumb-vr/build-20261109.log, check log: workspace/project-plumb-vr/check-20261109.log. Disk: 136 GB free.

## Next steps
1. Milestone 3 (Android VR foundation): toolchain complete — OpenXR SDK 1.1.65, Gradle 8.14.3, Temurin JDK 17.0.21, Android SDK/NDK r27c. Next is the initial test APK work.
2. Decomp help (approved by White 2026-11-09, in parallel with the VR port): candidate functions identified in the decomp-help audit in DEVELOPMENT_LOG.md — no PR submitted yet.
