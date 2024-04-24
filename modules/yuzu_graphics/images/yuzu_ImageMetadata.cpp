#include "yuzu_ImageMetadata.h"

#if YUZU_LINK_LIBHEIF
#include <libheif/heif.h>
#endif

using namespace juce;
namespace yuzu
{

    static bool loadHEIFMetadataFromStream(juce::OwnedArray<gin::ImageMetadata>& metadata, juce::InputStream& in)
    {
#if YUZU_LINK_LIBHEIF

        juce::MemoryBlock encodedImageData(in.getNumBytesRemaining());
        in.read(encodedImageData.getData(), encodedImageData.getSize());

        heif_context* ctx = heif_context_alloc();
        heif_context_read_from_memory_without_copy(ctx, encodedImageData.getData(), encodedImageData.getSize(), nullptr);

        // get a handle to the primary image
        heif_image_handle* handle = nullptr;
        heif_context_get_primary_image_handle(ctx, &handle);

        heif_item_id exif_id;

        int n = heif_image_handle_get_list_of_metadata_block_IDs(handle, "Exif", &exif_id, 1);
        if (n == 1) {
            size_t exifSize = heif_image_handle_get_metadata_size(handle, exif_id);
            uint8_t* exifData = (uint8_t*)malloc(exifSize);
            struct heif_error error = heif_image_handle_get_metadata(handle, exif_id, exifData);

            auto md = gin::ExifMetadata::create(exifData + 4, (int)exifSize - 4);
            if (md)
                metadata.add(md);
            free(exifData);
        }


        // clean up resources
        heif_image_handle_release(handle);
        heif_context_free(ctx);

        return metadata.size() > 0;
#else
        jassertfalse;
        return false;
#endif

    }


	bool yuzu::ImageMetadata::getFromImage(juce::InputStream& is, juce::OwnedArray<gin::ImageMetadata>& metadata)
	{
        if (gin::ImageMetadata::getFromImage(is, metadata))
            return true;


        yuzu::HEIFImageFormat heif;

        is.setPosition(0);
        if (heif.canUnderstand(is))
        {
            is.setPosition(0);
            return loadHEIFMetadataFromStream(metadata, is);
        }

        return false;
    }

}