# -----------------------------
# Minimum CMake version
# -----------------------------
cmake_minimum_required(VERSION 3.18)

# Project
project(mediainfo_android LANGUAGES C CXX)

# -----------------------------
# Compiler setup
# -----------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------
# Android-specific configuration
# -----------------------------
if(ANDROID)
    # Suppress warnings for cleaner builds
    add_compile_options(
            -Wno-cpp
            -Wno-deprecated-declarations
            -Wno-unused-parameter
            -Wno-unused-variable
    )

    # Optimizations for release builds
    set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

    # Output directories for AAR packaging
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../libs/${ANDROID_ABI})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../libs/${ANDROID_ABI})

    # Add your definitions
    add_definitions(
            -DZENLIB_NO_HTTP_CLIENT
            -DZENLIB_NO_THREAD_SAFE
            -DMEDIAINFO_ANDROID
            -DUNICODE
            -D_UNICODE
    )

    # Print build configuration
    message(STATUS "MediaInfo Build Configuration:")
    message(STATUS "  - References: YES")
    message(STATUS "  - Video: YES")
    message(STATUS "  - Audio: YES")
    message(STATUS "  - MKV Support: YES")
    message(STATUS "  - MP4 Support: YES")
    message(STATUS "  - AVI Support: YES")
endif()

# -----------------------------
# MediaInfo specific defines (common to all platforms)
# -----------------------------
# -----------------------------
# MediaInfo Feature Enables
# -----------------------------
add_definitions(
        # Enable basic media formats
        -DMEDIAINFO_REFERENCES_YES
        -DMEDIAINFO_VIDEO_YES
        -DMEDIAINFO_AUDIO_YES
        -DMEDIAINFO_TEXT_YES

        # Enable specific format support
        -DMEDIAINFO_MPEG4_YES
        -DMEDIAINFO_MK_YES           # Matroska (MKV) support
        -DMEDIAINFO_AVI_YES
        -DMEDIAINFO_MPEGPS_YES
        -DMEDIAINFO_MPEGTS_YES
        -DMEDIAINFO_OGG_YES
        -DMEDIAINFO_RIFF_YES
        -DMEDIAINFO_FLV_YES
        -DMEDIAINFO_GIF_YES
        -DMEDIAINFO_JPEG_YES
        -DMEDIAINFO_PNG_YES

        # Enable codec support
        -DMEDIAINFO_MPEGV_YES
        -DMEDIAINFO_AVC_YES
        -DMEDIAINFO_HEVC_YES
        -DMEDIAINFO_MPEGA_YES
        -DMEDIAINFO_AC3_YES
        -DMEDIAINFO_AAC_YES

        # Enable essential features
        -DMEDIAINFO_READER_YES
        -DMEDIAINFO_EVENTS_YES
        -DMEDIAINFO_FILTER_YES
        -DMEDIAINFO_DEMUX_YES
        -DMEDIAINFO_IBI_YES

        # Keep your existing disables
        -DMEDIAINFO_ADVANCED_NO
        -DMEDIAINFO_DUPLICATE_NO
        -DMEDIAINFO_MACROBLOCKS_NO
        -DMEDIAINFO_TRACE_NO
        -DMEDIAINFO_TRACE_FFV1CONTENT_NO
        -DMEDIAINFO_DIRECTORY_NO
        -DMEDIAINFO_LIBCURL_NO
        -DMEDIAINFO_LIBMMS_NO
        -DMEDIAINFO_DVDIF_ANALYZE_NO
        -DMEDIAINFO_MPEGTS_DUPLICATE_NO
        -DMEDIAINFO_READTHREAD_NO
        -DMEDIAINFO_MD5_NO
        -DMEDIAINFO_SHA1_NO
        -DMEDIAINFO_SHA2_NO
        -DMEDIAINFO_AES_NO
        -DMEDIAINFO_FIXITY_NO
        -DMEDIAINFO_NEXTPACKET_NO
        -DMEDIAINFO_GRAPHVIZ_NO
)

# -----------------------------
# Android compatibility header
# -----------------------------
file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/android_compat.h
        "#ifndef ANDROID_COMPAT_H
#define ANDROID_COMPAT_H

#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>

// Windows type definitions for Android
typedef int32_t INT;
typedef uint32_t UINT;
typedef int32_t LONG;
typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef uint8_t BYTE;
typedef char CHAR;
typedef unsigned char UCHAR;
typedef int BOOL;

// String function mappings
#define strnicmp strncasecmp
#define stricmp strcasecmp

// Socket compatibility
#define WSAGetLastError() errno
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define closesocket close

#endif // ANDROID_COMPAT_H
")

# Force include android_compat.h for all C files
add_compile_options($<$<COMPILE_LANGUAGE:C>:-include${CMAKE_CURRENT_SOURCE_DIR}/android_compat.h>)

# -----------------------------
# Include directories
# -----------------------------
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/ZenLib/Source
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaInfoLib/Source
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaInfoLib/Source/MediaInfo
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# -----------------------------
# ZenLib
# -----------------------------
file(GLOB_RECURSE ZENLIB_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/ZenLib/Source/ZenLib/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/ZenLib/Source/ZenLib/*.c"
)
# Remove HTTP client files
list(FILTER ZENLIB_SOURCES EXCLUDE REGEX ".*HTTP_Client.*")

add_library(zen STATIC ${ZENLIB_SOURCES})
set_target_properties(zen PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        C_VISIBILITY_PRESET hidden
        CXX_VISIBILITY_PRESET hidden
)
target_include_directories(zen PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# -----------------------------
# Third-party libs
# -----------------------------
set(TFSXML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/MediaInfoLib/Source/ThirdParty/tfsxml)
set(TINYXML2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/MediaInfoLib/Source/ThirdParty/tinyxml2)

file(GLOB_RECURSE TINYXML2_SOURCES "${TINYXML2_DIR}/*.cpp")

add_library(thirdparty STATIC
        ${TFSXML_DIR}/tfsxml.c
        ${TINYXML2_SOURCES}
)

set_target_properties(thirdparty PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        C_VISIBILITY_PRESET hidden
        CXX_VISIBILITY_PRESET hidden
)
target_include_directories(thirdparty PUBLIC
        ${TFSXML_DIR}
        ${TINYXML2_DIR}
)

# -----------------------------
# MediaInfo
# -----------------------------
file(GLOB_RECURSE MEDIAINFO_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/MediaInfoLib/Source/MediaInfo/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/MediaInfoLib/Source/MediaInfo/*.c"
)

add_library(mediainfo STATIC ${MEDIAINFO_SOURCES})
set_target_properties(mediainfo PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        C_VISIBILITY_PRESET hidden
        CXX_VISIBILITY_PRESET hidden
)
target_link_libraries(mediainfo thirdparty zen)

# -----------------------------
# JNI bridge - MAIN LIBRARY FOR AAR
# -----------------------------
add_library(mediainfo-android SHARED mediainfo-jni.cpp)  # Changed to match your Java loadLibrary name

target_include_directories(mediainfo-android PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ZenLib/Source
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaInfoLib/Source
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaInfoLib/Source/MediaInfo
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link all dependencies
target_link_libraries(mediainfo-android
        mediainfo
        thirdparty
        zen
        log
        z
)

# Set library version for AAR
set_target_properties(mediainfo-android PROPERTIES
        VERSION 1.0.0
        SOVERSION 1
        OUTPUT_NAME "mediainfo-android"  # Ensures libmediainfo-android.so
)