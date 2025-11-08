package com.jldevelopers.mediainfo;

public class MediaInfoFormatter {

    /**
     * Format MediaInfo output into a clean 2-column layout
     */
    public static String formatAsTwoColumns(String mediaInfoOutput) {
        if (mediaInfoOutput == null || mediaInfoOutput.isEmpty()) {
            return "No media information available";
        }

        StringBuilder formatted = new StringBuilder();
        String[] lines = mediaInfoOutput.split("\n");

        for (String line : lines) {
            if (line.trim().isEmpty()) {
                formatted.append("\n");
                continue;
            }

            // Check if this is a section header (like "General", "Video #0")
            if (!line.contains(" : ") && !line.startsWith(" ")) {
                formatted.append("\n").append(line).append("\n");
                formatted.append("=".repeat(50)).append("\n");
                continue;
            }

            // Split parameter and value
            if (line.contains(" : ")) {
                String[] parts = line.split(" : ", 2);
                if (parts.length == 2) {
                    String parameter = formatParameterName(parts[0].trim());
                    String value = parts[1].trim();

                    // Create two-column layout
                    String formattedLine = String.format("%-25s : %s", parameter, value);
                    formatted.append(formattedLine).append("\n");
                }
            }
        }

        return formatted.toString().trim();
    }

    /**
     * Clean up parameter names for better readability
     */
    private static String formatParameterName(String parameter) {
        // Remove common prefixes and format nicely
        return parameter
                .replace("CompleteName", "File Name")
                .replace("FileSize/String", "File Size")
                .replace("Duration/String", "Duration")
                .replace("OverallBitRate/String", "Overall Bitrate")
                .replace("FrameRate/String", "Frame Rate")
                .replace("StreamSize/String", "Stream Size")
                .replace("BitRate/String", "Bitrate")
                .replace("DisplayAspectRatio/String", "Aspect Ratio")
                .replace("Channel(s)/String", "Channels")
                .replace("SamplingRate/String", "Sample Rate")
                .replace("BitRate_Mode/String", "Bitrate Mode")
                .replace("Compression_Mode/String", "Compression")
                .replace("ScanType/String", "Scan Type")
                .replace("BitDepth/String", "Bit Depth")
                .replace("Format/Info", "Format Info")
                .replace("Encoded_", "")
                .replace("_", " ");
    }
}