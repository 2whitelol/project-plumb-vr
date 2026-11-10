# Demo progress

How far the real demo is. Not the whole game, just the demo slice.

## The number

**984 / 2,535 demo functions matched (38.8%)** as of 2026-11-10 16:34 CST.

That is the count from the local OdysseyDecomp clone's `data/file_list.yml`, for the 104 demo objects named in `docs/demo-gap-report.md`. Matching means byte-for-byte identical to the original game code.

The demo is both parts together. Part 1, the opening Bowser fight, the fall, meeting Cappy. Part 2, the wedding, Cap Kingdom, ending after Topper.

## Breakdown

| Area | Matched | Total | Percent |
|---|---|---|---|
| Topper (Boss/Stacker) | 119 | 179 | 66.5% |
| Cappy (Player/HackCap) | 141 | 189 | 74.6% |
| NPCs (Frog, TalkNpc, Cap NPCs) | 222 | 252 | 88.1% |
| Mario movement and player code | 220 | 664 | 33.1% |
| Bowser (Boss/Koopa) | 202 | 1,147 | 17.6% |
| Demo flow, camera, opening scene | 80 | 104 | 76.9% |
| **Total** | **984** | **2,535** | **38.8%** |

Bowser is the biggest blocker. His attack states are almost all still at zero. Mario movement is next. Topper and Cappy moved a lot today, they were near zero this morning.

## How the badge works

The badge at the top of the README is a live shields.io endpoint badge. It reads `demo-progress.json` from this repo, so it shows the current count whenever anyone loads the page — no manual badge edit needed. It turns orange at 50% and green at 80%.

The number itself is refreshed by `scripts/update_demo_progress.py`, which counts Matching functions across the 104 demo objects in `docs/demo-objects.txt` from the local OdysseyDecomp clone's `data/file_list.yml`, then updates `demo-progress.json` and this file together. It runs on a schedule (about every 30 minutes) while decomp work is landing. It only writes when the count moved.

It links here, so anyone can see what the number counts.

Badge now:

`Demo decomp — 39% (984/2,535 functions)`

## Next milestone: 50%

Set 2026-11-09. The demo is ready when **1,268 / 2,535 functions match (50%)**. That is 284 more than the 984 counted now.

How it adds up:

- Finish Topper (60 left), Cappy (48 left), and the NPCs (30 left): 138
- Mario core movement, Jump, WallCatch, PoleClimb, Rolling, Wait, SpinCap: about 180 of the 235 left there
- Bowser attack states, KoopaCap, KoopaCapPlayer, ThrowCap, RingBeam: about 90 of the 955 left there

Matching is decomp progress, not a playable demo. 50% matched does not mean half the demo is playable, the VR wiring is separate work.

## What this number is not

It is not the whole game. The whole decomp is 20,217 / 74,171 functions matched (27.3%) at the same time, and that is a different count.

It is not a playable demo either. Matching code is the game logic decompiled, not a game running on a Quest. The test APKs in VR are separate work, and they are not the demo yet.

## Older number

The gap report from 2026-11-09 18:05 CST said 770 / 2,748 (28.0%) across 127 objects. This file counts the 104 objects the report names individually. The other 23 objects in that survey were peripheral ones not tabulated one by one, 213 functions between them. Same demo, narrower named set, so the two totals are not directly comparable. The next update should use one list and stick to it.
