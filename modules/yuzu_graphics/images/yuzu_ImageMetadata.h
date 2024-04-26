// Copyright (c) 2024 Major Murphy

#pragma once

#include <JuceHeader.h>

namespace yuzu
{

    class ImageMetadata
    {
    public: 
        static bool getFromImage(juce::InputStream& is, juce::OwnedArray<gin::ImageMetadata>& metadata);
        static juce::String getAsString(juce::OwnedArray<gin::ImageMetadata>& metadata);
    private:
        JUCE_LEAK_DETECTOR(ImageMetadata)
    };


}