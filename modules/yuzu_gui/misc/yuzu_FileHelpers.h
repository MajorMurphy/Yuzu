#pragma once

namespace yuzu
{
    class FileHelpers
    {
    public:
        static int64_t recursiveDirectorySize(juce::File dir);
        static juce::File getTempFolder();
        static juce::File getDataFolder();
    };
}
