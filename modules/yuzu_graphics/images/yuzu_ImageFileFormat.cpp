#include "yuzu_ImageFileFormat.h"

namespace yuzu
{
    yuzu::ExtendedImageFileFormat::ExtendedImageFileFormat(juce::InputStream& is)
    {
        is.readIntoMemoryBlock(rawFileData, -1);
    }
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
        if (JPEGImageExtendedFormat::canUnderstand(input))
        {
            input.setPosition(streamPos);
            fmt.reset(new JPEGImageExtendedFormat(input));
            return fmt;
        }

        input.setPosition(streamPos);
        if (PNGImageExtendedFormat::canUnderstand(input))
        {
            input.setPosition(streamPos);
            fmt.reset(new PNGImageExtendedFormat(input));
            return fmt;
        }

        input.setPosition(streamPos);
        if (GIFImageExtendedFormat::canUnderstand(input))
        {
            input.setPosition(streamPos);
            fmt.reset(new GIFImageExtendedFormat(input));
            return fmt;
        }

        input.setPosition(streamPos);
        if (BMPImageExtendedFormat::canUnderstand(input))
        {
            input.setPosition(streamPos);
            fmt.reset(new BMPImageExtendedFormat(input));
            return fmt;
        }

        input.setPosition(streamPos);
        if (WEBPImageExtendedFormat::canUnderstand(input))
        {
            input.setPosition(streamPos);
            fmt.reset(new WEBPImageExtendedFormat(input));
            return fmt;
        }

       
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