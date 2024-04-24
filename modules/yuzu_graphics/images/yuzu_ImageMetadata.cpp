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

            yuzu::HEIFImageExtendedFormat fmt(is);
            return fmt.loadMetadataFromImage(metadata);
        }

        is.setPosition(0);
        if (yuzu::JPEGImageExtendedFormat::canUnderstand(is))
        {
            is.setPosition(0);

            yuzu::JPEGImageExtendedFormat fmt(is);
            return fmt.loadMetadataFromImage(metadata);
        }

        return false;
    }

}