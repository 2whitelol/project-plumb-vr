# Blockers

Last updated: 2026-11-09

## 1. Demo scope, fixed 2026-11-09

Done. Both parts together. Part 1 is the opening Bowser fight, the fall, first meeting with Cappy. Part 2 is the wedding crash, Cap Kingdom, ending after beating Topper.

## 2. The NSO, mostly done

The NSP is in. Super Mario Odyssey 1.0.0, 5,611,009,601 bytes, downloaded 2026-11-09 around 11:08. NSO extraction from its ExeFS has started, using my own prod.keys.

For the record, getting it here was the hard part. Chat upload caps at 20 MB, Yahoo email caps at 25 MB, the NSP is about 5 GB. Google Drive was tried, but the master prompt says don't rely on Google Drive, Google auth, phone checks, or desktop apps. What worked was SwissTransfer, up to 50 GB per transfer, no account, link lasts 1 to 30 days (15 days by default, you can extend it by email on day 12). First send from a new device needs an email check.

Even better if you can do it: send just the extracted main NSO or ExeFS instead of the full 5 GB NSP. Setup only needs the NSO, though RomFS helps for assets and reference.

Don't upload the NSP, NSO, or keys to public repos. Don't share them.

## 3. Android toolchain, mostly done

This one's moved a lot. At audit time there was no Android SDK, NDK, JDK, ninja, clang, Rust, adb, or gradle on the VM. As of 2026-11-09 the SDK is in with NDK r27c, plus ninja, clang 18, JDK 17, Rust, and a Python venv with the decomp modules, all under ~/workspace/. Home sticks around, /usr and /opt don't survive a VM replacement, so that's where they live.

Still missing: OpenXR SDK and Gradle. Those are next.

## 4. RAM is tight, still a risk

7.7 GiB total, about 0.7 GiB free at audit. Big C++ or Android builds can run out of memory.

What helps: low parallel jobs, ccache, build in stages, don't run other heavy work during a build.

## 5. The decomp is about 19% done, that's the real blocker

597 out of 3,126 .o entries have source in the 2026-09-29 snapshot. Topper and the Koopa and Bowser boss code are missing or partial.

A decomp build that works still isn't a playable game. The missing systems have to be decompiled or written from scratch for the demo slice.

This is the long pole of the project, not the Android or OpenXR shell.

## 6. No Quest test device access from the VM

The VM can't install or run anything on White's Quest 3 directly. APKs have to come back as downloadable files for White to sideload and install from the Quest 3 or 3S. Both headsets run the same chip, Snapdragon XR2 Gen 2, so the same APK should run on both, but testing still depends on White running it and reporting back.
