package com.jldevelopers.mediainfo;

public class MediaInfoHtmlFormatter {

    /**
     * Convert MediaInfo output to HTML table
     */
    /*public static String formatAsHtmlTable(String mediaInfoOutput) {
        if (mediaInfoOutput == null || mediaInfoOutput.isEmpty()) {
            return "<html><body><p>No media information available</p></body></html>";
        }

        StringBuilder html = new StringBuilder();
        html.append("""
            <html>
            <head>
                <style>
                    body { font-family: Arial, sans-serif; margin: 20px; }
                    table { border-collapse: collapse; width: 100%; max-width: 800px; }
                    th { background-color: #f2f2f2; text-align: left; padding: 12px; font-size: 16px; }
                    td { padding: 8px 12px; border-bottom: 1px solid #ddd; }
                    .section { background-color: #e8f4f8; font-weight: bold; }
                    .parameter { font-weight: bold; color: #333; width: 40%; }
                    .value { color: #666; width: 60%; }
                </style>
            </head>
            <body>
            """);

        String[] lines = mediaInfoOutput.split("\n");
        boolean tableOpen = false;

        for (String line : lines) {
            line = line.trim();
            if (line.isEmpty()) continue;

            // Section header
            if (!line.contains(" : ") && !line.startsWith(" ")) {
                if (tableOpen) {
                    html.append("</table><br/>\n");
                }
                html.append("<h3>").append(line).append("</h3>\n");
                html.append("""
                    <table>
                    <tr>
                        <th style="width: 40%">Parameter</th>
                        <th style="width: 60%">Value</th>
                    </tr>
                    """);
                tableOpen = true;
                continue;
            }

            // Parameter-value pair
            if (line.contains(" : ")) {
                String[] parts = line.split(" : ", 2);
                if (parts.length == 2) {
                    String parameter = formatParameterName(parts[0].trim());
                    String value = parts[1].trim();

                    html.append("<tr>\n")
                            .append("  <td class=\"parameter\">").append(parameter).append("</td>\n")
                            .append("  <td class=\"value\">").append(value).append("</td>\n")
                            .append("</tr>\n");
                }
            }
        }

        if (tableOpen) {
            html.append("</table>\n");
        }

        html.append("</body>\n</html>");
        return html.toString();
    }

    private static String formatParameterName(String parameter) {
        // Same formatting as Option 1
        return parameter
                .replace("CompleteName", "File Name")
                .replace("FileSize/String", "File Size")
                .replace("Duration/String", "Duration")
                .replace("OverallBitRate/String", "Overall Bitrate")
                .replace("_", " ");
    }*/
}