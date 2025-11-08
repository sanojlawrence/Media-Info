# MediaInfo Android

A lightweight Android wrapper for the powerful [MediaInfo](https://mediaarea.net/en/MediaInfo) library, providing comprehensive media file analysis capabilities for Android applications.

## Features

- 📊 **Complete Media Analysis**: Extract technical metadata from video, audio, and image files
- 🎬 **Video Information**: Codec, resolution, bitrate, framerate, duration
- 🔊 **Audio Information**: Codec, channels, sample rate, bitrate
- 📱 **Android Optimized**: Built specifically for Android with minimal footprint
- 🔧 **Easy Integration**: Simple Java API
- 🎯 **Multiple Formats**: Supports MP4, MKV, AVI, MP3, FLAC, and many more

## Installation

### Method 1: AAR File (Recommended)

1. Download the latest `mediainfo-android-release.aar` from [Releases](https://github.com/yourusername/mediainfo-android/releases)
2. Place it in your app's `libs` directory:
   ```
   app/
   └── libs/
       └── mediainfo-android-release.aar
   ```

3. Add to your `app/build.gradle`:
   ```gradle
   dependencies {
       implementation files('libs/mediainfo-android-release.aar')
   }
   ```

### Method 2: Gradle (Coming Soon)

```gradle
dependencies {
    implementation 'com.jldevelopers:mediainfo-android:1.0.0'
}
```

## Quick Start

### Basic Usage

```java
public class MainActivity extends AppCompatActivity {
    private MediaInfoHelper mediaInfoHelper;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        mediaInfoHelper = new MediaInfoHelper();
        
        if (mediaInfoHelper.initMediaInfo()) {
            // Analyze a media file
            String filePath = "/storage/emulated/0/Download/video.mp4";
            String mediaInfo = mediaInfoHelper.getGeneralInfo(filePath);
            
            Log.d("MediaInfo", mediaInfo);
            
            // Display in TextView
            TextView infoText = findViewById(R.id.info_text);
            infoText.setText(mediaInfo);
        }
    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mediaInfoHelper != null) {
            mediaInfoHelper.cleanupMediaInfo();
        }
    }
}
```

### Advanced Usage

```java
// Initialize
MediaInfoHelper mediaInfoHelper = new MediaInfoHelper();
boolean initialized = mediaInfoHelper.initMediaInfo();

if (initialized) {
    // Open specific file
    int fileIndex = mediaInfoHelper.openFile("/path/to/media/file.mp4");
    
    if (fileIndex >= 0) {
        // Get specific parameters
        String duration = mediaInfoHelper.getParameter(fileIndex, "Duration");
        String format = mediaInfoHelper.getParameter(fileIndex, "Format");
        String videoCodec = mediaInfoHelper.getParameterWithStream(
            fileIndex, 
            MediaInfoHelper.STREAM_VIDEO, 
            0, 
            "Format"
        );
        
        // Get complete technical information
        String fullInfo = mediaInfoHelper.getFullInfo(fileIndex);
        
        // Close when done
        mediaInfoHelper.closeAll();
        
        // Display results
        Log.d("MediaInfo", "Duration: " + duration);
        Log.d("MediaInfo", "Format: " + format);
        Log.d("MediaInfo", "Video Codec: " + videoCodec);
    }
    
    // Cleanup
    mediaInfoHelper.cleanupMediaInfo();
}
```

## API Reference

### Core Methods

| Method | Description |
|--------|-------------|
| `boolean initMediaInfo()` | Initialize the MediaInfo library |
| `int openFile(String filePath)` | Open a media file for analysis |
| `String getGeneralInfo(String filePath)` | Quick analysis with full information |
| `String getFullInfo(int fileIndex)` | Get complete technical information |
| `String getParameter(int fileIndex, String parameter)` | Get specific parameter value |
| `String getParameterWithStream(int fileIndex, int streamKind, int streamNumber, String parameter)` | Get parameter from specific stream |
| `int getStreamCount(int fileIndex, int streamKind)` | Get count of streams for a type |
| `void closeAll()` | Close all opened files |
| `void cleanupMediaInfo()` | Clean up resources |

### Stream Types Constants

```java
MediaInfoHelper.STREAM_GENERAL  // Container information
MediaInfoHelper.STREAM_VIDEO    // Video stream
MediaInfoHelper.STREAM_AUDIO    // Audio stream
MediaInfoHelper.STREAM_TEXT     // Subtitle/text stream
MediaInfoHelper.STREAM_OTHER    // Other stream types
MediaInfoHelper.STREAM_IMAGE    // Image stream
MediaInfoHelper.STREAM_MENU     // Menu stream
```

### Common Parameters

| Parameter | Description | Example |
|-----------|-------------|---------|
| `Duration` | Media duration | "02:30:15.500" |
| `Format` | Container/codec format | "MPEG-4", "AVC" |
| `FileSize` | File size in bytes | "154822345" |
| `OverallBitRate` | Overall bitrate | "1524000" |
| `Width`, `Height` | Video dimensions | "1920", "1080" |
| `FrameRate` | Video framerate | "23.976" |
| `BitDepth` | Color/audio depth | "8", "16", "24" |
| `Channels` | Audio channels | "2", "5.1", "7.1" |
| `SamplingRate` | Audio sample rate | "48000" |

## Examples

### Get Video Information

```java
public String getVideoInfo(String filePath) {
    MediaInfoHelper helper = new MediaInfoHelper();
    if (helper.initMediaInfo()) {
        String info = helper.getGeneralInfo(filePath);
        helper.cleanupMediaInfo();
        return info;
    }
    return "Failed to initialize MediaInfo";
}
```

### Extract Specific Properties

```java
public Map<String, String> getMediaProperties(String filePath) {
    MediaInfoHelper helper = new MediaInfoHelper();
    Map<String, String> properties = new HashMap<>();
    
    if (helper.initMediaInfo()) {
        int fileIndex = helper.openFile(filePath);
        
        if (fileIndex >= 0) {
            properties.put("Duration", helper.getParameter(fileIndex, "Duration"));
            properties.put("Format", helper.getParameter(fileIndex, "Format"));
            properties.put("FileSize", helper.getParameter(fileIndex, "FileSize"));
            properties.put("VideoCodec", helper.getParameterWithStream(
                fileIndex, MediaInfoHelper.STREAM_VIDEO, 0, "Format"));
            properties.put("AudioCodec", helper.getParameterWithStream(
                fileIndex, MediaInfoHelper.STREAM_AUDIO, 0, "Format"));
            
            helper.closeAll();
        }
        helper.cleanupMediaInfo();
    }
    
    return properties;
}
```

### Get Video Resolution

```java
public String getVideoResolution(String filePath) {
    MediaInfoHelper helper = new MediaInfoHelper();
    String resolution = "";
    
    if (helper.initMediaInfo()) {
        int fileIndex = helper.openFile(filePath);
        
        if (fileIndex >= 0) {
            String width = helper.getParameterWithStream(
                fileIndex, MediaInfoHelper.STREAM_VIDEO, 0, "Width");
            String height = helper.getParameterWithStream(
                fileIndex, MediaInfoHelper.STREAM_VIDEO, 0, "Height");
            
            if (!width.isEmpty() && !height.isEmpty()) {
                resolution = width + "x" + height;
            }
            
            helper.closeAll();
        }
        helper.cleanupMediaInfo();
    }
    
    return resolution;
}
```

### Batch File Analysis

```java
public List<MediaFileInfo> analyzeMultipleFiles(List<String> filePaths) {
    MediaInfoHelper helper = new MediaInfoHelper();
    List<MediaFileInfo> results = new ArrayList<>();
    
    if (helper.initMediaInfo()) {
        for (String filePath : filePaths) {
            int fileIndex = helper.openFile(filePath);
            
            if (fileIndex >= 0) {
                MediaFileInfo info = new MediaFileInfo();
                info.setFilePath(filePath);
                info.setDuration(helper.getParameter(fileIndex, "Duration"));
                info.setFormat(helper.getParameter(fileIndex, "Format"));
                info.setFileSize(helper.getParameter(fileIndex, "FileSize"));
                info.setVideoCodec(helper.getParameterWithStream(
                    fileIndex, MediaInfoHelper.STREAM_VIDEO, 0, "Format"));
                
                results.add(info);
                helper.closeAll();
            }
        }
        helper.cleanupMediaInfo();
    }
    
    return results;
}

// Helper class
public class MediaFileInfo {
    private String filePath;
    private String duration;
    private String format;
    private String fileSize;
    private String videoCodec;
    
    // Getters and setters
    public String getFilePath() { return filePath; }
    public void setFilePath(String filePath) { this.filePath = filePath; }
    
    public String getDuration() { return duration; }
    public void setDuration(String duration) { this.duration = duration; }
    
    public String getFormat() { return format; }
    public void setFormat(String format) { this.format = format; }
    
    public String getFileSize() { return fileSize; }
    public void setFileSize(String fileSize) { this.fileSize = fileSize; }
    
    public String getVideoCodec() { return videoCodec; }
    public void setVideoCodec(String videoCodec) { this.videoCodec = videoCodec; }
}
```

## Supported Formats

### Video Formats
- **MP4**, **MKV**, **AVI**, **MOV**, **WMV**, **FLV**, **WebM**
- **MPEG-1**, **MPEG-2**, **MPEG-4**
- **H.264**, **H.265**, **VP9**, **AV1**
- **ProRes**, **DNxHD**

### Audio Formats
- **MP3**, **AAC**, **FLAC**, **ALAC**, **WAV**, **AIFF**
- **AC-3**, **DTS**, **Opus**, **Vorbis**
- **WMA**, **OGG**

### Image Formats
- **JPEG**, **PNG**, **GIF**, **BMP**, **WebP**
- **TIFF**, **HEIC**

## Permissions

Add the following to your `AndroidManifest.xml`:

```xml
<uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE" />
<!-- For Android 10+ -->
<uses-permission android:name="android.permission.MANAGE_EXTERNAL_STORAGE" 
                 tools:ignore="ScopedStorage" />
```

For Android 10+, you may need to use the Storage Access Framework:

```java
// Request file access
Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
intent.addCategory(Intent.CATEGORY_OPENABLE);
intent.setType("*/*");
startActivityForResult(intent, REQUEST_CODE_PICK_FILE);
```

## ProGuard

If you're using ProGuard, add these rules to your `proguard-rules.pro`:

```proguard
-keep class com.jldevelopers.mediainfosource.MediaInfoHelper { *; }
-keepclasseswithmembers class * {
    native <methods>;
}
```

## Complete Example Activity

```java
public class MediaAnalysisActivity extends AppCompatActivity {
    private static final int REQUEST_PICK_FILE = 1001;
    private MediaInfoHelper mediaInfoHelper;
    private TextView resultText;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_media_analysis);
        
        resultText = findViewById(R.id.result_text);
        Button selectFileButton = findViewById(R.id.select_file_button);
        
        mediaInfoHelper = new MediaInfoHelper();
        
        selectFileButton.setOnClickListener(v -> selectMediaFile());
        
        // Initialize MediaInfo
        if (!mediaInfoHelper.initMediaInfo()) {
            resultText.setText("Failed to initialize MediaInfo library");
        }
    }
    
    private void selectMediaFile() {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("*/*");
        startActivityForResult(intent, REQUEST_PICK_FILE);
    }
    
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        
        if (requestCode == REQUEST_PICK_FILE && resultCode == RESULT_OK) {
            if (data != null) {
                Uri uri = data.getData();
                analyzeMediaFile(uri);
            }
        }
    }
    
    private void analyzeMediaFile(Uri fileUri) {
        try {
            // Get file path from URI
            String filePath = getFilePathFromUri(fileUri);
            
            if (filePath != null) {
                // Get media information
                String mediaInfo = mediaInfoHelper.getGeneralInfo(filePath);
                resultText.setText(mediaInfo);
            } else {
                resultText.setText("Could not access file");
            }
        } catch (Exception e) {
            resultText.setText("Error analyzing file: " + e.getMessage());
        }
    }
    
    private String getFilePathFromUri(Uri uri) {
        // Implementation to get file path from URI
        // This depends on your specific requirements
        Cursor cursor = getContentResolver().query(uri, null, null, null, null);
        if (cursor != null) {
            int nameIndex = cursor.getColumnIndex(OpenableColumns.DISPLAY_NAME);
            cursor.moveToFirst();
            String fileName = cursor.getString(nameIndex);
            cursor.close();
            return fileName;
        }
        return null;
    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mediaInfoHelper != null) {
            mediaInfoHelper.cleanupMediaInfo();
        }
    }
}
```

## Performance Notes

- **Initialization**: First call to `initMediaInfo()` may take 50-100ms
- **File Analysis**: Typically 10-500ms depending on file size and complexity
- **Memory**: Minimal memory footprint (~2-5MB additional)
- **Thread Safety**: Not thread-safe - use from a single thread or implement synchronization

## Troubleshooting

### Common Issues

1. **"MediaInfo not initialized"**
   - Ensure you call `initMediaInfo()` before other methods
   - Check that the native library is properly loaded

2. **File not found errors**
   - Verify file path is correct and accessible
   - Check storage permissions
   - Use proper file path methods for your Android version

3. **Native library loading fails**
   - Ensure AAR contains native libraries for your device's architecture
   - Check for conflicting C++ runtime libraries

### Logging

Enable debug logging:

```java
MediaInfoHelper helper = new MediaInfoHelper();

// Check initialization
boolean success = helper.initMediaInfo();
Log.d("MediaInfo", "Initialization: " + success);

// Check file opening
int fileIndex = helper.openFile(filePath);
Log.d("MediaInfo", "File opened with index: " + fileIndex);

if (fileIndex >= 0) {
    // Test various parameters
    String duration = helper.getParameter(fileIndex, "Duration");
    String format = helper.getParameter(fileIndex, "Format");
    Log.d("MediaInfo", "Duration: " + duration);
    Log.d("MediaInfo", "Format: " + format);
    
    helper.closeAll();
}
helper.cleanupMediaInfo();
```

## Building from Source

### Prerequisites

- Android Studio Arctic Fox or later
- Android NDK 25 or later
- CMake 3.22 or later

### Build Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/mediainfo-android.git
   ```

2. Open in Android Studio
3. Build the project:
   ```bash
   ./gradlew assembleRelease
   ```

4. Find the AAR in `app/build/outputs/aar/`

## License

```
Copyright 2024 JL Developers

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```

This project includes MediaInfo library which is licensed under its own terms.

## Support

- 📧 **Email**: support@jldevelopers.com
- 🐛 **Issues**: [GitHub Issues](https://github.com/yourusername/mediainfo-android/issues)
- 💬 **Discussions**: [GitHub Discussions](https://github.com/yourusername/mediainfo-android/discussions)

---

<div align="center">

**Made with ❤️ by JL Developers**

If this project helps you, please give it a ⭐️!

</div>
