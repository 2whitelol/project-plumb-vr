# Project Plumb VR

[![Discord](https://img.shields.io/badge/Discord-Join%20Project%20Plumb%20VR-5865F2?logo=discord&logoColor=white)](https://discord.gg/XUtAtkdumP)
[![Demo progress](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2F2whitelol%2Fproject-plumb-vr%2Fmain%2Fdemo-progress.json&cacheSeconds=60)](docs/DEMO_PROGRESS.md)
[![Recompendium](https://recompendium.com/insignias/port/super-mario-odyssey-project-plumb-vr.svg)](https://recompendium.com/en/port/super-mario-odyssey-project-plumb-vr/)

> **Bringing the Odyssey into another dimension.**
>
> A native Super Mario Odyssey VR port for Meta Quest 3 and Quest 3S.

![Project Plumb VR logo](assets/project-plumb-vr-logo.png)

## What this is

I'm White. I started this because I wanted to actually be Mario in VR, not watch him on a flat screen.

Project Plumb VR is a fan port of **Super Mario Odyssey (version 1.0.0)** for the **Meta Quest 3 and Quest 3S**. It's a standalone **Android ARM64 OpenXR APK**. No PC. No WebXR. No browser game. No streaming a flat game into a headset.

The idea is simple. Look down, see Mario's overalls. Reach out, your hands are his white gloves. Throw Cappy with a real throw. The Quest 3 and 3S do the tracking themselves, inside out, no base stations. Both headsets run the same chip, Snapdragon XR2 Gen 2, so the same APK should run on both.

This is not Nintendo's Labo VR. That came in update 1.3.0 and it's only three small side missions. SMO 1.0.0 has no VR in it at all. So the stereo rendering, the head tracked camera, 6DoF, the Touch controllers, all of it has to be built from scratch. The hard part is the game logic. It runs at 60 steps a second, and VR needs to run faster than that. Splitting those two apart is the core problem here.

## Demo scope

The first playable slice is both parts together. Not one or the other, both.

**Part 1, the opening.** Bowser's airship, the fight, Mario gets knocked off, he falls, then he meets Cappy in Cap Kingdom.

**Part 2, the wedding.** You crash Bowser's wedding, then it's Cap Kingdom again, and the demo ends after you beat **Topper**.

Topper makes sense as the end point. He takes 2 hits, which is the only Broodal fight that needs 2. He doesn't give a Power Moon. Beating him opens the Spark pylon onward. And Cap Kingdom has 0 Power Moons you can grab on a first visit, so the payoff here is unlocking the next kingdom, not a moon count.

## The long term goal

Once the game is fully decompiled, I want the whole thing in VR. All of it, like in the Nintendo Switch Presentation 2017 trailer (<https://www.youtube.com/watch?v=5kcdRBHM7kM>). All 17 kingdoms, all 880 Power Moons, all 52 captures.

That's a long way off. The demo comes first.

## How it should play

**Camera.** Third person VR most of the time, Mario in view, and you can look around on your own. You can change the distance and height, use smooth turning or snap turning, recenter whenever, and turn on a comfort vignette if you want it. Nothing should force your view around. No sudden spins.

**Captures change the camera.** It depends what you're controlling. **T-Rex is first person. Sherm, the tank, is first person.** Everything else stays third person.

**Being Mario.** First person where it fits, hands are Mario's gloves, inside out body tracking.

**Performance.** 72 FPS minimum on a standalone Quest 3 and Quest 3S. Higher refresh rates later, if I can get them.

**Controls, planned:**

Cappy is not a button press to throw him. You move your hand, grab him with the grip button, and throw him by letting go of the grip in a throw motion. Reach out and your hand is Mario's glove, hold grip to grab Cappy off Mario's head, throw your arm, release grip, and Cappy flies where you threw him. In the real build, after a throw Cappy stays in the air, rises slightly, then returns to your head. The demo's quick boomerang return is just a stand-in for that.

| Controller | Input | Action |
|---|---|---|
| Left | Thumbstick | Move Mario |
| Left | X | Secondary action |
| Left | Y | Menu |
| Right | Thumbstick | Camera |
| Right | A | Jump |
| Right | B | Secondary action |
| Right | Grip (hold) | Grab Cappy |
| Right | Grip (release in throw motion) | Throw Cappy |
| Either | Grip | Contextual interaction when not holding Cappy |

## Where things stand

Be honest about this. It's early.

**There is no playable demo yet.** But there are test APKs running on a Quest 3 now, and they have come a long way since v0.12. The latest Quest-tested one, **v0.37** (versionCode 50, 7,185,365 bytes, SHA-256 `8b7f6f01f2622e3601a48fc94bfd6ace5c46f7d2cfe94385897cfe5dcaf786fd`, delivered 2026-11-12, Quest-tested the same day, 83 frames all read), has Mario in first person with his real glove models on the controllers, a connected body and arms, real Mario textures, and Cappy thrown with the real decompiled HackCap throw logic: grip to grab, real release velocity to throw, lock-on to a capturable target, and return to the head. The v0.33.10 Quest test confirmed the big one: the turning blackout is FIXED — zero black in all 30 frames, even turning 165 degrees. It also confirmed the held-cap fix, the ghost Cappy look, the yellow cube, and fast returns (~1 s, 5 throws, no swinging). The v0.33.11 Quest test landed the first capture on Quest — the cube turned green and stayed green — but the arms were still straight pink tubes in all 23 frames, the body swung 1:1 with the head, and the HUD was gone entirely (a v0.33.11 regression). The v0.33.12 Quest test (33 frames, all read) confirmed the body fix held — no crash, zero black in 28/28 frames while turning 138 degrees, HUD back in 28/28, capture worked end to end twice, 72 Hz — but the arms were still straight tubes in 17/17 frames even though the ARM line proved the elbows bend in the math (d 35–81, elbow 44–128, never clamped), so the bend never reaches the screen, and the arms are too long. The v0.33.13 Quest test (55 frames, all read) confirmed the body decoupling on device and the ARM glyph fix, capture three times, no crash, no blackout — but the arms were still straight tubes in 50/50 frames: the .13 fix made it worse, shortening the arms to 62 units put them at full extension at White's normal hand distance (d 50–75, max 86, past the 61.999 clamp), so they cannot bend. Feet still float, and the new head-yaw line is clipped off the bottom of the screen. The v0.34 Quest test (51 frames, all read) confirmed the textures on device — red sleeves, blue overalls, brown shoes, white gloves with creases, capture twice, no crash, no blackout — but every closed fist shows a red slit through the middle, feet still float, ghost Cappy is still flat black and white with no texture, and the held cap sits at a fixed 0.90 m from the head instead of in the grabbing hand. The v0.35 Quest test (44 frames, all read) confirmed the held-cap fix — Cappy sits in the grabbing hand now (CD 0.34–0.71, not pinned at 0.90), the YH line is readable at last, body decoupling holds, and capture works end to end — but the arms still go straight when raised (look-down bends now, raised/look-up d climbs to 89 and the elbows lock at 179; the bow fix helped look-down only), the arms are still too thin vs the regular model, the fist gap is still there (closed empty fists show a red slit from the palm side), the tan untextured neck still fills the view when looking down, and the feet are still unconfirmed (never looked down at them in that test). v0.36 is delivered 2026-11-12 (versionCode 49, 7,185,365 B, SHA-256 `f421844d…`, host test 31/31 ASan-clean, all six check scripts pass) with a different arm root cause than length: the body now tracks White's torso (12° deadband, slow slew, so quick head glances don't move it) and the shoulders sit 4.2 units further forward — the arms are NOT lengthened. Arm radii are now the true ROM numbers (8.9/8.5), the fist gap gets a whole-palm plug, the neck piece is narrowed, and ghost Cappy is textured with his real atlas. Quest-tested 2026-11-12 (75 frames, all read): NO CRASH, no blackout on turns, capture worked three times, ghost Cappy textured, shoulder distances fixed (max 58, the 89 is gone) — but the body is still not glued to the camera (torso yaw swings 0–37, torso sits twisted under White when he looks down, hips swing out), the fist gaps are still there, the tan blob still fills the view looking down, the held cap still fills the face, and one elbow still locks straight at 179 in some raised poses. Feet never appear in any frame and stick-up direction could not be told from the video, so both stay unconfirmed. A blue wedge sits on the horizon in a few frames around 39–41 seconds. v0.37 delivered the same day (versionCode 50, 7,185,365 B, SHA-256 `8b7f6f01…`, host test 31/31 ASan-clean), Quest-tested the same day (83 frames, all read): NO CRASH, no blackout, capture worked three times end to end with a 4th throw in progress at the end of the video, ghost Cappy textured, body glue better with no gross torso twist looking down, and the green fist hole is gone. Still open: torso yaw still swings to 33 in sustained head-turned poses, shoulder distances regressed (max 70, was 58), one elbow still locks at 179 in extended-reach and throw poses (4 frames), a dark slit still shows in closed fists, the tan head-form still fills the view looking down (the 42% shrink did not fix it in practice), the held cap still fills the face when held up (fixed when held low), the blue wedge was not seen but the test never got far enough out to prove it gone (max about 14 m, the wedge needed about 24 m), the black rectangle in the sky is still there, and feet never appear in any frame. Stick-Y direction still unconfirmed. No Cappy eye animations in it. Next up is the hand-based torso-yaw fix, the headline for v0.38. Still no video, no music in the app.

**Demo decomp is at 61.1%: 1,548 / 2,535 functions matched** as of 2026-11-12. The 50% milestone is HIT, 280 past it. Round 43 finished PoleClimb at 32/41 (+14 matches) and the round 31 list is closed out — its last 7 Swim/attack functions are stuck on compiler codegen, not on the source. Framework round 25 added 37 more (full hideModel chain + sensor checks). Round 44 came back empty: the last PoleClimb, WallCatch and SpinCap items are a compiler-version ceiling under clang 3.9.1, not source problems — round 45 is digging into which clang Nintendo actually used. Bowser is still the big blocker. Full breakdown in [`docs/DEMO_PROGRESS.md`](docs/DEMO_PROGRESS.md). The badge at the top of this page shows the same number, and it updates as decomp lands. The whole game decomp is at 21,329 / 74,171 (28.8%) at the same time.

As of 2026-11-11:

- Toolchain done. OpenXR SDK 1.1.65, Gradle 8.14.3, Temurin JDK 17.0.21, Android SDK with NDK r27c, durable ninja 1.13.1 and ccache 4.10.2.
- The NSP is done. The Super Mario Odyssey 1.0.0 NSP finished downloading 2026-11-09 around 11:08, verified at 5,611,009,601 bytes exactly, PFS0 header, 8 files inside. It's not in this repo and it never will be.
- NSO extracted and the decomp setup done. `tools/build.py` and `tools/check` pass on the local clone.
- RomFS extracted, 4,636 files. Mario's model, gloves and body converted from the ROM. The real SMO menu extracted. Cap Kingdom ground partly converted. All 51 audio banks unpacked, 7,231 audio files catalogued, and a full-game reference written from the full walkthrough.
- Test APKs v0.6 through v0.36 built and Quest-tested in stages (v0.35 delivered 2026-11-11, Quest-tested the same day — held cap in hand and YH readable confirmed, arms still straight when raised / too thin, fist gap still open, feet unconfirmed; v0.36 delivered 2026-11-12, Quest-tested the same day (75 frames) — no crash/blackout, capture x3, ghost Cappy textured, shoulder distances fixed; body still not glued, fist gaps still open, tan blob and held cap in face still there, one elbow locks; feet + stick-Y unconfirmed. v0.37 delivered the same day (versionCode 50, SHA-256 8b7f6f01…, host 31/31), Quest-tested the same day (83 frames) — no crash/blackout, capture x3, ghost Cappy textured, body glue better, green fist hole gone; torso yaw still swings to 33, shoulder distances regressed to 70, one elbow still locks at 179 in reach poses, dark slit in closed fists, tan blob and held cap in face when raised still there, black rectangle still there, blue wedge untested, feet + stick-Y unconfirmed). The foundation app in this repo (`vr-foundation/`) is an early snapshot, v0.1, not the current test build. The current test builds are delivered to testers directly, not published here.
- The 50% demo milestone is HIT (1,548 / 2,535). Next milestone is 80% of the demo: 2,028 / 2,535 functions, 480 to go.
- Listed on Recompendium, approved and live 2026-11-11: [Project Plumb VR](https://recompendium.com/en/port/super-mario-odyssey-project-plumb-vr/). The badge at the top of this page updates with the listing status.

The decomp is the long pole. The 2026-09-29 snapshot has source for about 19% of the game's object files by file count, 597 of 3,126 `.o` entries. The published numbers don't agree with each other, so I'm not picking one and calling it the truth. The project's own page said 25.611% matching on 2026-11-09. Other trackers said 15.77% on 2026-10-26 and 16% on 2026-11-06. Different dates, different ways of counting. And a decomp build that works still isn't a playable game. The missing systems have to be decompiled or written from scratch for the demo slice.

## OdysseyDecomp

This project is not OdysseyDecomp, and it doesn't copy OdysseyDecomp's code in. OdysseyDecomp (<https://github.com/MonsterDruide1/OdysseyDecomp>) is its own thing, its own team, a work in progress decomp of SMO 1.0.0. I use it as a reference and a dependency, through a git submodule or a fresh clone, and I build my own VR host, renderer, input, and camera code on top of the decompiled logic where it exists.

One thing to know. At the time of writing, OdysseyDecomp has **no LICENSE file**. That means its code is all rights reserved by default, even though it's public on GitHub. So don't copy its files into this repo. If that changes upstream, this note needs updating.

Other projects in the same space, none of them mine: **Power Moon Recomp** (static recomp of 1.0.0 for PC) and **smo-recomp** (a separate 1.4.1 recomp project), plus **Power Moon VR** (an OpenXR overlay for Power Moon Recomp). Plumb is a native Quest port. Not an overlay, not a recomp.

## What you need to build it (not included)

You have to bring your own legally dumped copy. **None of this is in the repo, and none of it ever will be:**

- Super Mario Odyssey **1.0.0**, dumped from your own Switch. The main NSO from its ExeFS is what the decomp setup needs. RomFS assets are needed for a playable game.
- Your `prod.keys`, only for extracting your own dump.
- Any `.nsp`, `.nso`, `.xci`, RomFS files, game models, audio, or textures.

Check [`.gitignore`](.gitignore). All of that is excluded. Anything with that stuff in it gets rejected.

## Building (early days)

Full instructions are in [`docs/BUILD_INSTRUCTIONS.md`](docs/BUILD_INSTRUCTIONS.md). Short version, once the toolchain is in:

```bash
# 1. Clone with submodules (do NOT use a bare zip extract)
git clone --recursive https://github.com/MonsterDruide1/OdysseyDecomp
# (in the Plumb repo, OdysseyDecomp is a submodule, see docs)

# 2. Extract your own 1.0.0 main NSO from your own dump, then:
tools/setup.py [path to your main NSO]
tools/build.py
tools/check
```

That only checks the decomp. It doesn't make a playable game. The Android and OpenXR build steps still need writing. Put durable tools under `~/workspace/`, not `/usr`, `/opt`, or `/tmp`. See [`docs/PROJECT_STATUS.md`](docs/PROJECT_STATUS.md) and [`docs/BLOCKERS.md`](docs/BLOCKERS.md) for the environment audit and what's still blocked.

APK signing is an end of demo step only for the real release key. The test APKs are debug-signed with a test key and delivered to testers directly, they are not published in this repo.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md). Short version: your own code and docs only, no Nintendo assets, no keys, no dumps. Decomp work goes upstream to OdysseyDecomp, not here. The help I actually need is VR and host code, rendering, input, camera, builds, and testing.

## License

The original code and docs in this repo are under the MIT License. See [LICENSE](LICENSE). **That license only covers Project Plumb VR's own work.** It doesn't cover Nintendo's game, assets, or code, and it doesn't cover OdysseyDecomp's code. MIT, picked 2026-11-09, copyright holder Project Plumb VR. There's more on why MIT in [LICENSE-CHOICE.md](LICENSE-CHOICE.md).

## Legal

Project Plumb VR is an unofficial fan project. It's not affiliated with Nintendo, not endorsed by them, not connected to them. Super Mario Odyssey, Mario, Cappy, and all the related characters and assets are Nintendo's trademarks and copyrights. This repo has no Nintendo code, assets, or game files in it. You need your own legally obtained copy of the game to build or play anything from this project.

## Thanks

- The OdysseyDecomp team, for the decomp this is built on
- The SMO modding and decomp community
- White, project founder, Quest 3, no PC, doing it anyway
