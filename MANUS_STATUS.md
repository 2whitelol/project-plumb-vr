# Project Plumb VR — Status for Manus

Updated: 2026-11-09 ~16:35 CST (America/Regina). Synced to GitHub by the Manus
bridge (`bridge/sync.sh`, every 6 hours). Read `MANUS_HANDOFF.md` first for
the full project brief; this file is the current numbers and the next steps.

## Decomp matching (verified by Muse, local OdysseyDecomp clone only)
- `tools/check`: **OK** (exit 0)
- Matching: **19,245 (25.947% by count | 15.88% by size)**
- Decompiled: 19,549 (26.357% by count)
- Clone HEAD: `696db2ff` ("MapObj: Implement `MoonWorldBell` (#1210)")
- Local clone only — **no PR submitted, nothing pushed to OdysseyDecomp**.
- Latest rounds: ScenarioStartCamera ctor + appear matched; Cap Kingdom
  NPCs (CapWorldNpcTailJointController, TalkNpcCap); HackCap +65 then +11
  (throw getters, `isThrow`, `exeSpinAttack`, `isForceCapTouchJump`).
  HackCap.o is now 76/189 Matching.
- Still unmatched — backend scheduling/codegen diffs, left Wip, **do not
  retry without a genuinely new compiler/scheduling idea**: `RailMove::update`,
  `ScenarioStartCamera::isEnableStart` / `init` / `exePlay`,
  `PlayerTrigger::isOnAnyDamage`, `CapWorldNpcTailJointController::calcJointCallback`,
  `setupThrowStart`, `updateSeparateMode`.
- Stacker == Topper is **still UNVERIFIED**. Do not assert it in code, PRs,
  or docs.

## Test APK (Milestone 3)
- **Built and verified on Muse's VM**: `plumbvr-foundation-test.apk`,
  3,547,793 bytes, SHA-256
  `226fb7b12cee9781be032b08c9574d6c0b73c7a8b5660b1a0b2005323f3d8496`,
  package `com.plumbvr.foundation`, arm64-v8a, OpenXR 1.1.65, stereo
  rotating cube (blue left eye / teal right eye) driven by head poses.
  Debug-signed only. Source is in `vr-foundation/` in this repo.
- **Never run on a Quest yet.** Needs a real Quest 3/3S: sideload with
  `adb install -r`, check `adb logcat -s PlumbVR`.
- Gradle daemon fails on Muse's VM for every invocation — the APK was built
  by hand with NDK clang + aapt2 + zipalign + apksigner. Gradle files are
  kept for a machine where the daemon works.

## What Manus should do next
1. Clone this repo, read `MANUS_HANDOFF.md`, then this file.
2. Continue decomp matching (demo-relevant first): HackCap big functions
   (`init`, `movement`, `control`, `startThrow`, `exeThrow*`), `forcePutOn`
   (64 B, decoded — calls a virtual on the object at `_5c0[6]`, type
   unidentified), Cap Kingdom Frog / TalkNpcCap small functions.
3. Then Milestone 4: game integration toward the playable demo (opening
   Bowser encounter → fall → first meeting with Cappy, plus wedding
   interruption → Cap Kingdom → end after defeating Topper).
4. Report verified facts only — what matched, what built, what was actually
   tested on hardware, and what is still blocked.
5. **Never** push Nintendo assets, ROM/NSP/NSO/NCA, prod.keys, or any key
   file to this repo. The bridge safety check blocks them on Muse's side;
   do not work around it.

## Bridge status
- Last push: commit `1ad76c78` ("Sync project state for Manus handoff"),
  21 files. Sync runs every 6 hours (cron `project-plumb-vr-bridge-sync`).
- Pull-back: Manus's repo changes are pulled back into Muse's workspace
  with `bridge/pull-back.sh` (fetch + pull in the repo staging area only —
  it never touches the OdysseyDecomp clone or local decomp work).
