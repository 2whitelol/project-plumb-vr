# PROJECT PLUMB VR — BLOCKERS.md

Last updated: 2026-11-09

## 1. Demo scope — RESOLVED 2026-11-09
- Confirmed: BOTH parts together — Part 1 opening Bowser encounter → fall → first meeting with Cappy, and Part 2 wedding interruption → Cap Kingdom → ending after defeating Topper.
- [Correction note, added when staging this repo: this was originally listed as a blocking conflict awaiting L's confirmation.]

## 2. No main NSO yet — NEEDS L / TRANSFER
- OdysseyDecomp setup requires the original 1.0.0 main NSO. No NSO/NSP has been received.
- Chat upload limit is 20 MB, Yahoo email is 25 MB, NSP is ~5 GB.
- Google Drive was being tried, but master prompt says do not rely on Google Drive / Google auth / phone verification / desktop apps.
- Simplest Quest-only option found: SwissTransfer — up to 50 GB per transfer, no account, link lasts 1–30 days (15 days default, extend via email on day 12). First send from a new device needs an email check. VM can download it with curl/wget and stream-process it.
- Even better if possible: send only the extracted main NSO / ExeFS instead of the full 5 GB NSP — setup only needs the NSO, though RomFS helps for assets/reference.
- Do NOT upload the NSP/NSO/keys to public repos or redistribute.

## 3. Android toolchain not installed — INSTALLABLE
- No Android SDK, NDK, JDK, ninja, clang, Rust, adb, gradle on the VM.
- Fix: install under ~/workspace/ (home persists; /usr and /opt do not survive VM replacement).
- This is large (SDK + NDK several GB) but disk has 149 GB free.

## 4. RAM is tight — RISK
- 7.7 GiB total, ~0.7 GiB available at audit. Large C++ / Android builds may OOM.
- Mitigation: low parallel jobs, ccache, build in stages, don't run other heavy work during builds.

## 5. Decomp is ~19% complete — FUNDAMENTAL
- 597/3,126 .o entries have source in the 2026-09-29 snapshot. Topper/Stacker and Koopa/Bowser boss code are missing or partial.
- A successful decomp build does NOT produce a playable game. Missing systems must be decompiled or reimplemented from scratch for the demo slice.
- This is the long pole of the project, not the Android/OpenXR shell.

## 6. No Quest 3 test device access from VM
- VM cannot install/run on L's Quest 3 directly. APKs must be handed back as downloadable files for L to sideload/install from the Quest.
- Testing results will depend on L running the APK on the Quest and reporting back.
