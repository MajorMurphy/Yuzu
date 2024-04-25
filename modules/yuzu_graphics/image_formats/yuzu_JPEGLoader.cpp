#include "../images/yuzu_ImageFileFormat.h"


using namespace yuzu;
using namespace juce;


JPEGImageExtendedFormat::JPEGImageExtendedFormat(juce::InputStream& is) : ExtendedImageFileFormat(is)
{	
}

yuzu::JPEGImageExtendedFormat::~JPEGImageExtendedFormat()
{
}

juce::String yuzu::JPEGImageExtendedFormat::getFormatName()
{
	JPEGImageFormat fmt;
	return fmt.getFormatName();;
}

bool yuzu::JPEGImageExtendedFormat::usesFileExtension(const juce::File& file)
{
	JPEGImageFormat fmt;
	return fmt.usesFileExtension(file);
}

bool yuzu::JPEGImageExtendedFormat::canUnderstand(juce::InputStream& is)
{
	JPEGImageFormat fmt;
	return fmt.canUnderstand(is);
}

juce::Image yuzu::JPEGImageExtendedFormat::decodeImage()
{
	MemoryInputStream s(rawFileData,false);
	return fmt.decodeImage(s);
}

juce::Image yuzu::JPEGImageExtendedFormat::decodeThumbnail()
{
	juce::OwnedArray<gin::ImageMetadata> md;
	loadMetadataFromImage(md);
	for (auto entry : md)
	{
		// find EXIF metadata
		if (entry->getType().equalsIgnoreCase("exif"))
		{
			auto exif = (gin::ExifMetadata*)entry;
			return exif->getThumbnailImage();
		}

	}

	// no thumbnail found in EXIF
	return juce::Image();
}

bool yuzu::JPEGImageExtendedFormat::loadMetadataFromImage(juce::OwnedArray<gin::ImageMetadata>& metadata)
{
	MemoryInputStream s(rawFileData, false);
	return gin::ImageMetadata::getFromImage(s, metadata);
}
