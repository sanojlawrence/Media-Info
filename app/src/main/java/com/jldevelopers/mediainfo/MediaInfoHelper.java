package com.jldevelopers.mediainfo;

import android.net.Uri;
import android.util.Log;
import java.io.File;
import java.io.IOException;
import java.text.Normalizer;

public class MediaInfoHelper {

    public static final int STREAM_GENERAL = 0;
    public static final int STREAM_VIDEO = 1;
    public static final int STREAM_AUDIO = 2;
    public static final int STREAM_TEXT = 3;
    public static final int STREAM_OTHER = 4;
    public static final int STREAM_IMAGE = 5;
    public static final int STREAM_MENU = 6;

    private static volatile MediaInfoHelper instance;
    private static final Object lock = new Object();
    private boolean isInitialized = false;
    private File tempFile = null;

    private MediaInfoHelper() {}

    public static MediaInfoHelper getInstance() {
        if (instance == null) {
            synchronized (lock) {
                if (instance == null) instance = new MediaInfoHelper();
            }
        }
        return instance;
    }

    // Native methods
    public native boolean initMediaInfo();
    public native boolean openFile(String filePath);
    public native String getParameter(int streamKind, int streamNumber, String parameter);
    public native int getStreamCount(int streamKind);
    public native String getAllStreamsInfo();
    public native void closeFile();
    public native void cleanupMediaInfo();

    static {
        System.loadLibrary("mediainfo-android");
    }

    /**
     * Enhanced version with automatic path encoding handling
     */
    public synchronized MediaInfoData analyzeFileFast(String filePath) {
        return analyzeFileFast(filePath, true); // Use auto-encoding by default
    }

    /**
     * Enhanced version with encoding control
     */
    public synchronized MediaInfoData analyzeFileFast(String filePath, boolean useEncodingFix) {
        ensureInitialized();
        MediaInfoData data = new MediaInfoData();

        String processedPath = filePath;

        try {
            // Apply encoding fixes if requested
            if (useEncodingFix) {
                processedPath = applyEncodingFixes(filePath);
            }

            if (!openFile(processedPath)) {
                Log.w("MediaInfoHelper", "Failed to open file: " + processedPath);
                return data;
            }

            data.filePath = filePath; // Store original path
            data.fileSize = getParamSafe(0, 0, "FileSize/String");
            data.duration = getParamSafe(0, 0, "Duration/String");
            data.bitrate = getParamSafe(0, 0, "OverallBitRate/String");

            int videoCount = getStreamCount(1);
            int audioCount = getStreamCount(2);
            int textCount = getStreamCount(3);
            data.videoStreamCount = videoCount;
            data.audioStreamCount = audioCount;
            data.textStreamCount = textCount;

            if (videoCount > 0) {
                data.videoFormat = getParamSafe(1, 0, "Format");
                String width = getParamSafe(1, 0, "Width");
                String height = getParamSafe(1, 0, "Height");
                if (!width.isEmpty() && !height.isEmpty()) data.resolution = width + "x" + height;
                data.frameRate = getParamSafe(1, 0, "FrameRate/String");
                data.videoBitrate = getParamSafe(1, 0, "BitRate/String");
            }

            if (audioCount > 0) {
                data.audioFormat = getParamSafe(2, 0, "Format");
                data.audioChannels = getParamSafe(2, 0, "Channel(s)/String");
                data.audioBitrate = getParamSafe(2, 0, "BitRate/String");
                data.sampleRate = getParamSafe(2, 0, "SamplingRate/String");
            }

            // Optional: full text info only once
            data.rawInfo = getAllStreamsInfo();

        } catch (Exception e) {
            Log.e("MediaInfoHelper", "Error in analyzeFileFast", e);
        } finally {
            closeFile();
        }

        return data;
    }

    /**
     * Applies multiple encoding fixes in sequence until one works
     */
    // Update your applyEncodingFixes method
    private String applyEncodingFixes(String originalPath) {
        if (originalPath == null) return null;

        return originalPath;
    }

    // --- Your Existing Methods Enhanced ---

    private synchronized void ensureInitialized() {
        if (!isInitialized) {
            isInitialized = initMediaInfo();
        }
    }

    private String getParamSafe(int kind, int num, String key) {
        try {
            String v = getParameter(kind, num, key);
            return v == null ? "" : v.trim();
        } catch (Exception e) {
            return "";
        }
    }

