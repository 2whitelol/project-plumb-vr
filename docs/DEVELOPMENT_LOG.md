# PROJECT PLUMB VR — DEVELOPMENT_LOG.md

## 2026-11-08
- L ported SM64 Coop DX to VR on Quest 3, got it running. Screenshot from Peach's Castle.
- L pivoted all-in to SMO VR, dropping SM64 VR port.
- Proactive research delivered: Power Moon Recomp (native PC port, needs own SMO 1.0.0 dump) + Power Moon VR by evertec82 (OpenXR/SteamVR overlay, experimental, v0.2.1 listing stale, repo moved to v0.2.4/v0.2.5, still judder / 60 fps cap issues, no tracked hands).
- L chose Xbox Series X|S for Dragon Ball: Sparking! ZERO Legendary Edition ($159.99 CAD verified). Purchase not made.

## 2026-11-09
- L: "I have some decompiled code and I dumped my own ROM"
- Uploaded OdysseyDecomp-master.zip (3.3 MB, 2,244 files, 2026-09-29) + Project Plumb VR globe logos (1672x941, black + white versions).
- Demo scope stated: wedding confrontation → defeat Topper in Cap Kingdom.
- Path chosen: option 2 — native / from-scratch slice, decomp as reference, reimplement only what demo needs.
- ROM transfer: chat upload 20 MB limit, Yahoo email 25 MB limit. L putting NSP on Google Drive.
- prod.keys uploaded, saved at workspace/user/files/prod.keys. Contents not copied into memory.
- Capture-camera design: T-Rex first-person, tank first-person, rest third-person.
- Master prompt received: native Quest 3 APK, OpenXR, no WebXR, no PC required, demo scope now stated as opening Bowser encounter → fall → first meeting with Cappy (Scope later confirmed 2026-11-09: BOTH parts together — opening → Cappy, plus wedding → Cap Kingdom → Topper.)
- Milestone 1 environment audit completed (see PROJECT_STATUS.md).
  - Ubuntu 24.04.5, x86_64, 4 vCPU, 7.7 GiB RAM (~0.7 GiB free), 149 GB disk free
  - Present: git, python3 3.12, cmake 3.28, gcc/g++ 13.3, make, node
  - Missing: ninja, clang, Java JDK, Rust, Android SDK/NDK, OpenXR SDK, adb/gradle
  - Submodules in decomp extract are empty; fresh git clone + submodule init needed
  - Network verified: GitHub + Google downloads reachable
