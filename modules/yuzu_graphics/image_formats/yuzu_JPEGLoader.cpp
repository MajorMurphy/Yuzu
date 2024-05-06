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

	auto orientation = getOriginalOrientation();
	float rotation = 0;
	float translatedYscale = 1.0;
	float translatedXscale = 1.0;
	int rotatedWidth = 0, rotateHeight = 0;

	switch (orientation)
	{
		case Orientation::portrait :
			return img;
		case Orientation::landscape270 :
			rotation = MathConstants<float>::pi / -2.0;
			translatedYscale = 1;
			translatedXscale = 0;
			rotatedWidth = img.getHeight();
			rotateHeight = img.getWidth();
			break;
		case Orientation::landscape90 :
			rotation = MathConstants<float>::pi / 2.0;
			translatedYscale = 0;
			translatedXscale = 1;
			rotatedWidth = img.getHeight();
			rotateHeight = img.getWidth();
			break;
		case Orientation::inverted :
			rotation = MathConstants<float>::pi;
			translatedYscale = 1;
			translatedXscale = 1;
			rotatedWidth = img.getWidth();
			rotateHeight = img.getHeight();
			break;
		default:
			jassertfalse;
			return img;
	}

	Image rotated(img.getFormat(), rotatedWidth, rotateHeight, false);
	Graphics g(rotated);
	AffineTransform t;
	t = t.rotated(rotation);
	t = t.translated((float)rotated.getWidth() * translatedXscale, (float)rotated.getHeight() * translatedYscale);
	g.setOpacity(1.0f);
	g.drawImageTransformed(img, t);

	return rotated;

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
