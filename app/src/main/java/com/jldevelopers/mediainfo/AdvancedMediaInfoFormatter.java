package com.jldevelopers.mediainfosource;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class AdvancedMediaInfoFormatter {

    /**
     * Advanced formatting with section grouping and better organization
     */
    public static String formatAdvanced(String mediaInfoOutput) {
        if (mediaInfoOutput == null || mediaInfoOutput.isEmpty()) {
            return "No media information available";
        }

        StringBuilder result = new StringBuilder();
        Map<String, List<String[]>> sections = new LinkedHashMap<>();
        String currentSection = "General";

        // Parse and group data
        String[] lines = mediaInfoOutput.split("\n");
        for (String line : lines) {
            line = line.trim();
            if (line.isEmpty()) continue;

            if (!line.contains(" : ") && !line.startsWith(" ")) {
                currentSection = line;
                sections.putIfAbsent(currentSection, new ArrayList<>());
            } else if (line.contains(" : ")) {
                String[] parts = line.split(" : ", 2);
                if (parts.length == 2) {
                    sections.computeIfAbsent(currentSection, k -> new ArrayList<>())
                            .add(new String[]{parts[0].trim(), parts[1].trim()});
                }
            }
        }

        // Format output
        for (Map.Entry<String, List<String[]>> entry : sections.entrySet()) {
            result.append("\n╔══════════════════════════════════════════════════════╗\n");
            result.append("║ ").append(String.format("%-50s", entry.getKey())).append(" ║\n");
            result.append("╠══════════════════════════════════════════════════════╣\n");

            for (String[] pair : entry.getValue()) {
                String param = formatParam(pair[0]);
                String value = pair[1];
                result.append("║ ").append(String.format("%-25s", param))
                        .append(" : ").append(String.format("%-23s", value))
                        .append(" ║\n");
            }

            result.append("╚══════════════════════════════════════════════════════╝\n");
        }

        return result.toString();
    }

    private static String formatParam(String param) {
        // Clean up parameter names
        Map<String, String> paramMap = Map.ofEntries(
                Map.entry("CompleteName", "File Name"),
                Map.entry("FileSize/String", "File Size"),
                Map.entry("Duration/String", "Duration"),
                Map.entry("OverallBitRate/String", "Bitrate"),
                Map.entry("FrameRate/String", "Frame Rate"),
                Map.entry("StreamSize/String", "Stream Size"),
                Map.entry("BitRate/String", "Bitrate"),
                Map.entry("DisplayAspectRatio/String", "Aspect Ratio"),
                Map.entry("Channel(s)/String", "Channels"),
                Map.entry("SamplingRate/String", "Sample Rate")
        );

        return paramMap.getOrDefault(param, param.replace("_", " "));
    }
}