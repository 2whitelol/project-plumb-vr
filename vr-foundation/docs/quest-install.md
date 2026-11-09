# Installing the Plumb VR test APK on Quest 3 / Quest 3S

For White. This covers the **test APK only** (the Milestone 3 foundation build — a simple stereo scene that proves OpenXR works). It is not the game yet, and it is not signed for release. Do not share the APK outside the project.

## Before the first install (one-time setup)

1. **Turn on Developer Mode on the Quest.** In the Meta Quest phone app: Menu → Devices → select the headset → Developer Mode → on. This needs a Meta developer account — creating one is free at developer.oculus.com (sign in with the Meta account, create an "organization", no payment). Without Developer Mode, sideloading is blocked.
2. **Install SideQuest on a computer** (sidequestvr.com) **or** use `adb` from the Android SDK platform-tools. SideQuest is the easier route; `adb` is the fallback. Both need the Quest connected by USB-C cable (or SideQuest over Wi-Fi after the first USB pairing).
3. **Allow USB debugging on the headset.** Plug the Quest in, put it on, and accept the "Allow USB debugging?" prompt (check "always allow from this computer"). If the prompt doesn't appear, unplug, replug, and check the phone app's USB connection status.

## Installing the APK

### Option A — SideQuest (easiest)
1. Open SideQuest with the Quest connected (green dot = connected).
2. Drag the `.apk` file onto the SideQuest window, or click the "Install APK" button (top bar) and pick the file.
3. Wait for "Install successful". The app appears in the headset under **Library → Unknown Sources** (filter top-right) as "Plumb VR Foundation".

### Option B — adb
```bash
adb install -r app-debug.apk
```
Success looks like: `Performing Streamed Install` then `Success`. If adb says `device unauthorized`, re-do step 3 above. If it says `INSTALL_FAILED_UPDATE_INCOMPATIBLE`, uninstall the old version first: `adb uninstall com.plumbvr.foundation`.

## First launch — what to expect

1. Put the headset on. Open **Library → Unknown Sources → Plumb VR Foundation**. (If the VR category fix from `manifest-review.md` is in, it should also appear in the main VR library.)
2. **First launch may ask for permissions** — allow whatever it asks. The test build needs no permissions beyond what the manifest declares, but Horizon OS may still show a prompt.
3. **What you should see:** a simple stereo VR scene (coloured background / a cube or grid — whatever the Milestone 3 build renders) that moves correctly when you turn your head. That's the whole point of this test: OpenXR session starts, head tracking works, stereo rendering works.
4. **What is NOT in this build:** no Mario, no game, no Cappy, no controllers-as-gloves yet. If you see the scene and can look around, the foundation works. If it crashes back to the home screen immediately, note exactly when (splash? black screen? after a few seconds?) and tell me.

## If it doesn't install / doesn't launch

| Symptom | Likely cause / fix |
|---|---|
| SideQuest shows the headset as not connected | USB cable (use a data cable, not charge-only), USB debugging prompt not accepted, try another USB port |
| `INSTALL_FAILED_UPDATE_INCOMPATIBLE` | Old version installed with a different signature — `adb uninstall com.plumbvr.foundation`, then reinstall |
| App not in the library | Check **Unknown Sources** filter in Library; if it's there, the manifest VR category is missing (see `manifest-review.md` finding 2) — not fatal, just annoying |
| Black screen, then back to home | OpenXR session failed to start — check the logcat (see below) and report the first error lines |
| Head tracking frozen / scene doesn't move | Head tracking not initialised — report it; do not reinstall repeatedly, it won't help |

## Getting logs (only if something goes wrong)

```bash
adb logcat -d | grep -i -E "plumbvr|openxr|vr" > plumbvr-log.txt
```
Send that file to me. The first error lines matter most — the last 20 lines are usually just the crash aftermath.

## Sending screenshots back (photo station)

This is how I test the game visually — I can't run a Quest APK on my VM, so your pictures are my eyes.

1. **Take a screenshot on the Quest:** hold the **Oculus/Meta button** on the right controller and pull the **trigger** (a shutter sound + a small preview confirms it). Alternatively, use SideQuest's screenshot button while connected.
2. **Video:** hold the Oculus button and the **grip** button to start/stop a recording, or use SideQuest's record button.
3. **Get the files off the headset:** in SideQuest, use the file manager / "pull" the files from the Quest's Pictures/Screenshots folder, or plug the Quest into a computer and copy them over USB (MTP).
4. **Send them to me** in chat (or drop them in the Discord #screenshots-and-clips channel). I file them in `workspace/project-plumb-vr/photo-station/screenshots/` (raw clips in `captures/`) and look at them properly — framing, lighting, glitches, what's working — and report back.
5. Tell me what you were doing in each shot ("looking at the cube", "turned my head left", "crashed after 5 seconds") — a picture without context is half as useful.

## After the test
- **Do not uninstall it yet** unless I say so — later test APKs will update over it (`adb install -r` / SideQuest reinstall keeps it simple).
- The test APK is **debug-signed, not release-signed**. The real demo APK gets a proper key signature at the very end only, per White's instruction — do not ask for signing on a test build.
