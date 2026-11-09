# Project Plumb VR

> **Bringing the Odyssey into another dimension.**
>
> A native Super Mario Odyssey VR port for Meta Quest 3.

> **Status:** The guides and project documentation are published here
> now. The VR code is still in progress — there is no playable build yet.
> Repository: <https://github.com/yoohoocoocoollawliet/project-plumb-vr>
> — MIT License, © 2026 Project Plumb VR. See
> [PUBLISH-CHECKLIST.md](PUBLISH-CHECKLIST.md).

![Project Plumb VR logo](assets/project-plumb-vr-logo.png)

## What this is

Project Plumb VR is a fan project to port **Super Mario Odyssey (version
1.0.0)** to VR — natively, on the **Meta Quest 3**, as a standalone
**Android ARM64 OpenXR APK**. No PC required, no WebXR, no browser game,
no flat-screen spectator app, and no video-streaming overlay.

The goal is to *be* Mario in first person: look down and see his overalls,
reach out and your hands are his white gloves, throw Cappy with a real
throwing motion. Quest 3 inside-out tracking does it all — no base
stations.

This is **not** Nintendo's Labo VR mode (that arrived in update 1.3.0 and
is only three small side missions). SMO 1.0.0 has no VR code at all, so
everything VR — stereo rendering, head-tracked camera, 6DoF, Touch
controllers, and decoupling the game's 60-steps-per-second logic so it can
run at Quest refresh rates — has to be built from scratch.

## Demo scope

The first playable slice is **both parts together**:

- **Part 1 — Opening:** the Bowser encounter aboard his airship → Mario
  is knocked off → falls → first meeting with Cappy in Cap Kingdom.
- **Part 2 — Wedding → Cap Kingdom:** interrupting Bowser's wedding →
  Cap Kingdom → ends after defeating **Topper**.

Topper is the right end point for the demo: he takes 2 hits (the only
Broodal fight that needs 2), gives no Power Moon, and beating him opens
the Spark pylon onward. Cap Kingdom also has 0 Power Moons collectable on
a first visit, so the demo's payoff is unlocking the next kingdom.

## Long-term goal

Once the game is fully decompiled, port **all** of Super Mario Odyssey to
VR — the full game as shown in the Nintendo Switch Presentation 2017
trailer (<https://www.youtube.com/watch?v=5kcdRBHM7kM>): all 17 kingdoms,
all 880 Power Moons, all 52 captures.

## Design

- **Camera:** comfortable third-person VR by default, Mario visible,
  player can physically look around. Adjustable distance/height, smooth
  and snap turning, recenter, optional comfort vignette. No forced
  rotation, no excessive acceleration.
- **Capture camera rule:** first-person depends on the capture —
  **T-Rex first-person, Sherm (tank) first-person**, everything else
  third-person.
- **Being Mario:** first-person play where it fits, hands are Mario's
  gloves, inside-out body tracking.
- **Performance target:** 72 FPS minimum on standalone Quest 3, higher
  refresh rates as an optimisation target.
- **Controls (planned):**

  | Controller | Input | Action |
  |---|---|---|
  | Left | Thumbstick | Mario movement |
  | Left | X | Secondary action |
  | Left | Y | Menu |
  | Right | Thumbstick | Camera control |
  | Right | A | Jump |
  | Right | B | Secondary action |
  | Right | Trigger | Cappy-related action |
  | Either | Grip | Contextual interaction |

## Current status — be honest, this is early

**There is no playable build yet. There is no VR code yet.**

As of 2026-11-09:

- ✅ Environment audited (Ubuntu 24.04 VM, git/python/cmake/gcc present)
- ✅ Decomp snapshot on disk (OdysseyDecomp-master, 2026-09-29, 2,244 files)
- ✅ Fresh OdysseyDecomp clone started, with submodules to initialise
- ✅ Research done: opening/wedding/Cap Kingdom/Topper, all 17 kingdoms,
  52 captures, decomp and recomp landscape — see `docs/`
- 🔄 Android SDK / NDK / JDK / ninja / clang / Rust toolchain install
  in progress (not finished at the time of writing)
- ❌ No main NSO extracted yet — the 1.0.0 NSP dump transfer is in
  progress separately and is **not** in this repo
- ❌ No Android/OpenXR host code written yet
- ❌ No APK, no sideloadable build

The decompilation itself is the long pole: the 2026-09-29 snapshot has
source for ~19% of the game's object files by file count (597 of 3,126
`.o` entries). Published progress figures disagree by date and method —
the project's own page read 25.611% matching on 2026-11-09, other
trackers reported 15.77% on 2026-10-26 and 16% on 2026-11-06 — so no
single percentage is quoted here as the truth. A successful decomp build
does **not** produce a playable game; missing systems must be decompiled
or reimplemented for the demo slice.

