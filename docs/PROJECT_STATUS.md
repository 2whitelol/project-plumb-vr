# PROJECT PLUMB VR — PROJECT_STATUS.md

Last updated: 2026-11-09
Owner: L (Quest 3, no PC) / Developer: Aidan (Muse Secure VM)

## Current milestone
Milestone 1 — ENVIRONMENT AUDIT: DONE (2026-11-09)
Milestone 2 — REPOSITORY ANALYSIS: IN PROGRESS

## Environment audit — verified 2026-11-09
- VM: Ubuntu 24.04.5 LTS, x86_64, AMD EPYC 9D25, 4 vCPU visible (runtime shared CPU 1.5), 7.7 GiB RAM total, ~0.7 GiB available at audit time, 1 GiB swap
- Disk: /home 150 GB, 149 GB free. /tmp is 512 MB tmpfs — do NOT build there.
- Home directory persists between sessions. Files outside ~ (e.g. /usr, /opt, /etc) are ephemeral — install Android SDK/NDK and tools under ~/workspace/
- Shell: bash, root, terminal access: YES
- Network: YES — GitHub and Google download servers reachable
- Present: git 2.43.0, python3 3.12.3, pip3 24.0, cmake 3.28.3, make 4.3, gcc/g++ 13.3.0, node 24.22.1
- MISSING (to install under ~/workspace): ninja, clang/llvm, Java JDK, Rust toolchain, Android SDK + NDK, CMake is present but Android needs SDK, OpenXR SDK, adb/gradle/sdkmanager
- Decomp Python modules missing: capstone, colorama, cxxfilt, pyelftools, watchdog, python-Levenshtein, toml
- Can clone large GitHub repos: YES (network verified)
- Can compile native ARM64 Android apps: NOT YET — no Android SDK/NDK installed. Possible after installing SDK/NDK + JDK under ~/workspace.
- Can create downloadable APK artifacts: NOT YET — depends on Android SDK/NDK install above.

## Repository — verified 2026-11-09
- Source on disk: ~/workspace/project-plumb-vr/OdysseyDecomp-master/ — zip extract, NOT a git repo
- Snapshot: 2026-09-29, 2,244 files
- src + lib C++/headers: 2,036 files
- file_list.yml: 3,126 .o entries, 597 with matching source (~19% decompiled)
- Submodules EMPTY: lib/sead, lib/NintendoSDK, lib/NintendoSDK-NEX, lib/agl, lib/eui, toolchain/musl, tools/common — all 0 bytes. Need a fresh git clone + `git submodule update --init --recursive`
- README: "this will not produce a playable game" — building the decomp is NOT a playable game
- Setup requires: original 1.0.0 `main` NSO. `tools/setup.py [path to NSO]`, then `tools/build.py`, then `tools/check`
- Setup.py verifies NSO hashes for v1.0.0 (compressed e21692d9…, uncompressed 18ece865…)

## Demo scope — CONFIRMED 2026-11-09 (BOTH parts together)
- Part 1 — Opening: Bowser encounter aboard his airship → Mario is knocked off → falls → first meeting with Cappy in Cap Kingdom.
- Part 2 — Wedding → Cap Kingdom: interrupting Bowser's wedding → Cap Kingdom → ends after defeating Topper.
- [Correction note, added when staging this repo: the snapshot above originally presented this as a conflict to confirm. L confirmed BOTH parts together on 2026-11-09.]
- Capture-camera design (2026-11-09, from L): first-person depends on capture — T-Rex first-person, tank first-person, rest third-person
- Controls (master prompt): left stick move, X secondary, Y menu; right stick camera, A jump, B secondary, trigger Cappy, grip contextual

## Files held
- Decomp zip: workspace/user/files/OdysseyDecomp-master.zip
- prod.keys: workspace/user/files/prod.keys (uploaded 2026-11-09, contents NOT copied into memory)
- NSP: NOT received yet. ~5 GB, on L's Quest 3. L was uploading to Google Drive, then master prompt says do not rely on Google Drive.
- Logos: workspace/user/media_library/image/Project_Plumb_VR_Globe_Logo.png (black bg) and …__1.png (white bg), 1672x941

## Next steps
1. Demo scope confirmed (BOTH parts together — see above)
2. Get main NSO — smallest required file, not the full 5 GB NSP if extraction is possible
3. Fresh git clone of OdysseyDecomp + submodules
4. Install Android SDK/NDK + JDK + ninja/clang/Rust under ~/workspace/
5. Run tools/setup.py with the 1.0.0 NSO, then tools/build.py / tools/check
