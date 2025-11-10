#include <jni.h>
#include <string>
#include <vector>
#include <android/log.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <android/log.h>
#include <codecvt>
#include <locale>
#include <string>
#include <vector>

// Include MediaInfo headers
#include "MediaInfo/MediaInfo.h"

// Global variable declaration - CHOOSE ONE and be consistent!
MediaInfoLib::MediaInfo* g_mediaInfo = nullptr;
// OR MediaInfoLib::MediaInfoList* g_mediaInfoList = nullptr;

// Add this at the top of your C++ file
#ifdef NDEBUG
    #define LOGI(...) ((void)0)
    #define LOGE(...) ((void)0)
#else
    #define LOG_TAG "MediaInfoJNI"
    #define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
    #define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

// Helper function declarations (C++ functions)
std::string MediaInfoStringToStdString(const MediaInfoLib::String& miString);
MediaInfoLib::String StdStringToMediaInfoString(const std::string& stdString);
std::string getGeneralStreamDetails();
std::string getVideoStreamDetails(int streamNumber);
std::string getAudioStreamDetails(int streamNumber);
std::string getTextStreamDetails(int streamNumber);
std::string getOtherStreamDetails(int streamNumber);

// ============================================================================
// Helper function implementations (C++ functions - outside extern "C")
// ============================================================================

// Helper function to convert MediaInfoLib::String to std::string
std::string MediaInfoStringToStdString(const MediaInfoLib::String& miString) {
    std::string result;
    for (size_t i = 0; i < miString.size(); ++i) {
        result += (char)miString[i];
    }
    return result;
}

// Helper function to convert std::string to MediaInfoLib::String
MediaInfoLib::String StdStringToMediaInfoString(const std::string& stdString) {
    MediaInfoLib::String result;
    for (char c : stdString) {
        result += (MediaInfoLib::Char)c;
    }
    return result;
}

// Helper methods for detailed stream information
std::string getGeneralStreamDetails() {
    if (g_mediaInfo == nullptr) {
        LOGE("MediaInfo not initialized in getGeneralStreamDetails");
        return "MediaInfo not initialized\n";
    }

    std::string result = "General\n";
    std::vector<std::string> generalParams = {
            "CompleteName",
            "Format",
            "Format/Info",
            "Format/Commercial",
            "FileSize/String",
            "Duration/String",
            "OverallBitRate/String",
            "FrameRate/String",
            "StreamCount",
            "VideoCount",
            "AudioCount",
            "TextCount",
            "MenuCount",
            "WritingApplication",
            "WritingLibrary"
    };

    for (const auto& param : generalParams) {
        MediaInfoLib::String value = g_mediaInfo->Get(
                MediaInfoLib::Stream_General, 0,
                StdStringToMediaInfoString(param),
                MediaInfoLib::Info_Text, MediaInfoLib::Info_Name
        );

        if (!value.empty()) {
            result += param + " : " + MediaInfoStringToStdString(value) + "\n";
        }
    }

    return result + "\n";
}

std::string getVideoStreamDetails(int streamNumber) {
    if (g_mediaInfo == nullptr) {
        LOGE("MediaInfo not initialized in getVideoStreamDetails");
        return "MediaInfo not initialized\n";
    }

    std::string result = "Video #" + std::to_string(streamNumber) + "\n";

    std::vector<std::string> videoParams = {
            "Format",
            "Format/Info",
            "Format/Commercial",
            "Format_Profile",
            "Format_Level",
            "Format_Settings",
            "CodecID",
            "Duration/String",
            "BitRate/String",
            "Width",
            "Height",
            "DisplayAspectRatio",
            "DisplayAspectRatio/String",
            "FrameRate/String",
            "Standard",
            "ColorSpace",
            "ChromaSubsampling",
            "BitDepth",
            "ScanType",
            "ScanType/String",
            "Bits-(Pixel*Frame)",
            "StreamSize/String",
            "Language",
            "Default",
            "Forced"
    };

    for (const auto& param : videoParams) {
        MediaInfoLib::String value = g_mediaInfo->Get(
                MediaInfoLib::Stream_Video, streamNumber,
                StdStringToMediaInfoString(param),
                MediaInfoLib::Info_Text, MediaInfoLib::Info_Name
        );

        if (!value.empty()) {
            result += param + " : " + MediaInfoStringToStdString(value) + "\n";
        }
    }

    return result + "\n";
}

