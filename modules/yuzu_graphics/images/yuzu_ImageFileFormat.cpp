// Copyright (c) 2024 Major Murphy

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

    juce::Image yuzu::ExtendedImageFileFormat::decodeThumbnail()
    {
        if (loadMetadata())
        {
            return exif->getThumbnailImage();
        }

        return juce::Image();
    }

    juce::StringPairArray ExtendedImageFileFormat::getMetadata()
    {        
        if (loadMetadata())
            return exif->getAllMetadata();
        else return juce::StringPairArray();
    }

    bool ExtendedImageFileFormat::extractVideo(juce::OutputStream& os)
    {
        if (!hasCheckedForMotionPhoto)
            getMotionPhotoSize();

        if (videoSize <= 0)
        {
            return false;
        }
        if (videoPosition < 0 || videoPosition + videoSize > rawFileData.getSize())
        {
            jassertfalse;
            return false;
        }

        juce::MemoryInputStream dataStream(rawFileData,false);
        dataStream.setPosition(videoPosition);
        
        if (!os.writeFromInputStream(dataStream, videoSize))
        {
            DBG("Failed to write video data to output stream");
            jassertfalse;
            return false;
        }
        return true;
    }

    ExtendedImageFileFormat::Orientation ExtendedImageFileFormat::getOriginalOrientation()
    {
        if (loadMetadata())
        {
            auto value = getMetadata().getValue("Orientation", "");
            if (value.isNotEmpty())
            {
                if (value.equalsIgnoreCase("1") || value.equalsIgnoreCase("0"))
                    return Orientation::portrait;
                else if (value.equalsIgnoreCase("6"))
                    return Orientation::landscape90;
                else if (value.equalsIgnoreCase("8"))
                    return Orientation::landscape270;
                else if (value.equalsIgnoreCase("3"))
                    return Orientation::inverted;
                else
                    jassertfalse;
            }
        }

        return Orientation::portrait;
    }

    bool ExtendedImageFileFormat::loadMetadata()
    {
        if (hasCheckedForMetadata)
            return exif != nullptr;

        juce::OwnedArray<gin::ImageMetadata> allMetadata;

        juce::MemoryInputStream s(rawFileData, false);
        if (gin::ImageMetadata::getFromImage(s, allMetadata))
        {
            for (auto md : allMetadata)
            {
                if (md->getType().equalsIgnoreCase("exif"))
                {
                    allMetadata.remove(allMetadata.indexOf(md), false);
                    exif.reset((gin::ExifMetadata*)md);
                    hasCheckedForMetadata = true;
                    return true;
                }
            }
        }

        hasCheckedForMetadata = true;
        return false;
    }


}