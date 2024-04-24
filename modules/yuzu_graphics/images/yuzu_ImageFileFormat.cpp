#include "yuzu_ImageFileFormat.h"

namespace yuzu
{
    std::unique_ptr<ExtendedImageFileFormat> yuzu::ExtendedImageFileFormat::findImageFormatForStream(juce::InputStream& input)
    {
        std::unique_ptr<ExtendedImageFileFormat> fmt;
        const auto streamPos = input.getPosition();
        if (HEIFImageExtendedFormat::canUnderstand(input))
        {
            input.setPosition(streamPos);
            fmt.reset(new HEIFImageExtendedFormat(input));
            return fmt;
        }
        
        input.setPosition(streamPos);
        // format not identified
        jassertfalse;
        return fmt;
    }

    std::unique_ptr<ExtendedImageFileFormat> ExtendedImageFileFormat::findImageFormatForFile(juce::File input)
    {
        juce::FileInputStream stream(input);

        if (stream.openedOk())
        {
            juce::BufferedInputStream b(stream, 8192);
            return findImageFormatForStream(b);
        }
        return nullptr;
    }


}