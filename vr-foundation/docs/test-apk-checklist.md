# Test APK build checklist — Plumb VR Foundation (Milestone 3)

Check every box before handing an APK to White. This is the **test APK only** (debug build, not the demo, not signed for release). If any MUST item fails, do not hand it over — fix it first and rebuild.

Tool paths (all verified 2026-11-09):
- Gradle: `~/workspace/gradle/gradle-8.14.3/bin/gradle`
- JDK: `~/workspace/jdk-17` (`JAVA_HOME=~/workspace/jdk-17`, Temurin 17.0.21)
- Android SDK: `~/workspace/android-sdk` (build-tools 34.0.0, NDK r27c 27.2.12479018, platform android-34)
- `aapt2`, `apksigner`, `zipalign`: `~/workspace/android-sdk/build-tools/34.0.0/`
- `adb`: `~/workspace/android-sdk/platform-tools/adb`

## MUST — build

- [ ] **Gradle build exits 0.** Run from `vr-foundation/` with `JAVA_HOME=~/workspace/jdk-17`, `-j1` equivalent (gradle.properties already sets daemon off, parallel off, workers max 1, `-Xmx512m` — RAM is tight, do not raise these). Command: `./gradlew assembleDebug` or `gradle assembleDebug`. **Only one Gradle build at a time** — coordinate with the Milestone 3 agent; never run two builds in `vr-foundation/` at once.
- [ ] **APK exists and is non-trivial in size.** Expected location: `vr-foundation/app/build/outputs/apk/debug/app-debug.apk`. Record the exact path and size in bytes. An APK under ~1 MB is suspicious (the OpenXR loader alone is 1.6 MB).
- [ ] **`aapt2 dump badging` runs clean.** Command: `~/workspace/android-sdk/build-tools/34.0.0/aapt2 dump badging app-debug.apk`. Check the output shows:
  - `package: name='com.plumbvr.foundation'`
  - `sdkVersion:'29'` and `targetSdkVersion:'34'`
  - `uses-feature: name='android.hardware.vr.headtracking'`
  - `uses-feature: name='oculus.software.handtracking'` (after the manifest fix — see `manifest-review.md`)
  - `launchable-activity: name='android.app.NativeActivity'` (with the `com.oculus.intent.category.VR` category listed after the manifest fix)
  - If `android:exported` is missing from the manifest, the build itself fails first — see `manifest-review.md` finding 1.

## MUST — contents (inspect the APK, do not trust the build log alone)

- [ ] **`arm64-v8a` only.** `unzip -l app-debug.apk | grep lib/arm64-v8a/` — the APK must contain `lib/arm64-v8a/libplumbvr_foundation.so` and `lib/arm64-v8a/libopenxr_loader.so`. No `armeabi-v7a`, no `x86_64` libs. The loader `.so` in `app/src/main/jniLibs/arm64-v8a/` is verified ELF 64-bit ARM aarch64 (1,661,048 bytes).
- [ ] **OpenXR loader present.** `libopenxr_loader.so` (OpenXR SDK 1.1.65) must be inside the APK at `lib/arm64-v8a/`. If it's missing, the app will crash on launch with an OpenXR loader error.
- [ ] **Native lib present.** `libplumbvr_foundation.so` (built from `app/src/main/cpp/main.cpp` + `android_native_app_glue.c`) must be inside the APK at `lib/arm64-v8a/`.
- [ ] **No Nintendo assets inside.** `unzip -l app-debug.apk` must show **no** `.nsp`, `.nso`, `.nca`, `.tik`, `prod.keys`, RomFS/ExeFS files, or Nintendo models/audio/textures. The test APK is pure foundation code + OpenXR loader. If anything like that appears, stop — do not hand it over, do not publish it.
- [ ] **No OdysseyDecomp code inside.** The foundation APK must not bundle decomp source or the decomp build. It is a separate dependency, not part of this APK.

## MUST — manifest (verify against the built APK, not just the source file)

- [ ] Manifest fixes from `manifest-review.md` applied: `android:exported="true"` on the activity, `com.oculus.intent.category.VR` category + `vr_only` meta-data, hand-tracking permission + feature. Verify in the `aapt2 dump badging` output above, not by re-reading the source XML (AGP merges manifests).
- [ ] 72 Hz is **not** a manifest check — verify in code: `main.cpp` requests the refresh rate via OpenXR (`xrRequestDisplayRefreshRateFB` or equivalent) and logs the rate the runtime grants. Record what rate was requested.

## SHOULD — before handover

- [ ] `zipalign` check passes: `zipalign -c -v 4 app-debug.apk` (AGP normally aligns already — this is a sanity check, not a rebuild step).
- [ ] `apksigner verify` passes for the debug signature: `apksigner verify --verbose app-debug.apk`. This is a **debug** signature. Do **not** release-sign, do **not** create a release keystore — signing the real demo APK is an end-of-demo step only, per White.
- [ ] `adb install -r` dry-run impossible on this VM (no Quest, no emulator) — record that explicitly in the handover note instead of claiming an install test happened. The first real install test is White's, following `quest-install.md`.
- [ ] Handover note written: APK path + size + SHA-256, Gradle exit code, `aapt2 dump badging` summary, what the app is expected to show on first launch (per `quest-install.md`), and what is known-not-tested (install on real Quest hardware, head tracking on device, 72 Hz on device).

## MUST NOT

- [ ] Do not publish the APK anywhere public (no GitHub release, no Discord upload, no file host) — hand it to White directly only.
- [ ] Do not sign with a release key. Do not create a keystore for this build.
- [ ] Do not bundle Nintendo assets, ROM/NSP/NSO, prod.keys, or OdysseyDecomp code (see contents checks above).
- [ ] Do not claim the APK was tested on a Quest — it was not and cannot be on this VM. Say so plainly in the handover note.
