# Contributing to Project Plumb VR

Thanks for wanting to help bring the Odyssey into another dimension.

## The one hard rule

**Never contribute Nintendo game files, keys, or dumps.** That means no
`.nsp`, `.nso`, `.xci`, RomFS/ExeFS files, game models/audio/textures,
and no `prod.keys` or any other keys. Pull requests containing any of
these will be closed without review. Check `.gitignore` — if your file
matches it, don't commit it.

## What helps most right now

The project is very early (no playable build, no VR code yet). The most
useful contributions are, in order:

1. **VR host / OpenXR / Android** — the native Quest 3 APK shell,
   stereo rendering, 6DoF tracking, Touch controller input
2. **Camera** — third-person VR camera, capture-dependent first-person
   (T-Rex / tank), comfort options (turning, recenter, vignette)
3. **Game logic** — reimplementing the demo slice's missing systems from
   the decomp, or decompiling them (see below)
4. **Build / tooling** — Android SDK/NDK setup, CI, reproducible builds
5. **Testing** — Quest 3 sideload testing, performance reports, bug
   reports with repro steps (see the Discord's #bug-reports and
   #performance-reports channels)
6. **Docs** — keeping `docs/` honest as the project moves

## Decomp work belongs upstream

If you decompile game functions or match code, contribute that to
**OdysseyDecomp** (<https://github.com/MonsterDruide1/OdysseyDecomp>),
not here. This repo consumes OdysseyDecomp as a dependency; it does not
fork or vendor it.

## Ground rules

- Original code and docs only. Don't paste code you don't have the
  right to share.
- Keep the demo scope in mind: opening → Cappy, plus wedding → Cap
  Kingdom → Topper. Don't gold-plate systems the demo doesn't need yet.
- Small, focused PRs with a clear description of what you tested.
- Be kind — this is a fan project, run by one person on a Quest 3.
- If you're unsure whether something is publishable, ask first. The
  safe answer is "don't include it."

## How to get started

1. Read `README.md` and `docs/PROJECT_STATUS.md`.
2. Read `docs/BUILD_INSTRUCTIONS.md` — note the toolchain is still
   being installed; the build steps are preliminary.
3. You need your own legally dumped SMO 1.0.0 copy and your own
   `prod.keys` to build anything that touches the decomp. Those are
   never provided by this project.
4. Open an issue before a big PR, so work doesn't duplicate.
