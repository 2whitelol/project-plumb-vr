# Demo progress

How far the real demo is. Not the whole game, just the demo slice.

## The number

**2,295 / 2,535 demo functions matched (90.5%)** as of 2026-11-15 19:34 CST.

That is the count from the local OdysseyDecomp clone's `data/file_list.yml`, for the 104 demo objects named in `docs/demo-gap-report.md`. Matching means byte-for-byte identical to the original game code.

The demo is both parts together. Part 1, the opening Bowser fight, the fall, meeting Cappy. Part 2, the wedding, Cap Kingdom, ending after Topper.

## Breakdown

| Area | Matched | Total | Percent |
|---|---|---|---|
| Topper (Boss/Stacker) | 165 | 179 | 92.2% |
| Cappy (Player/HackCap) | 152 | 189 | 80.4% |
| NPCs (Frog, TalkNpc, Cap NPCs) | 234 | 252 | 92.9% |
| Mario movement and player code | 575 | 664 | 86.6% |
| Bowser (Boss/Koopa) | 1069 | 1,147 | 93.2% |
| Demo flow, camera, opening scene | 100 | 104 | 96.2% |
| **Total** | **2295** | **2,535** | **90.5%** |

Bowser is the biggest blocker. His attack states are almost all still at zero. Mario movement is next. Topper and Cappy moved a lot today, they were near zero this morning.

## How the badge works

The badge at the top of the README is a live shields.io endpoint badge. It reads `demo-progress.json` from this repo, so it shows the current count whenever anyone loads the page — no manual badge edit needed. It turns orange at 50% and green at 80%.

The number itself is refreshed by `scripts/update_demo_progress.py`, which counts Matching functions across the 104 demo objects in `docs/demo-objects.txt` from the local OdysseyDecomp clone's `data/file_list.yml`, then updates `demo-progress.json` and this file together. It runs on a schedule (about every 30 minutes) while decomp work is landing. It only writes when the count moved.

It links here, so anyone can see what the number counts.

Badge now:

`Demo decomp — 91% (2,295/2,535 functions)`

## Next milestone: 100%

Set 2026-11-09. The demo is ready when **2,535 / 2,535 functions match (100%)**. That is 240 more than the 2,295 counted now.

50% (1,268) and 80% (2,028) are both HIT. The next milestone is **100%: 2,535 / 2,535 functions** (266 more from now). Set by White 2026-11-15.

How it adds up:

- Finish Topper (14 left), Cappy (37 left), and the NPCs (18 left): 69
- Mario core movement, Jump, WallCatch, PoleClimb, Rolling, Wait, SpinCap: 92 left
- Bowser attack states, KoopaCap, KoopaCapPlayer, ThrowCap, RingBeam: 96 left
- Demo flow, camera, opening scene: 5 left

Matching is decomp progress, not a playable demo. 50% matched does not mean half the demo is playable, the VR wiring is separate work.

## What this number is not

It is not the whole game. The whole decomp is 22,657 / 74,171 functions matched (30.5%) at the same time, and that is a different count.

It is not a playable demo either. Matching code is the game logic decompiled, not a game running on a Quest. The test APKs in VR are separate work, and they are not the demo yet.

## Older number

The gap report from 2026-11-09 18:05 CST said 770 / 2,748 (28.0%) across 127 objects. This file counts the 104 objects the report names individually. The other 23 objects in that survey were peripheral ones not tabulated one by one, 213 functions between them. Same demo, narrower named set, so the two totals are not directly comparable. The next update should use one list and stick to it.
