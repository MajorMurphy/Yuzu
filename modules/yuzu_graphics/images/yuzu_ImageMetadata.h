#pragma once

#include <JuceHeader.h>

namespace yuzu
{

    class ImageMetadata
    {
    public: 
        static bool getFromImage(juce::InputStream& is, juce::OwnedArray<gin::ImageMetadata>& metadata);
    };


}