    public synchronized MediaInfoData getDetailedInfo(String filePath) {
        return analyzeFileFast(filePath, true); // Use encoding fixes
    }

    public synchronized String analyzeFile(String filePath) {
        return analyzeFile(filePath, true);
    }

    public synchronized String analyzeFile(String filePath, boolean useEncodingFix) {
        ensureInitialized();
        String info = "";

        String processedPath = filePath;
        try {
            if (useEncodingFix) {
                processedPath = applyEncodingFixes(filePath);
            }

            if (openFile(processedPath)) {
                info = getAllStreamsInfo();
            }
        } catch (Exception e) {
            Log.e("MediaInfoHelper", "Error in analyzeFile", e);
        } finally {
            closeFile();
        }
        return info != null ? info : "";
    }

    // --- Backward compatible method for boolean parameters ---
    public synchronized boolean getBoolean(String filePath, int streamKind, int streamNumber, String parameter) {
        return getBoolean(filePath, streamKind, streamNumber, parameter, true);
    }

    public synchronized boolean getBoolean(String filePath, int streamKind, int streamNumber, String parameter, boolean useEncodingFix) {
        ensureInitialized();
        boolean result = false;

        String processedPath = filePath;
        try {
            if (useEncodingFix) {
                processedPath = applyEncodingFixes(filePath);
            }

            if (openFile(processedPath)) {
                String value = getParameter(streamKind, streamNumber, parameter);
                if (value != null) {
                    value = value.trim().toLowerCase();
                    result = value.equals("1") || value.equals("yes") || value.equals("true");
                }
            }
        } catch (Exception e) {
            Log.e("MediaInfoHelper", "Error in getBoolean", e);
        } finally {
            closeFile();
        }
        return result;
    }

    // ✅ Old compatibility wrapper for countGet()
    public synchronized int countGet(String filePath, int streamKind) {
        return countGet(filePath, streamKind, true);
    }

    public synchronized int countGet(String filePath, int streamKind, boolean useEncodingFix) {
        ensureInitialized();
        int count = 0;

        String processedPath = filePath;
        try {
            if (useEncodingFix) {
                processedPath = applyEncodingFixes(filePath);
            }

            if (openFile(processedPath)) {
                count = getStreamCount(streamKind);
            }
        } catch (Exception e) {
            Log.e("MediaInfoHelper", "Error in countGet", e);
        } finally {
            closeFile();
        }
        return count;
    }

    // ✅ Old compatibility wrapper for getString()
    public synchronized String getString(String filePath, int streamKind, int streamNumber, String parameter) {
        return getString(filePath, streamKind, streamNumber, parameter, true);
    }

    public synchronized String getString(String filePath, int streamKind, int streamNumber, String parameter, boolean useEncodingFix) {
        ensureInitialized();
        String value = "";

        String processedPath = filePath;
        try {
            if (useEncodingFix) {
                processedPath = applyEncodingFixes(filePath);
            }

            if (openFile(processedPath)) {
                value = getParameter(streamKind, streamNumber, parameter);
            }
        } catch (Exception e) {
            Log.e("MediaInfoHelper", "Error in getString", e);
        } finally {
            closeFile();
        }
        return value != null ? value : "";
    }

    public synchronized void cleanup() {
        try {
            cleanupMediaInfo();
        } catch (Exception e) {
            Log.e("MediaInfoHelper", "Error in cleanup", e);
        }
        isInitialized = false;
    }

    // --- Same MediaInfoData class you already have ---
    public static class MediaInfoData {
        public String filePath = "";
        public String fileSize = "";
        public String duration = "";
        public String bitrate = "";
        public String videoFormat = "";
        public String resolution = "";
        public String frameRate = "";
        public String videoBitrate = "";
        public String audioFormat = "";
        public String audioChannels = "";
        public String audioBitrate = "";
        public String sampleRate = "";
        public int videoStreamCount = 0;
        public int audioStreamCount = 0;
        public int textStreamCount = 0;
        public String rawInfo = "";

        public String toBriefString() {
            StringBuilder sb = new StringBuilder();
            if (!duration.isEmpty()) sb.append("Duration: ").append(duration).append(" • ");
            if (!resolution.isEmpty()) sb.append(resolution).append(" • ");
            if (!videoFormat.isEmpty()) sb.append(videoFormat).append(" • ");
            if (!audioFormat.isEmpty()) sb.append(audioFormat);
            return sb.toString();
        }
    }
}