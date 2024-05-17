// Copyright (c) 2024 Major Murphy

#include "../images/yuzu_ImageFileFormat.h"

#if YUZU_LINK_LIBULTRAHDR
#include <ultrahdr_api.h>
#endif

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
#if YUZU_LINK_LIBULTRAHDR
	return (is_uhdr_image(rawFileData.getData(), rawFileData.getSize()));
#else
	loadMetadata();

	if (!xmpMetadata)
		return false;
	auto rdf = xmpMetadata->getChildByName("rdf:RDF");
	if (!rdf)
		return false;
	auto desc = rdf->getChildByAttribute("xmlns:hdrgm", "http://ns.adobe.com/hdr-gain-map/1.0/");

	return desc != nullptr;
#endif
}

yuzu::ExtendedImage yuzu::JPEGImageExtendedFormat::decodeHDRImage()
{
	

#if YUZU_LINK_LIBULTRAHDR
	if (!containsUltraHDR())
		return yuzu::ExtendedImage();

	uhdr_compressed_image_t compressed{};

	compressed.capacity = rawFileData.getSize();
	compressed.data_sz = compressed.capacity;
	compressed.data = rawFileData.getData();
	compressed.cg = UHDR_CG_UNSPECIFIED;
	compressed.ct = UHDR_CT_UNSPECIFIED;
	compressed.range = UHDR_CR_UNSPECIFIED;

	auto decoder = uhdr_create_decoder();
	if (!decoder)
	{
		jassertfalse;
		return yuzu::ExtendedImage();
	}
	auto status = uhdr_dec_set_image(decoder, &compressed);
	if (status.error_code != UHDR_CODEC_OK)
	{
		jassertfalse;
		if (status.has_detail)
			DBG(status.detail);

		uhdr_release_decoder(decoder);
		return yuzu::ExtendedImage();
	}

	auto colorTransfer = uhdr_color_transfer::UHDR_CT_LINEAR;
	status = uhdr_dec_set_out_color_transfer(decoder, colorTransfer);
	if (status.error_code != UHDR_CODEC_OK)
	{
		jassertfalse;
		if (status.has_detail)
			DBG(status.detail);

		uhdr_release_decoder(decoder);
		return yuzu::ExtendedImage();
	}

	auto pixelFormat = uhdr_img_fmt_t::UHDR_IMG_FMT_64bppRGBAHalfFloat;
	status = uhdr_dec_set_out_img_format(decoder, pixelFormat);
	if (status.error_code != UHDR_CODEC_OK)
	{
		jassertfalse;
		if (status.has_detail)
			DBG(status.detail);

		uhdr_release_decoder(decoder);
		return yuzu::ExtendedImage();
	}

	status = uhdr_decode(decoder);
	if (status.error_code != UHDR_CODEC_OK)
	{
		jassertfalse;
		if (status.has_detail)
			DBG(status.detail);

		uhdr_release_decoder(decoder);
		return yuzu::ExtendedImage();
	}
	
	auto output = uhdr_get_decoded_image(decoder);
	if (!output)
	{
		jassertfalse;
		uhdr_release_decoder(decoder);
		return yuzu::ExtendedImage();
	}
	int bytespp = (output->fmt == UHDR_IMG_FMT_64bppRGBAHalfFloat) ? 8 : 4;
	juce::MemoryBlock data(output->w * output->h * bytespp);
	data.copyFrom(output->planes[0], 0, data.getSize());

	auto embeddedProfile = uhdr_dec_get_icc(decoder);

	juce::MemoryBlock icc;
	if (embeddedProfile)
	{
		icc.setSize(embeddedProfile->data_sz);
		icc.copyFrom(embeddedProfile->data, 0, embeddedProfile->data_sz);
	}

	yuzu::ExtendedImage hdr(
		ExtendedImage::PixelFormat::RGBAHalfFloat64,
		output->w,
		output->h,
		bytespp * 8,
		output->stride[0]*bytespp,
		data,
		icc
		);
	
	uhdr_release_decoder(decoder);
	return hdr;

#else	
	yuzu::ExtendedImage();
#endif
}
