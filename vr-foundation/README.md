# Project Plumb VR — Milestone 3: Android VR Foundation (test APK)

Minimal native Quest 3 / Quest 3S Android ARM64 OpenXR test app. Proves the
foundation works — it does NOT contain any game code yet.

## What it does
- Launches on Quest via `android.app.NativeActivity`.
- Creates an OpenXR 1.1.65 instance + session (OpenGL ES 3.0 binding).
- Renders a stereo scene: a coloured cube on a dark background, one colour
  per eye (blue left, teal right — proves per-eye stereo), rotating slowly.
- Follows head tracking (view poses from `xrLocateViews` drive the camera).
- Exits cleanly on session EXITING / LOSS_PENDING / app destroy.

## What built
- **Test APK:** `build-manual/plumbvr-foundation-test.apk`
  - 3,547,793 bytes, SHA-256 `226fb7b12cee9781be032b08c9574d6c0b73c7a8b5660b1a0b2005323f3d8496`
  - Package `com.plumbvr.foundation`, versionCode 1, versionName 0.1
  - `aapt2 dump badging`: sdkVersion 29, targetSdkVersion 34, native-code
    `arm64-v8a`, launchable NativeActivity, `android.hardware.vr.headtracking`
  - Contains `lib/arm64-v8a/libopenxr_loader.so` (1,661,048 B, OpenXR 1.1.65),
    `libplumbvr_foundation.so` (81,280 B, ARM aarch64, NEEDED libopenxr_loader.so),
    `libc++_shared.so`
  - Signed with a locally generated debug key (`build-manual/debug.keystore`,
    alias `plumbvr-debug`) — test key only, NOT the final demo signing key.
  - `apksigner verify`: Verifies (APK Signature Scheme v3)
- **Native lib:** `build-manual/lib/arm64-v8a/libplumbvr_foundation.so`
- **Source:** `app/src/main/cpp/main.cpp` (+ OpenXR 1.1.65 headers in
  `app/src/main/cpp/openxr/`, loader .so in `app/src/main/jniLibs/arm64-v8a/`)

## Gradle build — BLOCKED on this VM (2026-11-09)
The project has a full Gradle setup (`settings.gradle`, `build.gradle`,
`app/build.gradle`, AGP 8.5.2, Gradle 8.14.3, `gradle.properties` with
`-Xmx384m`). It does **not** build on this VM: every Gradle invocation —
including `gradle help` in an empty directory — fails with
`Could not receive a message from the daemon` /
`IllegalArgumentException: Unexpected type tag 109 found` in the daemon
client, even with `--no-daemon` and a fresh `GRADLE_USER_HOME`. The daemon
starts and listens on 127.0.0.1, but the client cannot read its first
message. Loopback TCP itself works (tested with a Python socket). This is
a Gradle-daemon communication failure on this VM, not a project error.
Full log: `gradle-build.log`.

## Manual build (what actually produced the APK)
```sh
cd ~/workspace/project-plumb-vr/vr-foundation
NDK=$HOME/workspace/android-sdk/ndk/27.2.12479018
CC=$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android34-clang
CXX=$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android34-clang++
BT=$HOME/workspace/android-sdk/build-tools/34.0.0
# 1. compile
$CC  -O2 -fPIC -c $NDK/sources/android/native_app_glue/android_native_app_glue.c \
     -I$NDK/sources/android/native_app_glue -o build-manual/glue.o
$CXX -std=c++17 -O2 -fPIC -c app/src/main/cpp/main.cpp \
     -Iapp/src/main/cpp -I$NDK/sources/android/native_app_glue -o build-manual/main.o
$CXX -shared -o build-manual/lib/arm64-v8a/libplumbvr_foundation.so \
     build-manual/main.o build-manual/glue.o \
     -Lapp/src/main/jniLibs/arm64-v8a -lopenxr_loader -landroid -llog -lEGL -lGLESv3
# 2. package (copy libc++_shared.so + libopenxr_loader.so into lib/arm64-v8a first)
$BT/aapt2 link -o build-manual/app-unsigned.apk --manifest app/src/main/AndroidManifest.xml \
    -I $HOME/workspace/android-sdk/platforms/android-34/android.jar --auto-add-overlay \
    --version-code 1 --version-name 0.1
cd build-manual && zip -0 -q app-unsigned.apk lib/arm64-v8a/*.so
$BT/zipalign -f 4 app-unsigned.apk app-aligned.apk
# 3. sign (debug key only; apksigner needs java on PATH)
export JAVA_HOME=$HOME/workspace/jdk-17 PATH=$HOME/workspace/jdk-17/bin:$PATH
$BT/apksigner sign --ks debug.keystore --ks-pass pass:android --key-pass pass:android \
    --out plumbvr-foundation-test.apk app-aligned.apk
$BT/apksigner verify plumbvr-foundation-test.apk
$BT/aapt2 dump badging plumbvr-foundation-test.apk
```

## Install / test on Quest (not yet done — no Quest access from this VM)
```sh
adb install -r plumbvr-foundation-test.apk
adb logcat -s PlumbVR   # look for "OpenXR instance created", "OpenXR session created", "Session begun"
```
Expected on a real Quest 3/3S: app opens in VR, a rotating coloured cube
floats in front of you, it stays world-locked as you move your head.
Watch for: black screen (swapchain/format issue), crash on launch
(check `adb logcat` for the `PlumbVR` tag), or the app closing immediately
(session never reaches READY).

## Still missing / not verified
- **Never run on a Quest.** Everything above is static verification only:
  build exit 0, badging, APK contents, signature. No runtime test exists.
- Gradle build on this VM (see above) — retry on a machine with a working
  Gradle daemon, or keep the manual build.
- No input handling yet (controllers), no game code, no comfort options.
- The debug keystore in `build-manual/` is a test key — the final demo APK
  gets its own key signature at the end, per White's instruction.
