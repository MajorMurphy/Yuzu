// Copyright (c) 2024 Major Murphy

#include "yuzu_ImageMetadata.h"

#if YUZU_LINK_LIBHEIF
#include <libheif/heif.h>
#endif

using namespace juce;
namespace yuzu
{

	bool yuzu::ImageMetadata::getFromImage(juce::InputStream& is, juce::OwnedArray<gin::ImageMetadata>& metadata)
	{
        auto fmt = ExtendedImageFileFormat::findImageFormatForStream(is);
        is.setPosition(0);

       return fmt->loadMetadataFromImage(metadata);
    }

    juce::String ImageMetadata::getAsString(juce::OwnedArray<gin::ImageMetadata>& metadata)
    {
        juce::String mdString;
        for (auto m : metadata)
        {
            auto pairs = m->getAllMetadata();
            auto keys = pairs.getAllKeys();
            auto values = pairs.getAllValues();
            if (keys.size() == values.size())
            {
                for (int i=0;i< keys.size();i++)
                {
                    mdString += keys[i] + ":  " + values[i] + "\n";
                }
            }
        }
        return mdString;
    }

}