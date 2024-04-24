#pragma once
#if YUZU_LINK_LIBHEIF
#include <libheif/heif.h>
#endif
namespace yuzu
{

    class ExtendedImageFileFormat
    {
    public: 
        ExtendedImageFileFormat() = default;
        virtual ~ExtendedImageFileFormat() = default;
        static std::unique_ptr<ExtendedImageFileFormat> findImageFormatForStream(juce::InputStream& input);
        static std::unique_ptr<ExtendedImageFileFormat> findImageFormatForFile(juce::File input);

        virtual juce::Image decodeImage() = 0;
        virtual bool loadMetadataFromImage(juce::OwnedArray<gin::ImageMetadata>& metadata) = 0;

    };

    //==============================================================================
    /**
    A subclass of ImageFileFormat for reading and writing HEICfiles.

    @see ImageFileFormat, PNGImageFormat

    @tags{Graphics}
    */
    class HEIFImageExtendedFormat : public yuzu::ExtendedImageFileFormat
    {
    public:
        //==============================================================================
        HEIFImageExtendedFormat(juce::InputStream& image);

        ~HEIFImageExtendedFormat() override;
        //==============================================================================
        static juce::String getFormatName();
        static bool usesFileExtension(const juce::File&);
        static bool canUnderstand(juce::InputStream&);

        juce::Image decodeImage() override;        
        bool loadMetadataFromImage(juce::OwnedArray<gin::ImageMetadata>& metadata) override;

    private:
        juce::MemoryBlock rawFileData;
#if YUZU_LINK_LIBHEIF
        heif_context* ctx = nullptr;
        heif_image_handle* primaryImageHandle = nullptr;
#endif

    };

    class HEIFImageFormat : public juce::ImageFileFormat
    {
    public:
        HEIFImageFormat();
        ~HEIFImageFormat() override;

        juce::String getFormatName() override;
        bool usesFileExtension(const juce::File&) override;
        bool canUnderstand(juce::InputStream&)  override;
        juce::Image decodeImage(juce::InputStream&) override;
        bool writeImageToStream(const juce::Image& sourceImage, juce::OutputStream& destStream) override;
    };

}