## Relationship to OdysseyDecomp

Project Plumb VR is **not** OdysseyDecomp, and it does not vendor or
re-publish OdysseyDecomp's code. OdysseyDecomp
(<https://github.com/MonsterDruide1/OdysseyDecomp>) is a separate,
independent WIP decompilation of SMO 1.0.0 by its own team. Plumb uses
it as a **reference and dependency** (via a git submodule / fresh clone)
and builds its own original VR host, renderer, input, and camera code
on top of the decompiled logic where it exists.

Note: at the time of writing, the OdysseyDecomp repository contains **no
LICENSE file**, so its code is technically all-rights-reserved by
default despite being public on GitHub. Do not copy its files into this
repo. If that changes upstream, update this note.

Related, unaffiliated work: **Power Moon Recomp** (static recompilation
of 1.0.0 for PC) and **smo-recomp** (a separate 1.4.1 recomp project),
and **Power Moon VR** (an OpenXR overlay for Power Moon Recomp). Plumb
is a native Quest port, not an overlay and not a recomp.

## What you need to build it (not included)

You must supply your own, legally dumped copy. **None of the following
is in this repository, and none of it ever will be:**

- Super Mario Odyssey **1.0.0** — your own dump from your own Switch
  (the main NSO from its ExeFS is what the decomp setup needs; RomFS
  assets are needed for a playable game)
- Your `prod.keys` — needed only to extract your own dump
- Any `.nsp`, `.nso`, `.xci`, RomFS files, game models, audio, or
  textures

See [`.gitignore`](.gitignore) — all of these are excluded, and any
contribution containing them will be rejected.

## Building (preliminary)

Full instructions live in [`docs/BUILD_INSTRUCTIONS.md`](docs/BUILD_INSTRUCTIONS.md).
Short version, once the toolchain is installed:

```bash
# 1. Clone with submodules (do NOT use a bare zip extract)
git clone --recursive https://github.com/MonsterDruide1/OdysseyDecomp
# (in the Plumb repo, OdysseyDecomp is a submodule — see docs)

# 2. Extract your own 1.0.0 main NSO from your own dump, then:
tools/setup.py [path to your main NSO]
tools/build.py
tools/check
```

That verifies the decomp only — it does not make a playable game, and
the Android/OpenXR build steps are still to be written once the SDK/NDK
install lands. Install durable tools under `~/workspace/`, not `/usr`,
`/opt`, or `/tmp`. See [`docs/PROJECT_STATUS.md`](docs/PROJECT_STATUS.md)
and [`docs/BLOCKERS.md`](docs/BLOCKERS.md) for the environment audit and
open blockers.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md). In short: original code and
docs only, no Nintendo assets, no keys, no dumps. Decomp contributions
belong upstream in OdysseyDecomp, not here. VR/host, rendering, input,
camera, build, and testing help is where this project needs people.

## License

The original code and documentation in this repository are licensed
under the MIT License — see [LICENSE](LICENSE). **That license covers
only Project Plumb VR's original work.** It does not cover Nintendo's
game, assets, or code, and it does not cover OdysseyDecomp's code.
License: **MIT, confirmed 2026-11-09**, copyright holder Project Plumb
VR. See [LICENSE-CHOICE.md](LICENSE-CHOICE.md) for why MIT was chosen
over GPLv3 / Apache 2.0.

## Legal / disclaimer

Project Plumb VR is an unofficial fan project. It is not affiliated
with, endorsed by, or connected to Nintendo. Super Mario Odyssey, Mario,
Cappy, and all related characters and assets are trademarks / copyrights
of Nintendo. This repository contains no Nintendo code, assets, or game
files. You need your own legally obtained copy of the game to build or
play anything from this project.

## Acknowledgements

- The OdysseyDecomp team, for the decompilation this project builds on
- The SMO modding / decomp community
- L — project founder, Quest 3, no PC, doing it anyway