std::string getAudioStreamDetails(int streamNumber) {
    if (g_mediaInfo == nullptr) {
        LOGE("MediaInfo not initialized in getAudioStreamDetails");
        return "MediaInfo not initialized\n";
    }

    std::string result = "Audio #" + std::to_string(streamNumber) + "\n";

    std::vector<std::string> audioParams = {
            "Format",
            "Format/Info",
            "Format/Commercial",
            "Format_Commercial_IfAny",        // Alternative commercial name
            "Format_Profile",
            "CodecID",
            "Duration/String",
            "BitRate/String",
            "BitRate_Mode",
            "BitRate_Mode/String",
            "Channel(s)",
            "Channel(s)/String",
            "ChannelPositions",
            "ChannelLayout",
            "SamplingRate",
            "SamplingRate/String",
            "FrameRate/String",
            "Compression_Mode",
            "StreamSize/String",
            "BitDepth",
            "BitDepth/String",
            "Language",
            "Default",
            "Forced"
    };

    for (const auto& param : audioParams) {
        MediaInfoLib::String value = g_mediaInfo->Get(
                MediaInfoLib::Stream_Audio, streamNumber,
                StdStringToMediaInfoString(param),
                MediaInfoLib::Info_Text, MediaInfoLib::Info_Name
        );

        if (!value.empty()) {
            result += param + " : " + MediaInfoStringToStdString(value) + "\n";
        }
    }

    return result + "\n";
}

std::string getTextStreamDetails(int streamNumber) {
    if (g_mediaInfo == nullptr) {
        LOGE("MediaInfo not initialized in getTextStreamDetails");
        return "MediaInfo not initialized\n";
    }

    std::string result = "Text #" + std::to_string(streamNumber) + "\n";

    std::vector<std::string> textParams = {
            "Format",
            "Format/Info",
            "Format/Commercial",
            "CodecID",
            "Duration/String",
            "BitRate",
            "BitRate/String",
            "FrameRate/String",
            "StreamSize/String",
            "Language",
            "Default",
            "Forced"
    };

    for (const auto& param : textParams) {
        MediaInfoLib::String value = g_mediaInfo->Get(
                MediaInfoLib::Stream_Text, streamNumber,
                StdStringToMediaInfoString(param),
                MediaInfoLib::Info_Text, MediaInfoLib::Info_Name
        );

        if (!value.empty()) {
            result += param + " : " + MediaInfoStringToStdString(value) + "\n";
        }
    }

    return result + "\n";
}

std::string getOtherStreamDetails(int streamNumber) {
    if (g_mediaInfo == nullptr) {
        LOGE("MediaInfo not initialized in getOtherStreamDetails");
        return "MediaInfo not initialized\n";
    }

    std::string result = "Other #" + std::to_string(streamNumber) + "\n";

    std::vector<std::string> otherParams = {
            "Format",
            "Format/Info",
            "CodecID",
            "Duration",
            "Duration/String",
            "BitRate",
            "BitRate/String",
            "StreamSize",
            "StreamSize/String",
            "Language",
            "Default",
            "Forced"
    };

    for (const auto& param : otherParams) {
        MediaInfoLib::String value = g_mediaInfo->Get(
                MediaInfoLib::Stream_Other, streamNumber,
                StdStringToMediaInfoString(param),
                MediaInfoLib::Info_Text, MediaInfoLib::Info_Name
        );

        if (!value.empty()) {
            result += param + " : " + MediaInfoStringToStdString(value) + "\n";
        }
    }

    return result + "\n";
}

