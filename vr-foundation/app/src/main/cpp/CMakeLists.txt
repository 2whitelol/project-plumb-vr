cmake_minimum_required(VERSION 3.22)
project(plumbvr_foundation)

add_library(openxr_loader SHARED IMPORTED)
set_target_properties(openxr_loader PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/arm64-v8a/libopenxr_loader.so)

add_library(plumbvr_foundation SHARED
    main.cpp
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

target_include_directories(plumbvr_foundation PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ANDROID_NDK}/sources/android/native_app_glue)

target_link_libraries(plumbvr_foundation
    openxr_loader
    android
    log
    EGL
    GLESv3)
