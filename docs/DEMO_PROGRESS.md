# Demo progress

How far the real demo is. Not the whole game, just the demo slice.

## The number

**802 / 2,535 demo functions matched (31.6%)** as of 2026-11-09 21:15 CST.

That is the count from the local OdysseyDecomp clone's `data/file_list.yml`, for the 104 demo objects named in `docs/demo-gap-report.md`. Matching means byte-for-byte identical to the original game code.

The demo is both parts together. Part 1, the opening Bowser fight, the fall, meeting Cappy. Part 2, the wedding, Cap Kingdom, ending after Topper.

## Breakdown

| Area | Matched | Total | Percent |
|---|---|---|---|
| Topper (Boss/Stacker) | 119 | 179 | 66.5% |
| Cappy (Player/HackCap) | 121 | 189 | 64.0% |
| NPCs (Frog, TalkNpc, Cap NPCs) | 182 | 252 | 72.2% |
| Mario movement and player code | 108 | 664 | 16.3% |
| Bowser (Boss/Koopa) | 192 | 1,147 | 16.7% |
| Demo flow, camera, opening scene | 80 | 104 | 76.9% |
| **Total** | **802** | **2,535** | **31.6%** |

Bowser is the biggest blocker. His attack states are almost all still at zero. Mario movement is next. Topper and Cappy moved a lot today, they were near zero this morning.

## How the badge works

The badge at the top of the README is a static shields.io badge. It does not update itself. When the count moves, update the badge URL in `README.md` and the number in this file together, in the same commit.

Badge now:

`Demo decomp — 32% (802/2,535 functions)`

It links here, so anyone can see what the number counts.

## What this number is not

It is not the whole game. The whole decomp is 19,974 / 74,171 functions matched (26.9%) at the same time, and that is a different count.

It is not a playable demo either. Matching code is the game logic decompiled, not a game running on a Quest. The test APKs in VR are separate work, and they are not the demo yet.

## Older number

The gap report from 2026-11-09 18:05 CST said 770 / 2,748 (28.0%) across 127 objects. This file counts the 104 objects the report names individually. The other 23 objects in that survey were peripheral ones not tabulated one by one, 213 functions between them. Same demo, narrower named set, so the two totals are not directly comparable. The next update should use one list and stick to it.
