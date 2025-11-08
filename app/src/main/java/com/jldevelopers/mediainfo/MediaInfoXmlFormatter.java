package com.jldevelopers.mediainfosource;

public class MediaInfoXmlFormatter {

    /**
     * Convert MediaInfo output to structured XML
     */
    public static String formatAsXml(String mediaInfoOutput) {
        if (mediaInfoOutput == null || mediaInfoOutput.isEmpty()) {
            return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<MediaInfo>\n</MediaInfo>";
        }

        StringBuilder xml = new StringBuilder();
        xml.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        xml.append("<MediaInfo>\n");

        String[] lines = mediaInfoOutput.split("\n");
        String currentSection = "";

        for (String line : lines) {
            line = line.trim();
            if (line.isEmpty()) continue;

            // Section header
            if (!line.contains(" : ") && !line.startsWith(" ")) {
                if (!currentSection.isEmpty()) {
                    xml.append("  </").append(currentSection).append(">\n");
                }
                currentSection = convertToXmlTag(line);
                xml.append("  <").append(currentSection).append(">\n");
                continue;
            }

            // Parameter-value pair
            if (line.contains(" : ")) {
                String[] parts = line.split(" : ", 2);
                if (parts.length == 2) {
                    String param = convertToXmlTag(parts[0].trim());
                    String value = escapeXml(parts[1].trim());
                    xml.append("    <").append(param).append(">")
                            .append(value).append("</").append(param).append(">\n");
                }
            }
        }

        if (!currentSection.isEmpty()) {
            xml.append("  </").append(currentSection).append(">\n");
        }

        xml.append("</MediaInfo>");
        return xml.toString();
    }

    private static String convertToXmlTag(String text) {
        return text.replaceAll("[^a-zA-Z0-9]", "_")
                .replaceAll("_{2,}", "_")
                .replaceAll("^_|_$", "");
    }

    private static String escapeXml(String text) {
        return text.replace("&", "&amp;")
                .replace("<", "&lt;")
                .replace(">", "&gt;")
                .replace("\"", "&quot;")
                .replace("'", "&apos;");
    }
}