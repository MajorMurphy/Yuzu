// Copyright (c) 2024 Major Murphy

#include "../images/yuzu_ImageFileFormat.h"



JUCE_BEGIN_IGNORE_WARNINGS_MSVC(4100)


using namespace yuzu;
using namespace juce;


yuzu::HEIFImageExtendedFormat::HEIFImageExtendedFormat(InputStream& is) : ExtendedImageFileFormat(is)
{	
	is.readIntoMemoryBlock(rawFileData, -1);
#if YUZU_LINK_LIBHEIF

	ctx = heif_context_alloc();
	if (!ctx)
	{
		jassertfalse;
		return;
	}
	auto error = heif_context_read_from_memory_without_copy(ctx, rawFileData.getData(), rawFileData.getSize(), nullptr);
	if (error.code != heif_error_code::heif_error_Ok)
	{
		DBG("Heif read error: " + String(error.message));
		jassertfalse;
		return;
	}
	error = heif_context_get_primary_image_handle(ctx, &primaryImageHandle);
	if (error.code != heif_error_code::heif_error_Ok)
	{
		DBG("Heif read error: " + String(error.message));
		jassertfalse;
		return;
	}
	if (!primaryImageHandle)
	{
		jassertfalse;
		return;
	}
#endif
}

yuzu::HEIFImageExtendedFormat::~HEIFImageExtendedFormat()
{
#if YUZU_LINK_LIBHEIF
	// clean up resources
	if(primaryImageHandle)
		heif_image_handle_release(primaryImageHandle);
	if(primaryThumbHandle)
		heif_image_handle_release(primaryThumbHandle);
	if(ctx)
		heif_context_free(ctx);
#endif
}

juce::String yuzu::HEIFImageExtendedFormat::getFormatName()
{
	return "High Efficiency Image File Format (HEIF)";
}

bool yuzu::HEIFImageExtendedFormat::usesFileExtension(const File& file)
{
	return file.hasFileExtension("heif;heic");
}

bool yuzu::HEIFImageExtendedFormat::canUnderstand([[maybe_unused]] InputStream& in)
{
#if YUZU_LINK_LIBHEIF
	in.readByte();
	in.readByte();
	in.readByte();
	in.readByte();
	in.readByte();
	uint32 ftpy = in.readInt();
	uint32 heic = in.readInt();
	return heic == 6515045 && ftpy == 1752201588; 
#else
	return false;
#endif
}

inline void ABGRtoARGB(juce::uint32* x)
{
	// Source: 0xAABBGGRR
	*x = (*x & 0xFF00FF00) |
		((*x & 0x00FF0000) >> 16) | 
		((*x & 0x000000FF) << 16); 
	// Return:  0xAARRGGBB
}


