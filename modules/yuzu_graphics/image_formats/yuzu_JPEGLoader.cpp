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


juce::uint32 yuzu::JPEGImageExtendedFormat::getMotionPhotoSize()
{
	if (hasCheckedForMotionPhoto)
		return videoSize;

	videoSize = 0;
	videoPosition = 0;

	const int signatureSize = sizeof(SAMSUNG_MOTIONPHOTO_SIGNATURE_V1);
	char candidate[signatureSize];
	for (uint32 i = 0; i < rawFileData.getSize() - signatureSize; i++)
	{
		rawFileData.copyTo(candidate, i, signatureSize);
		if (0 == strcmp(candidate, SAMSUNG_MOTIONPHOTO_SIGNATURE_V1))
		{
			videoPosition = i + signatureSize - 1;
			videoSize = (uint32)(rawFileData.getSize() - videoPosition - 1);

			hasCheckedForMotionPhoto = true;
			return videoSize;
		}
		if (0 == strcmp(candidate, GOOGLE_MOTIONPHOTO_SIGNATURE_V2) ||
			0 == strcmp(candidate, GOOGLE_MOTIONPHOTO_SIGNATURE_V1))
		{
			videoPosition = i - 3;
			videoSize = (uint32)(rawFileData.getSize() - videoPosition);

			hasCheckedForMotionPhoto = true;
			return videoSize;
		}
	}

	hasCheckedForMotionPhoto = true;
	return videoSize;
}
