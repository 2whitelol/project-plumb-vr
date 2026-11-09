# Dev log

## 2026-11-08

- I ported SM64 Coop DX to VR on my Quest 3 and got it running. Screenshot from Peach's Castle.
- Then I dropped the SM64 VR port and went all in on SMO VR instead.
- Research came back on Power Moon Recomp (a native PC port, needs your own SMO 1.0.0 dump) and Power Moon VR by evertec82 (an OpenXR and SteamVR overlay, experimental, v0.2.1 listing was stale, repo had moved to v0.2.4 and v0.2.5, still judder and 60 fps cap problems, no tracked hands).
- I picked Xbox Series X|S for Dragon Ball: Sparking! ZERO Legendary Edition. $159.99 CAD, checked. Didn't buy it.

## 2026-11-09

- I said I had some decompiled code and I'd dumped my own ROM.
- Uploaded OdysseyDecomp-master.zip (3.3 MB, 2,244 files, 2026-09-29) and the Project Plumb VR globe logos (1672x941, black and white versions).
- Demo scope at first: wedding fight, beat Topper in Cap Kingdom.
- Path picked: option 2. Native, from scratch slice, decomp as reference, only rebuild what the demo needs.
- ROM transfer was a pain. Chat upload caps at 20 MB, Yahoo email caps at 25 MB. I was putting the NSP on Google Drive at one point.
- prod.keys uploaded, saved at workspace/user/files/prod.keys. The contents aren't written down anywhere.
- Camera idea for captures: T-Rex first person, tank first person, everything else third person.
- Master prompt came in: native Quest 3 and Quest 3S APK, OpenXR, no WebXR, no PC needed, demo scope as opening Bowser fight, fall, first meeting with Cappy. Scope got confirmed later that day: both parts together. Opening to Cappy, plus wedding to Cap Kingdom to Topper.
- Milestone 1, the environment audit, done. See PROJECT_STATUS.md.
  - Ubuntu 24.04.5, x86_64, 4 vCPU, 7.7 GiB RAM (about 0.7 GiB free), 149 GB disk free.
  - There: git, python3 3.12, cmake 3.28, gcc and g++ 13.3, make, node.
  - Missing at audit time: ninja, clang, Java JDK, Rust, Android SDK and NDK, OpenXR SDK, adb, gradle.
  - Submodules in the decomp extract were empty. Fresh git clone and submodule init needed.
  - Network checked: GitHub and Google downloads reachable.
- NSP finished downloading around 11:08. Super Mario Odyssey 1.0.0, 5,611,009,601 bytes exactly, PFS0 header, 8 files inside. It came over SwissTransfer. NSO extraction started after that, pulling the main NSO out of the ExeFS with my prod.keys.
- Toolchain prep finished the same day. Android SDK with NDK r27c, ninja, clang 18, JDK 17, Rust, Python venv with the decomp modules. Still missing: OpenXR SDK and Gradle.
- Repo went live. yoohoocoocoollawliet/project-plumb-vr, created by me 2026-11-09. Guides and docs pushed first, commit 68e6f32, then the logo, commit c3e4e66, "Add Project Plumb VR logo". MIT license, copyright holder Project Plumb VR.
- Discord server started too. Same day, same project, in parallel with the repo.
- Target hardware corrected: not Quest 3 only. Quest 3 and Quest 3S. Same chip, Snapdragon XR2 Gen 2, so the same APK should run on both.
