// Copyright (c) 2024 Major Murphy

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
	auto img = fmt.decodeImage(s);
	if (!img.isValid())
		return img;

	return rotate(img, getOriginalOrientation());
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

bool yuzu::JPEGImageExtendedFormat::containsUltraHDR()
{
	if (!xmpMetadata)
		return false;
	auto rdf = xmpMetadata->getChildByName("rdf:RDF");
	if (!rdf)
		return false;
	auto desc = rdf->getChildByAttribute("xmlns:hdrgm", "http://ns.adobe.com/hdr-gain-map/1.0/");

	return desc != nullptr;
}
