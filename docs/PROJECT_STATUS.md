# Project status

Last updated: 2026-11-09
Owner: L, Quest 3, no PC. Developer: Aidan, Muse Secure VM.

## Where I'm at

Milestone 1, the environment audit, is done, 2026-11-09.
Milestone 2, looking at the repo and the decomp, is in progress.

## The machine, checked 2026-11-09

- Ubuntu 24.04.5 LTS, x86_64, AMD EPYC 9D25, 4 vCPU visible (the runtime shares 1.5), 7.7 GiB RAM total, about 0.7 GiB free at audit time, 1 GiB swap.
- Disk: /home is 150 GB, 149 GB free. /tmp is a 512 MB tmpfs, so don't build there.
- Home sticks around between sessions. Stuff outside ~, like /usr, /opt, /etc, doesn't. Install the Android SDK, NDK, and tools under ~/workspace/.
- Shell is bash, root, terminal access works.
- Network works. GitHub and Google download servers are reachable.
- Already there: git 2.43.0, python3 3.12.3, pip3 24.0, cmake 3.28.3, make 4.3, gcc and g++ 13.3.0, node 24.22.1.
- Toolchain prep, done 2026-11-09: Android SDK with NDK r27c, ninja, clang 18, JDK 17, Rust, and a Python venv with the decomp modules (capstone, colorama, cxxfilt, pyelftools, watchdog, python-Levenshtein, toml).
- Still missing: OpenXR SDK and Gradle.
- Big repos clone fine, network's good for that.
- Can't compile native ARM64 Android apps yet in the old audit sense, that was before the SDK and NDK went in. That part's done now. Can't make a downloadable APK yet either, that needs the Android and OpenXR host code, which doesn't exist yet.

## The repo, checked 2026-11-09

- Source on disk: ~/workspace/project-plumb-vr/OdysseyDecomp-master/. That's a zip extract, not a git repo.
- Snapshot: 2026-09-29, 2,244 files.
- src and lib C++ and headers: 2,036 files.
- file_list.yml: 3,126 .o entries, 597 with matching source, about 19% decompiled.
- The submodules were empty in the zip extract: lib/sead, lib/NintendoSDK, lib/NintendoSDK-NEX, lib/agl, lib/eui, toolchain/musl, tools/common, all 0 bytes. A fresh git clone plus `git submodule update --init --recursive` fixed that.
- The decomp README says it straight: "this will not produce a playable game". Building the decomp is not a playable game.
- Setup needs the original 1.0.0 `main` NSO. `tools/setup.py [path to NSO]`, then `tools/build.py`, then `tools/check`.
- setup.py checks the NSO hashes for v1.0.0 (compressed e21692d9, uncompressed 18ece865, first bytes as in the original docs).

## Demo scope, confirmed 2026-11-09, both parts together

- Part 1, the opening: Bowser on his airship, Mario gets knocked off, falls, first meeting with Cappy in Cap Kingdom.
- Part 2, the wedding to Cap Kingdom: crash Bowser's wedding, Cap Kingdom, ends after beating Topper.
- Camera rule, from me, 2026-11-09: first person depends on the capture. T-Rex first person, tank first person, everything else third person.
- Controls, from me, 2026-11-09, changed from the master prompt: left stick moves, X is secondary, Y is menu. Right stick is camera, A is jump, B is secondary, grip is contextual when not holding Cappy. Cappy is not a button press to throw him. You move your hand, grab him with the grip button, and throw him by letting go of the grip in a throw motion.

## Files I have

- Decomp zip: workspace/user/files/OdysseyDecomp-master.zip
- prod.keys: workspace/user/files/prod.keys, uploaded 2026-11-09. The contents aren't written down anywhere.
- NSP: done. Super Mario Odyssey [0100000000010000][v0].nsp finished downloading 2026-11-09 around 11:08. Verified at 5,611,009,601 bytes exactly, PFS0 header, 8 files inside. It's at workspace/browser_downloads/sess-3704057155/browser-download-20261109T170111.866001991Z-0-Super_Mario_Odyssey__0100000000010000__v0_.nsp. Not in this repo, never will be.
- NSO extraction has started, pulling the main NSO out of the ExeFS with my prod.keys.
- Logos: workspace/user/media_library/image/Project_Plumb_VR_Globe_Logo.png (black background) and the __1.png one (white background), 1672x941.

## Next

1. Demo scope is confirmed, both parts together, see above.
2. Get the main NSO out. Extraction has started, it's the smallest piece the setup actually needs. The full NSP is already on disk if RomFS is needed later.
3. Fresh OdysseyDecomp clone and submodules are done. Run tools/setup.py with the 1.0.0 NSO, then tools/build.py and tools/check.
4. Android SDK, NDK, JDK, ninja, clang, and Rust are in under ~/workspace/. OpenXR SDK and Gradle are still missing.
5. Write the Android and OpenXR host code. That's the actual VR part, and it doesn't exist yet.