juce::Image yuzu::HEIFImageExtendedFormat::decodeImage()
{
#if YUZU_LINK_LIBHEIF
	if (!primaryImageHandle)
	{
		jassertfalse;
		return juce::Image();
	}

	auto width = heif_image_handle_get_width(primaryImageHandle);
	auto height = heif_image_handle_get_height(primaryImageHandle);
	auto hasAlpha = heif_image_handle_has_alpha_channel(primaryImageHandle);
#if JUCE_MAC
    hasAlpha = true;
#endif
    
    
	if (width <= 0 || height <= 0)
	{
		jassertfalse;
		return juce::Image();
	}

	// decode the image and convert colorspace to RGB, saved as 24bit interleaved
	heif_image* encodedImage = nullptr;

	auto error = heif_decode_image(primaryImageHandle, 
                                   &encodedImage,
                                   heif_colorspace_RGB,
                                   hasAlpha ? heif_chroma_interleaved_RGBA : heif_chroma_interleaved_RGB,
                                   nullptr);

    
    if (!encodedImage || error.code != heif_error_Ok)
	{
        DBG("libheif decode error: " + String(error.message));
		jassertfalse;
		return juce::Image();
	}

	int stride = 0;
	const uint8_t* data = heif_image_get_plane_readonly(encodedImage, heif_channel_interleaved, &stride);
	if (stride <= 0)
	{
		jassertfalse;
		return juce::Image();
	}

	auto decodedImage = juce::Image(hasAlpha ? juce::Image::ARGB : juce::Image::RGB, width, height, false);
	juce::Image::BitmapData bmp(decodedImage, juce::Image::BitmapData::writeOnly);
	for (int y = 0; y < height; y++)
	{
		auto linePtr = data + (y * stride);
		if (hasAlpha)
		{
			if (bmp.lineStride == stride)
			{
				memcpy(bmp.getLinePointer(y), linePtr, stride);
				for (int x = 0; x < width; x++)
				{
					ABGRtoARGB((uint32*)bmp.getPixelPointer(x, y));
				}
			}
			else jassertfalse;
		}
		else
		{
			for (int x = 0; x < width; x++)
			{
				auto dest = bmp.getPixelPointer(x, y);
				auto src = linePtr + x * 3;
				dest[0] = src[2];
				dest[1] = src[1];
				dest[2] = src[0];
			}
		}
	}

	// clean up resources
	heif_image_release(encodedImage);

	return decodedImage;
#else 
	jassertfalse;
	return Image();
#endif

}

juce::Image yuzu::HEIFImageExtendedFormat::decodeThumbnail()
{
	juce::Image decodedImage;

#if YUZU_LINK_LIBHEIF
	if (!primaryImageHandle || !ctx)
		return decodedImage;

	if (!primaryThumbHandle)
	{
		heif_item_id thumbnail_ID;
		int nThumbnails = heif_image_handle_get_list_of_thumbnail_IDs(primaryImageHandle, &thumbnail_ID, 1);
		if (nThumbnails > 0) {
			struct heif_image_handle* thumbnail_handle;
			auto err = heif_image_handle_get_thumbnail(primaryImageHandle, thumbnail_ID, &thumbnail_handle);
			if (err.code)
			{
				DBG("Could not read HEIF thumbnail : " << err.message << "\n");
				return juce::Image();
			}
			primaryThumbHandle = thumbnail_handle;
		}

	}
	if (!primaryThumbHandle)
	{
		return decodedImage;
	}

	auto width = heif_image_handle_get_width(primaryThumbHandle);
	auto height = heif_image_handle_get_height(primaryThumbHandle);
	auto hasAlpha = heif_image_handle_has_alpha_channel(primaryThumbHandle);

	// decode the image and convert colorspace to RGB, saved as 24bit interleaved
	heif_image* encodedImage = nullptr;
	auto error = heif_decode_image(primaryThumbHandle, &encodedImage, heif_colorspace_RGB, hasAlpha ? heif_chroma_interleaved_RGBA : heif_chroma_interleaved_RGB, nullptr);
    if (!encodedImage || error.code != heif_error_Ok)
    {
        DBG("libheif decode error: " + String(error.message));
        jassertfalse;
        return juce::Image();
    }

	int stride = 0;
	const uint8_t* decodedData = heif_image_get_plane_readonly(encodedImage, heif_channel_interleaved, &stride);
	if (stride <= 0)
	{
		jassertfalse;
		return juce::Image();
	}

	decodedImage = juce::Image(hasAlpha ? juce::Image::ARGB : juce::Image::RGB, width, height, false);
	juce::Image::BitmapData bmp(decodedImage, juce::Image::BitmapData::writeOnly);
	for (int y = 0; y < height; y++)
	{
		auto linePtr = decodedData + (y * stride);
		if (hasAlpha)
		{
			if (bmp.lineStride == stride)
			{
				memcpy(bmp.getLinePointer(y), linePtr, stride);
				for (int x = 0; x < width; x++)
				{
					ABGRtoARGB((uint32*)bmp.getPixelPointer(x, y));
				}
			}
			else jassertfalse;
		}
		else
		{
			for (int x = 0; x < width; x++)
			{
				auto dest = bmp.getPixelPointer(x, y);
				auto src = linePtr + x * 3;
				dest[0] = src[2];
				dest[1] = src[1];
				dest[2] = src[0];
			}
		}
	}

	// clean up resources
	heif_image_release(encodedImage);

#endif
	return decodedImage;
}


