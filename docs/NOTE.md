# A note on the docs in this folder

These five files are copies of the Project Plumb VR working docs as of **2026-11-09, 14:33** (`PROJECT_STATUS.md`, `TECHNICAL_ARCHITECTURE.md`, `BUILD_INSTRUCTIONS.md`, `DEVELOPMENT_LOG.md`, `BLOCKERS.md`).

They're in here so the repo is honest about where the project stands. But they're snapshots, not current on publish day. They've been rewritten in plain language since, and a few things in them have moved on.

## The demo scope is confirmed

The snapshots first described the demo scope as a conflict to confirm, opening to Cappy versus wedding to Topper. That was **fixed on 2026-11-09: the demo is both parts together**. Part 1 is the opening Bowser fight, the fall, first meeting with Cappy. Part 2 is the wedding crash, Cap Kingdom, ending after beating Topper. The scope sections in `PROJECT_STATUS.md` and `BLOCKERS.md` say that now.

## Target hardware, corrected

The snapshots say Quest 3. That's out of date. The target is **Quest 3 and Quest 3S**. Both headsets run the same chip, Snapdragon XR2 Gen 2, so the same APK should run on both. The README and the docs say that now.

## Other stuff that's moved on

- They say no NSO has been received and the Android toolchain isn't installed. Both have moved. The NSP finished downloading 2026-11-09 around 11:08, 5,611,009,601 bytes, and NSO extraction has started. The toolchain is mostly in too: Android SDK with NDK r27c, ninja, clang 18, JDK 17, Rust, Python venv. Still missing: OpenXR SDK and Gradle.
- Check the README's "Where things stand" section. That's the one to trust for status.

No secrets in these files. The only mention of `prod.keys` is a file path, with no key contents. No Nintendo assets, ROMs, or dumps are in here.
