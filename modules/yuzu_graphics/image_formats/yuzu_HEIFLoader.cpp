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
	heif_context_read_from_memory_without_copy(ctx, rawFileData.getData(), rawFileData.getSize(), nullptr);
	heif_context_get_primary_image_handle(ctx, &primaryImageHandle);
	if (!primaryImageHandle)
	{
		jassertfalse;
		return;
	}
#endif
}

yuzu::HEIFImageExtendedFormat::~HEIFImageExtendedFormat()
{
	// clean up resources
	if(primaryImageHandle)
		heif_image_handle_release(primaryImageHandle);
	if(ctx)
		heif_context_free(ctx);
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
#if YUZU_LINK_LIBHEIF || JUCE_USING_COREIMAGE_LOADER
	in.readByte();
	in.readByte();
	in.readByte();
	in.readByte();
	in.readByte();
	uint32 ftpy = in.readInt();
	uint32 heic = in.readInt();
	return heic == 6515045 && ftpy == 1752201588; 
#else
	jassertfalse;
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

#if JUCE_USING_COREIMAGE_LOADER
Image juce_loadWithCoreImage(InputStream&);
#endif

juce::Image yuzu::HEIFImageExtendedFormat::decodeImage()
{
#if JUCE_USING_COREIMAGE_LOADER
	return juce_loadWithCoreImage(in);
#elif YUZU_LINK_LIBHEIF
	if (!primaryImageHandle)
	{
		jassertfalse;
		return Image();
	}

	auto width = heif_image_handle_get_width(primaryImageHandle);
	auto height = heif_image_handle_get_height(primaryImageHandle);
	auto hasAlpha = heif_image_handle_has_alpha_channel(primaryImageHandle);

	if (width <= 0 || height <= 0)
	{
		jassertfalse;
		return Image();
	}

	// decode the image and convert colorspace to RGB, saved as 24bit interleaved
	heif_image* encodedImage = nullptr;
	heif_decode_image(primaryImageHandle, &encodedImage, heif_colorspace_RGB, hasAlpha ? heif_chroma_interleaved_RGBA : heif_chroma_interleaved_RGB, nullptr);
	if (!encodedImage)
	{
		jassertfalse;
		return Image();
	}

	int stride = 0;
	const uint8_t* data = heif_image_get_plane_readonly(encodedImage, heif_channel_interleaved, &stride);
	if (stride <= 0)
	{
		jassertfalse;
		return Image();
	}

	auto decodedImage = Image(hasAlpha ? Image::ARGB : Image::RGB, width, height, false);
	Image::BitmapData bmp(decodedImage, Image::BitmapData::writeOnly);
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


bool yuzu::HEIFImageExtendedFormat::loadMetadataFromImage(juce::OwnedArray<gin::ImageMetadata>& metadata)
{
#if YUZU_LINK_LIBHEIF

	heif_item_id exif_id;

	int n = heif_image_handle_get_list_of_metadata_block_IDs(primaryImageHandle, "Exif", &exif_id, 1);
	if (n == 1) {
		size_t exifSize = heif_image_handle_get_metadata_size(primaryImageHandle, exif_id);
		uint8_t* exifData = (uint8_t*)malloc(exifSize);
		struct heif_error error = heif_image_handle_get_metadata(primaryImageHandle, exif_id, exifData);

		auto md = gin::ExifMetadata::create(exifData + 4, (int)exifSize - 4);
		if (md)
			metadata.add(md);
		free(exifData);
	}

	return metadata.size() > 0;
#else
	jassertfalse;
	return false;
#endif
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

bool yuzu::HEIFImageFormat::writeImageToStream(const Image&, OutputStream&)
{
	// not yet implemented
	jassertfalse;
	return false;
}


JUCE_END_IGNORE_WARNINGS_MSVC
