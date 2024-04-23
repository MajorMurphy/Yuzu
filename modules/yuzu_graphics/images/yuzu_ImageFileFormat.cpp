#include "yuzu_ImageFileFormat.h"

namespace yuzu
{
    struct DefaultImageFormats
    {
        static juce::ImageFileFormat** get()
        {
            static DefaultImageFormats formats;
            return formats.formats;
        }

    private:
        DefaultImageFormats() noexcept
        {
            formats[0] = &png;
            formats[1] = &jpg;
            formats[2] = &gif;
            formats[3] = &heif;
            formats[4] = nullptr;
        }

        juce::PNGImageFormat  png;
        juce::JPEGImageFormat jpg;
        juce::GIFImageFormat  gif;
        yuzu::HEIFImageFormat heif;

        juce::ImageFileFormat* formats[5];
    };


    juce::ImageFileFormat* yuzu::ImageFileFormat::findImageFormatForStream(juce::InputStream& input)
    {
        const juce::int64 streamPos = input.getPosition();

        for (juce::ImageFileFormat** i = yuzu::DefaultImageFormats::get(); *i != nullptr; ++i)
        {
            const bool found = (*i)->canUnderstand(input);
            input.setPosition(streamPos);

            if (found)
                return *i;
        }

        return nullptr;
    }

    juce::ImageFileFormat* yuzu::ImageFileFormat::findImageFormatForFileExtension(const juce::File& file)
    {
        for (juce::ImageFileFormat** i = yuzu::DefaultImageFormats::get(); *i != nullptr; ++i)
            if ((*i)->usesFileExtension(file))
                return *i;

        return nullptr;
    }

    //==============================================================================
    juce::Image yuzu::ImageFileFormat::loadFrom(juce::InputStream& input)
    {
        if (juce::ImageFileFormat* format = findImageFormatForStream(input))
            return format->decodeImage(input);

        return juce::Image();
    }

    juce::Image yuzu::ImageFileFormat::loadFrom(const juce::File& file)
    {
        juce::FileInputStream stream(file);

        if (stream.openedOk())
        {
            juce::BufferedInputStream b(stream, 8192);
            return loadFrom(b);
        }

        return juce::Image();
    }

    juce::Image yuzu::ImageFileFormat::loadFrom(const void* rawData, const size_t numBytes)
    {
        if (rawData != nullptr && numBytes > 4)
        {
            juce::MemoryInputStream stream(rawData, numBytes, false);
            return loadFrom(stream);
        }

        return juce::Image();
    }
}