# Contributing to Project Plumb VR

Thanks for wanting to help. This is a fan project, run by one person with a Quest 3, so here's how to not waste each other's time.

## The one hard rule

**Never send in Nintendo game files, keys, or dumps.** No `.nsp`, no `.nso`, no `.xci`, no RomFS or ExeFS files, no game models, audio, or textures, and no `prod.keys` or any other keys. A pull request with any of that in it gets closed without review. Check `.gitignore` first. If your file matches it, don't commit it.

## What actually helps right now

It's very early. No playable build, no VR code yet. The most useful stuff, in rough order:

1. **VR host, OpenXR, Android.** The native Quest 3 and Quest 3S APK shell, stereo rendering, 6DoF tracking, Touch controller input.
2. **Camera.** Third person VR camera, first person for T-Rex and the tank, comfort stuff like turning, recenter, and the vignette.
3. **Game logic.** Rebuilding the demo slice's missing systems from the decomp, or decompiling them (see below).
4. **Build and tooling.** Android SDK and NDK setup, CI, builds that work the same way twice.
5. **Testing.** Quest 3 and Quest 3S sideload testing, performance reports, bug reports with steps to make it happen again. There's #bug-reports and #performance-reports in the Discord for that.
6. **Docs.** Keeping `docs/` honest as things move. If something in there is out of date, fixing it counts.

## Decomp work goes upstream

If you decompile game functions or match code, send that to **OdysseyDecomp** (<https://github.com/MonsterDruide1/OdysseyDecomp>), not here. This repo uses OdysseyDecomp as a dependency. It doesn't fork it or copy it in.

## Ground rules

- Your own code and docs only. Don't paste code you don't have the right to share.
- Keep the demo in mind. Opening to Cappy, plus the wedding to Cap Kingdom to Topper. Don't build big systems the demo doesn't need yet.
- Small, focused PRs. Say what you changed and what you tested.
- Be kind. It's a fan project.
- Not sure if something is okay to publish? Ask first. If in doubt, leave it out.

## How to start

1. Read `README.md` and `docs/PROJECT_STATUS.md`.
2. Read `docs/BUILD_INSTRUCTIONS.md`. The toolchain is mostly in now, the build steps are still early, and the Android and OpenXR parts are still being written.
3. You need your own legally dumped SMO 1.0.0 copy and your own `prod.keys` to build anything that touches the decomp. This project will never give you those.
4. Open an issue before a big PR, so two people don't do the same work.