bool yuzu::HEIFImageExtendedFormat::loadMetadata()
{

	if (hasCheckedForMetadata)
		return exifMetadata == nullptr;

#if YUZU_LINK_LIBHEIF

	heif_item_id exif_id;

	int n = heif_image_handle_get_list_of_metadata_block_IDs(primaryImageHandle, "Exif", &exif_id, 1);
	if (n == 1) {
		size_t exifSize = heif_image_handle_get_metadata_size(primaryImageHandle, exif_id);
		uint8_t* exifData = (uint8_t*)malloc(exifSize);
        if(!exifData)
        {
            DBG("memory allocation error");
            jassertfalse;
            return false;
        }
		auto error = heif_image_handle_get_metadata(primaryImageHandle, exif_id, exifData);
        if (error.code != heif_error_Ok)
        {
            DBG("libheif getmetadata error: " + String(error.message));
            jassertfalse;
        }
        else
        {
            exifMetadata.reset(gin::ExifMetadata::create(exifData + 4, (int)exifSize - 4));         
        }
        free(exifData);
	}

	hasCheckedForMetadata = true;
	return exifMetadata != nullptr;
#else
	jassertfalse;
	hasCheckedForMetadata = true;
	return false;
#endif
}

juce::uint32 yuzu::HEIFImageExtendedFormat::getMotionPhotoSize()
{
	if (hasCheckedForMotionPhoto)
		return videoSize;

	videoPosition = 0;
	videoSize = 0;
	uint32 position = 0;

	const int signatureSize = sizeof(SAMSUNG_MOTIONPHOTO_SIGNATURE_V2);
	char candidate[signatureSize] = { 0 };
	for (auto i = (int)rawFileData.getSize() - signatureSize-1; i > 0; i--)
	{
		rawFileData.copyTo(candidate, i, signatureSize);
		if (0 == strcmp(candidate, SAMSUNG_MOTIONPHOTO_SIGNATURE_V2))
		{
			position = i + signatureSize - 1;
			break;
		}
	}
	if (position == 0)
		return videoSize;

	if (position + sizeof(position) + sizeof(videoSize) < rawFileData.getSize())
	{
		rawFileData.copyTo(&videoPosition, position, sizeof(videoPosition));
		rawFileData.copyTo(&videoSize, position + sizeof(videoPosition), sizeof(videoSize));
	}
	videoPosition = ByteOrder::swap(videoPosition);
	videoSize = ByteOrder::swap(videoSize);

	hasCheckedForMotionPhoto = true;
	return videoSize;
}

yuzu::HEIFImageFormat::HEIFImageFormat()
{
}

yuzu::HEIFImageFormat::~HEIFImageFormat()
{
}

juce::String yuzu::HEIFImageFormat::getFormatName()
{
	return HEIFImageExtendedFormat::getFormatName();
}

bool yuzu::HEIFImageFormat::usesFileExtension(const juce::File& file)
{
	return HEIFImageExtendedFormat::usesFileExtension(file);
}

bool yuzu::HEIFImageFormat::canUnderstand(juce::InputStream& is)
{
	return HEIFImageExtendedFormat::canUnderstand(is);
}

juce::Image yuzu::HEIFImageFormat::decodeImage(juce::InputStream& is)
{
	HEIFImageExtendedFormat fmt(is);
	return fmt.decodeImage();
}

bool yuzu::HEIFImageFormat::writeImageToStream(const juce::Image&, OutputStream&)
{
	// not yet implemented
	jassertfalse;
	return false;
}


JUCE_END_IGNORE_WARNINGS_MSVC
