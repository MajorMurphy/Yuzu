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
            return rotate(exif->getThumbnailImage(), getOriginalOrientation());
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

    juce::Image ExtendedImageFileFormat::rotate(juce::Image img, yuzu::ExtendedImageFileFormat::Orientation orientation)
    {
        if (!img.isValid())
            return juce::Image();

        float rotation = 0;
        float translatedYscale = 1.0;
        float translatedXscale = 1.0;
        int rotatedWidth = 0, rotateHeight = 0;

        switch (orientation)
        {
        case Orientation::portrait:
            return img;
        case Orientation::landscape270:
            rotation = juce::MathConstants<float>::pi / -2.0;
            translatedYscale = 1;
            translatedXscale = 0;
            rotatedWidth = img.getHeight();
            rotateHeight = img.getWidth();
            break;
        case Orientation::landscape90:
            rotation = juce::MathConstants<float>::pi / 2.0;
            translatedYscale = 0;
            translatedXscale = 1;
            rotatedWidth = img.getHeight();
            rotateHeight = img.getWidth();
            break;
        case Orientation::inverted:
            rotation = juce::MathConstants<float>::pi;
            translatedYscale = 1;
            translatedXscale = 1;
            rotatedWidth = img.getWidth();
            rotateHeight = img.getHeight();
            break;
        default:
            jassertfalse;
            return img;
        }

        juce::Image rotated(img.getFormat(), rotatedWidth, rotateHeight, false);
        juce::Graphics g(rotated);
        juce::AffineTransform t;
        t = t.rotated(rotation);
        t = t.translated((float)rotated.getWidth() * translatedXscale, (float)rotated.getHeight() * translatedYscale);
        g.setOpacity(1.0f);
        g.drawImageTransformed(img, t);

        return rotated;
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