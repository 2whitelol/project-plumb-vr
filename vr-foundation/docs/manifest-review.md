# AndroidManifest review — Plumb VR Foundation test APK

Reviewed 2026-11-09 by the Quest test-APK support agent. **This is a review only — the manifest itself was NOT changed.** The file under review is `app/src/main/AndroidManifest.xml` (1,139 bytes, written by the Milestone 3 agent 2026-11-09 21:42 UTC). The proposed additions at the bottom are for that agent to apply; do not hand-edit the manifest while a Gradle build is running.

## What is already correct

| Item | Manifest | Notes |
|---|---|---|
| Package / namespace | `com.plumbvr.foundation` | Set in `app/build.gradle` (`namespace` + `applicationId`). AGP 8.x does not need a `package` attribute on `<manifest>` — its absence is correct, not a gap. |
| Head tracking | `<uses-feature android:name="android.hardware.vr.headtracking" android:required="true" />` | Correct and required for a VR app. |
| VR mode | `<uses-feature android:name="android.software.vr.mode" android:required="false" />` | Fine as optional. |
| SDK levels | `minSdkVersion 29`, `targetSdkVersion 34` | Matches `app/build.gradle` (minSdk 29, targetSdk 34, compileSdk 34). Quest 3/3S run Android 12L+ (API 32), so minSdk 29 is fine; targetSdk 34 is what Meta currently requires for new uploads. |
| Activity | `android.app.NativeActivity`, `singleTask`, landscape, fullscreen, `configChanges="orientation\|keyboardHidden\|screenSize"` | Correct for a native OpenXR app. |
| Native lib | `<meta-data android:name="android.app.lib_name" android:value="plumbvr_foundation" />` | Matches the CMake target name in `app/src/main/cpp/CMakeLists.txt`. |
| Launcher | `MAIN` + `LAUNCHER` intent filter | Correct. |
| `hasCode="false"` | On `<application>` | Standard for a NativeActivity-only app (no Java/Kotlin code). |

## Findings — in priority order

### 1. BLOCKER: activity is missing `android:exported="true"` (must fix before any APK is handed to White)
The activity has an `<intent-filter>` (MAIN/LAUNCHER) and the app targets SDK 34 (Android 14). Since Android 12 (API 31), **every activity, service, or receiver with an intent filter must declare `android:exported` explicitly**, or the build fails (`android:exported needs to be explicitly specified`) / install fails. Add:

```xml
<activity android:name="android.app.NativeActivity"
    android:exported="true"
    ...>
```

### 2. Missing: VR intent category (app may not show up as a VR app)
Quest recognises a VR app by the `com.oculus.intent.category.VR` category. Without it, a sideloaded app can end up only in the flat 2D app list / Unknown Sources instead of the VR library. Add to the intent filter:

```xml
<category android:name="com.oculus.intent.category.VR" />
```

And add the matching meta-data inside `<application>` (per Meta/Qt docs):

```xml
<meta-data android:name="com.oculus.intent.category.VR" android:value="vr_only" />
```

### 3. Missing: hand tracking permission + feature (hands will not be enumerated)
Per Meta's own hand-tracking docs (developers.meta.com, "Enable Hand Tracking"), an app must declare both of these or it will not see hand devices at all:

```xml
<uses-permission android:name="com.oculus.permission.HAND_TRACKING" />
<uses-feature android:name="oculus.software.handtracking" android:required="false" />
```

Notes:
- `required="false"` is deliberate: the app must still run with Touch controllers if hand tracking is off or unavailable. The Cappy grip-throw uses controllers first; hands are a later input path.
- Spelling discrepancy found while checking: Meta's docs use `oculus.software.handtracking` (no hyphen); some Qt/third-party samples write `oculus.software.hand-tracking`. Use the Meta spelling above and verify it survives the build via `aapt2 dump badging` on the finished APK (see `test-apk-checklist.md`).
- Optional, for later: hand-tracking frequency/version meta-data (`com.oculus.handtracking.frequency` = `HIGH`, `com.oculus.handtracking.version` = `V2.0`). Not needed for the test APK.

### 4. Touch controllers: no manifest entry needed (do NOT invent one)
There is **no** `uses-feature` name for Touch controllers. Controllers come through OpenXR interaction profiles at runtime (Oculus Touch / Touch Plus profiles), not through the manifest. Do not add a made-up feature name — it would be ignored at best and could break store filtering. If a future check shows controllers not enumerating, the fix is in `main.cpp` (interaction profiles / action sets), not the manifest.

### 5. 72 Hz: not a manifest setting (do NOT add a made-up meta-data)
Display refresh rate is a **runtime OpenXR setting**, not a manifest entry. There is no standard Quest manifest key for it. The 72 Hz target is achieved by requesting it in code (e.g. `xrRequestDisplayRefreshRateFB`, where supported) and by keeping the frame loop at 72 fps. Checklist item for `main.cpp`: confirm the refresh-rate request exists and log the rate the runtime actually grants. Do not add a `com.oculus.displayRefreshRate` meta-data — that key is not part of Meta's documented manifest surface.

### 6. Optional, for later (not needed for the test APK)
- `com.oculus.supportedDevices` meta-data to restrict to Quest 3 / Quest 3S (values e.g. `quest3`, `quest3s`). Only matters for store filtering; sideloading ignores it. Leave out until the APK goes anywhere near a store listing.
- `android:installLocation` — leave at default.

## Proposed additions (apply to the real manifest, do not copy this file over it)

```xml
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android">
    <uses-feature android:name="android.hardware.vr.headtracking" android:required="true" />
    <uses-feature android:name="android.software.vr.mode" android:required="false" />
    <uses-feature android:name="oculus.software.handtracking" android:required="false" />
    <uses-permission android:name="com.oculus.permission.HAND_TRACKING" />
    <uses-sdk android:minSdkVersion="29" android:targetSdkVersion="34" />
    <application android:label="Plumb VR Foundation" android:hasCode="false">
        <meta-data android:name="com.oculus.intent.category.VR" android:value="vr_only" />
        <activity android:name="android.app.NativeActivity"
            android:label="Plumb VR Foundation"
            android:exported="true"
            android:theme="@android:style/Theme.NoTitleBar.Fullscreen"
            android:launchMode="singleTask"
            android:screenOrientation="landscape"
            android:configChanges="orientation|keyboardHidden|screenSize">
            <meta-data android:name="android.app.lib_name" android:value="plumbvr_foundation" />
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
                <category android:name="com.oculus.intent.category.VR" />
            </intent-filter>
        </activity>
    </application>
</manifest>
```

## Sources checked 2026-11-09
- Meta, "Enable Hand Tracking" (developers.meta.com/vr/documentation/native/android/mobile-hand-tracking/): `com.oculus.permission.HAND_TRACKING` permission + `oculus.software.handtracking` feature are required for hands to be enumerated.
- Meta Spatial SDK skill (meta-quest/agentic-tools): VR category `com.oculus.intent.category.VR` in the intent filter; `oculus.software.handtracking` optional feature.
- Qt Quest 3 quick start (doc.qt.io): `com.oculus.intent.category.VR` meta-data `vr_only`, hand-tracking permission + feature.
- Android 12 behaviour (API 31+): activities with intent filters must declare `android:exported` explicitly.