// Helper function to convert jstring to std::string (UTF-8)
std::string jstringToUtf8(JNIEnv* env, jstring jstr) {
    if (!jstr) return "";

    const jchar* jchars = env->GetStringChars(jstr, nullptr);
    jsize len = env->GetStringLength(jstr);

    // Get the required buffer size for UTF-8
    jsize utf8_len = env->GetStringUTFLength(jstr);
    std::vector<char> buffer(utf8_len + 1);

    // Convert to UTF-8
    env->GetStringUTFRegion(jstr, 0, len, buffer.data());

    std::string result(buffer.data());
    env->ReleaseStringChars(jstr, jchars);

    return result;
}

// Implement other helper functions (getVideoStreamDetails, getAudioStreamDetails, etc.)
// ... [your implementations here]

// ============================================================================
// JNI function implementations (inside extern "C")
// ============================================================================

#ifdef __cplusplus
extern "C" {
#endif

// Initialize MediaInfo
JNIEXPORT jboolean JNICALL
Java_com_jldevelopers_mediainfo_MediaInfoHelper_initMediaInfo(JNIEnv *env, jobject thiz) {
    try {
        if (g_mediaInfo == nullptr) {
            g_mediaInfo = new MediaInfoLib::MediaInfo();

            LOGI("=== INITIALIZING MEDIAINFO FOR URL SUPPORT ===");

            // Essential configuration options
            g_mediaInfo->Option(StdStringToMediaInfoString("ParseSpeed"), StdStringToMediaInfoString("0.5"));
            g_mediaInfo->Option(StdStringToMediaInfoString("Complete"), StdStringToMediaInfoString(""));

            LOGI("MediaInfo configured for URL support");
            return JNI_TRUE;
        }
        return JNI_TRUE;
    } catch (const std::exception& e) {
        LOGE("Failed to initialize MediaInfo: %s", e.what());
        return JNI_FALSE;
    } catch (...) {
        LOGE("Unknown exception initializing MediaInfo");
        return JNI_FALSE;
    }
}

// Open a file for analysis - USING DIRECT MEDIAINFO
JNIEXPORT jboolean JNICALL
Java_com_jldevelopers_mediainfo_MediaInfoHelper_openFile(JNIEnv *env, jobject thiz, jstring file_path) {
    // Check initialization
    if (!g_mediaInfo) {
        __android_log_print(ANDROID_LOG_ERROR, "MediaInfoJNI", "MediaInfo instance not initialized");
        return JNI_FALSE;
    }

    // Validate input
    if (file_path == nullptr) {
        __android_log_print(ANDROID_LOG_ERROR, "MediaInfoJNI", "Invalid file path (null)");
        return JNI_FALSE;
    }

    // Get UTF-16 chars directly from Java string
    const jchar* jchars = env->GetStringChars(file_path, nullptr);
    jsize len = env->GetStringLength(file_path);

    // Build MediaInfoLib::String from UTF-16 input
    MediaInfoLib::String mediaInfoPath;
    mediaInfoPath.reserve(len);
    for (jsize i = 0; i < len; ++i) {
        mediaInfoPath += (MediaInfoLib::Char)jchars[i];
    }

    // Release Java chars
    env->ReleaseStringChars(file_path, jchars);

    // Log path (for debugging only)
    // Note: Logging UTF-16 directly can show garbled output for non-ASCII chars, so convert to UTF-8 for logs
    const char *utf8Path = env->GetStringUTFChars(file_path, nullptr);
    env->ReleaseStringUTFChars(file_path, utf8Path);

    // Close any previously opened file and open the new one
    g_mediaInfo->Close();
    size_t result = g_mediaInfo->Open(mediaInfoPath);

    // Log and return success/failure
    if (result == 0) {
        __android_log_print(ANDROID_LOG_ERROR, "MediaInfoJNI", "Failed to open media file");
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

// Get media information as string - USING DIRECT MEDIAINFO
JNIEXPORT jstring JNICALL
Java_com_jldevelopers_mediainfo_MediaInfoHelper_getInform(JNIEnv *env, jobject thiz) {
    if (g_mediaInfo == nullptr) {
        LOGE("MediaInfo not initialized in getInform");
        return env->NewStringUTF("MediaInfo not initialized");
    }

    try {
        LOGI("Calling Inform() with direct MediaInfo");
        MediaInfoLib::String inform = g_mediaInfo->Inform();
        LOGI("Direct Inform() returned string of size: %zu", inform.size());

        if (inform.empty()) {
            LOGI("Direct Inform() returned empty string");
            return env->NewStringUTF("");
        }

        std::string result = MediaInfoStringToStdString(inform);
        LOGI("Converted to std::string of size: %zu", result.size());

        return env->NewStringUTF(result.c_str());
    } catch (const std::exception& e) {
        LOGE("Exception in getInform: %s", e.what());
        return env->NewStringUTF("Error getting media information");
    } catch (...) {
        LOGE("Unknown exception in getInform");
        return env->NewStringUTF("Error getting media information");
    }
}

// Get detailed information for a specific stream
JNIEXPORT jstring JNICALL
Java_com_jldevelopers_mediainfo_MediaInfoHelper_getStreamDetails(JNIEnv *env, jobject thiz, jint stream_kind, jint stream_number) {
    if (g_mediaInfo == nullptr) {
        return env->NewStringUTF("");
    }

    try {
        std::string result;

        // Common parameters for each stream type
        if (stream_kind == MediaInfoLib::Stream_General) {
            result += getGeneralStreamDetails();
        } else if (stream_kind == MediaInfoLib::Stream_Video) {
            result += getVideoStreamDetails(stream_number);
        } else if (stream_kind == MediaInfoLib::Stream_Audio) {
            result += getAudioStreamDetails(stream_number);
        } else if (stream_kind == MediaInfoLib::Stream_Text) {
            result += getTextStreamDetails(stream_number);
        } else if (stream_kind == MediaInfoLib::Stream_Other) {
            result += getOtherStreamDetails(stream_number);
        }

        return env->NewStringUTF(result.c_str());

    } catch (...) {
        return env->NewStringUTF("");
    }
}

// Get all streams information in mediainfo format
JNIEXPORT jstring JNICALL
Java_com_jldevelopers_mediainfo_MediaInfoHelper_getAllStreamsInfo(JNIEnv *env, jobject thiz) {
    if (g_mediaInfo == nullptr) {
        return env->NewStringUTF("MediaInfo not initialized");
    }

    try {
        std::string result;

        // Collect all metadata
        result += getGeneralStreamDetails();

        size_t videoCount = g_mediaInfo->Count_Get(MediaInfoLib::Stream_Video);
        for (size_t i = 0; i < videoCount; ++i)
            result += getVideoStreamDetails(i);

        size_t audioCount = g_mediaInfo->Count_Get(MediaInfoLib::Stream_Audio);
        for (size_t i = 0; i < audioCount; ++i)
            result += getAudioStreamDetails(i);

        size_t textCount = g_mediaInfo->Count_Get(MediaInfoLib::Stream_Text);
        for (size_t i = 0; i < textCount; ++i)
            result += getTextStreamDetails(i);

        size_t otherCount = g_mediaInfo->Count_Get(MediaInfoLib::Stream_Other);
        for (size_t i = 0; i < otherCount; ++i)
            result += getOtherStreamDetails(i);

        // ✅ Convert UTF-8 to UTF-16 manually (safe for all characters)
        std::u16string utf16;
        utf16.reserve(result.size());
        for (size_t i = 0; i < result.size();) {
            unsigned char c = result[i];
            if (c < 0x80) {  // ASCII
                utf16.push_back(static_cast<char16_t>(c));
                i++;
            } else if ((c >> 5) == 0x6 && i + 1 < result.size()) {  // 2-byte sequence
                char16_t ch = ((c & 0x1F) << 6) | (result[i + 1] & 0x3F);
                utf16.push_back(ch);
                i += 2;
            } else if ((c >> 4) == 0xE && i + 2 < result.size()) {  // 3-byte sequence
                char16_t ch = ((c & 0x0F) << 12) | ((result[i + 1] & 0x3F) << 6) | (result[i + 2] & 0x3F);
                utf16.push_back(ch);
                i += 3;
            } else {
                // Replace invalid bytes with '?'
                utf16.push_back(u'?');
                i++;
            }
        }

        // ✅ Use NewString (UTF-16-safe)
        return env->NewString(reinterpret_cast<const jchar*>(utf16.c_str()), utf16.length());

    } catch (const std::exception &e) {
        LOGE("Exception in getAllStreamsInfo: %s", e.what());
        return env->NewStringUTF("Error getting all streams information");
    } catch (...) {
        LOGE("Unknown exception in getAllStreamsInfo");
        return env->NewStringUTF("Error getting all streams information");
    }
}

// Get specific parameter value - USING DIRECT MEDIAINFO
JNIEXPORT jstring JNICALL
Java_com_jldevelopers_mediainfo_MediaInfoHelper_getParameter(JNIEnv *env, jobject thiz, jint stream_kind, jint stream_number, jstring parameter) {
    if (g_mediaInfo == nullptr) {
        LOGE("MediaInfo not initialized in getParameter");
        return env->NewStringUTF("");
    }

    const char *nativeParam = env->GetStringUTFChars(parameter, nullptr);
    if (nativeParam == nullptr) {
        LOGE("Failed to get parameter string");
        return env->NewStringUTF("");
    }

    LOGI("getParameter: stream_kind=%d, stream_number=%d, parameter='%s'",
         stream_kind, stream_number, nativeParam);

    try {
        MediaInfoLib::String mediaInfoParam = StdStringToMediaInfoString(nativeParam);

        MediaInfoLib::String value = g_mediaInfo->Get(
                (MediaInfoLib::stream_t)stream_kind,
                (size_t)stream_number,
                mediaInfoParam,
                MediaInfoLib::Info_Text,
                MediaInfoLib::Info_Name
        );

        env->ReleaseStringUTFChars(parameter, nativeParam);

        std::string result = MediaInfoStringToStdString(value);
        LOGI("Result for '%s': '%s'", nativeParam, result.c_str());
        return env->NewStringUTF(result.c_str());
    } catch (const std::exception& e) {
        LOGE("Exception in getParameter: %s", e.what());
        env->ReleaseStringUTFChars(parameter, nativeParam);
        return env->NewStringUTF("");
    } catch (...) {
        LOGE("Unknown exception in getParameter");
        env->ReleaseStringUTFChars(parameter, nativeParam);
        return env->NewStringUTF("");
    }
}

// Get count of streams for a specific type - USING DIRECT MEDIAINFO
JNIEXPORT jint JNICALL
Java_com_jldevelopers_mediainfo_MediaInfoHelper_getStreamCount(JNIEnv *env, jobject thiz, jint stream_kind) {
    if (g_mediaInfo == nullptr) {
        LOGE("MediaInfo not initialized in getStreamCount");
        return -1;
    }

    try {
        size_t count = g_mediaInfo->Count_Get((MediaInfoLib::stream_t)stream_kind);
        LOGI("getStreamCount: stream_kind=%d -> count=%zu", stream_kind, count);
        return (jint)count;
    } catch (const std::exception& e) {
        LOGE("Exception in getStreamCount: %s", e.what());
        return -1;
    } catch (...) {
        LOGE("Unknown exception in getStreamCount");
        return -1;
    }
}

// Get all information as a formatted string - USING DIRECT MEDIAINFO
JNIEXPORT jstring JNICALL
Java_com_jldevelopers_mediainfo_MediaInfoHelper_getFullInfo(JNIEnv *env, jobject thiz) {
    if (g_mediaInfo == nullptr) {
        return env->NewStringUTF("MediaInfo not initialized");
    }

    try {
        LOGI("Getting full info with direct MediaInfo");
        MediaInfoLib::String inform = g_mediaInfo->Inform();
        LOGI("Full info size: %zu", inform.size());

        std::string result = MediaInfoStringToStdString(inform);
        return env->NewStringUTF(result.c_str());
    } catch (const std::exception& e) {
        LOGE("Exception in getFullInfo: %s", e.what());
        return env->NewStringUTF("Error getting full information");
    } catch (...) {
        LOGE("Unknown exception in getFullInfo");
        return env->NewStringUTF("Error getting full information");
    }
}

// Close file - USING DIRECT MEDIAINFO
JNIEXPORT void JNICALL
Java_com_jldevelopers_mediainfo_MediaInfoHelper_closeFile(JNIEnv *env, jobject thiz) {
    if (g_mediaInfo != nullptr) {
        LOGI("Closing file with direct MediaInfo");
        g_mediaInfo->Close();
    }
}

// Cleanup MediaInfo
JNIEXPORT void JNICALL
Java_com_jldevelopers_mediainfo_MediaInfoHelper_cleanupMediaInfo(JNIEnv *env, jobject thiz) {
    if (g_mediaInfo != nullptr) {
        delete g_mediaInfo;
        g_mediaInfo = nullptr;
        LOGI("MediaInfo cleaned up");
    }
}

// Get version info
JNIEXPORT jstring JNICALL
Java_com_jldevelopers_mediainfo_MediaInfoHelper_getVersionInfo(JNIEnv *env, jobject thiz) {
    try {
        MediaInfoLib::String version = MediaInfoLib::MediaInfo::Option_Static(StdStringToMediaInfoString("Info_Version"), StdStringToMediaInfoString(""));

        std::string result = "MediaInfoLib Version: " + MediaInfoStringToStdString(version);

        LOGI("Version info: %s", result.c_str());
        return env->NewStringUTF(result.c_str());
    } catch (const std::exception& e) {
        LOGE("Exception in getVersionInfo: %s", e.what());
        return env->NewStringUTF("Error getting version info");
    } catch (...) {
        LOGE("Unknown exception in getVersionInfo");
        return env->NewStringUTF("Error getting version info");
    }
}

// Get complete inform similar to mediainfo command line
JNIEXPORT jstring JNICALL
Java_com_jldevelopers_mediainfo_MediaInfoHelper_getCompleteInform(JNIEnv *env, jobject thiz) {
    if (g_mediaInfo == nullptr) {
        return env->NewStringUTF("MediaInfo not initialized");
    }

    try {
        LOGI("Getting complete inform (mediainfo style)");

        // Configure for complete output like command line
        g_mediaInfo->Option(StdStringToMediaInfoString("Complete"), StdStringToMediaInfoString("1"));
        g_mediaInfo->Option(StdStringToMediaInfoString("Language"), StdStringToMediaInfoString("raw"));

        MediaInfoLib::String inform = g_mediaInfo->Inform();
        LOGI("Complete inform size: %zu", inform.size());

        std::string result = MediaInfoStringToStdString(inform);
        return env->NewStringUTF(result.c_str());

    } catch (const std::exception& e) {
        LOGE("Exception in getCompleteInform: %s", e.what());
        return env->NewStringUTF("Error getting complete information");
    } catch (...) {
        LOGE("Unknown exception in getCompleteInform");
        return env->NewStringUTF("Error getting complete information");
    }
}

// Add this to your JNI functions
JNIEXPORT jboolean JNICALL
Java_com_jldevelopers_mediainfo_MediaInfoHelper_isParameterNotBlank(
        JNIEnv *env, jobject thiz, jint stream_kind, jint stream_number, jstring parameter) {

    if (g_mediaInfo == nullptr) {
        return JNI_FALSE;
    }

    const char *nativeParam = env->GetStringUTFChars(parameter, nullptr);
    if (nativeParam == nullptr) {
        return JNI_FALSE;
    }

    try {
        MediaInfoLib::String mediaInfoParam = StdStringToMediaInfoString(nativeParam);

        MediaInfoLib::String value = g_mediaInfo->Get(
                (MediaInfoLib::stream_t)stream_kind,
                (size_t)stream_number,
                mediaInfoParam,
                MediaInfoLib::Info_Text,
                MediaInfoLib::Info_Name
        );

        env->ReleaseStringUTFChars(parameter, nativeParam);

        // Check if value is not empty
        return !value.empty() ? JNI_TRUE : JNI_FALSE;

    } catch (...) {
        env->ReleaseStringUTFChars(parameter, nativeParam);
        return JNI_FALSE;
    }
}

#ifdef __cplusplus
}
#endif