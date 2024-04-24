#include "yuzu_ImageMetadata.h"

#if YUZU_LINK_LIBHEIF
#include <libheif/heif.h>
#endif

using namespace juce;
namespace yuzu
{

	bool yuzu::ImageMetadata::getFromImage(juce::InputStream& is, juce::OwnedArray<gin::ImageMetadata>& metadata)
	{
        if (gin::ImageMetadata::getFromImage(is, metadata))
            return true;


        is.setPosition(0);
        if (yuzu::HEIFImageExtendedFormat::canUnderstand(is))
        {
            is.setPosition(0);

            yuzu::HEIFImageExtendedFormat heif(is);
            return heif.loadMetadataFromImage(metadata);
        }

        return false;
    }

}