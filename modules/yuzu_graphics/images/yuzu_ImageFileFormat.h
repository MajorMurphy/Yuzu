#pragma once
#if YUZU_LINK_LIBHEIF
#include <libheif/heif.h>
#endif

namespace yuzu
{

    class ExtendedImageFileFormat
    {
    public: 
        ExtendedImageFileFormat(juce::InputStream&);
        virtual ~ExtendedImageFileFormat() = default;
        static std::unique_ptr<ExtendedImageFileFormat> findImageFormatForStream(juce::InputStream& input);
        static std::unique_ptr<ExtendedImageFileFormat> findImageFormatForFile(juce::File input);

        virtual juce::Image decodeImage() = 0;
        virtual bool loadMetadataFromImage(juce::OwnedArray<gin::ImageMetadata>& metadata) = 0;
    protected:
        juce::MemoryBlock rawFileData;
    };

    class PNGImageExtendedFormat : public yuzu::ExtendedImageFileFormat
    {
    public:
        //==============================================================================
        PNGImageExtendedFormat(juce::InputStream& image);

        ~PNGImageExtendedFormat() override;
        //==============================================================================
        static juce::String getFormatName();
        static bool usesFileExtension(const juce::File&);
        static bool canUnderstand(juce::InputStream&);

        juce::Image decodeImage() override;
        bool loadMetadataFromImage(juce::OwnedArray<gin::ImageMetadata>& metadata) override;

    private:
        juce::PNGImageFormat fmt;

    };

    class WEBPImageExtendedFormat : public yuzu::ExtendedImageFileFormat
    {
    public:
        //==============================================================================
        WEBPImageExtendedFormat(juce::InputStream& image);

        ~WEBPImageExtendedFormat() override;
        //==============================================================================
        static juce::String getFormatName();
        static bool usesFileExtension(const juce::File&);
        static bool canUnderstand(juce::InputStream&);

        juce::Image decodeImage() override;
        bool loadMetadataFromImage(juce::OwnedArray<gin::ImageMetadata>& metadata) override;

    private:
        gin::WEBPImageFormat fmt;

    };

    class BMPImageExtendedFormat : public yuzu::ExtendedImageFileFormat
    {
    public:
        //==============================================================================
        BMPImageExtendedFormat(juce::InputStream& image);

        ~BMPImageExtendedFormat() override;
        //==============================================================================
        static juce::String getFormatName();
        static bool usesFileExtension(const juce::File&);
        static bool canUnderstand(juce::InputStream&);

        juce::Image decodeImage() override;
        bool loadMetadataFromImage(juce::OwnedArray<gin::ImageMetadata>& metadata) override;

    private:
        gin::BMPImageFormat fmt;

    };

    class GIFImageExtendedFormat : public yuzu::ExtendedImageFileFormat
    {
    public:
        //==============================================================================
        GIFImageExtendedFormat(juce::InputStream& image);

        ~GIFImageExtendedFormat() override;
        //==============================================================================
        static juce::String getFormatName();
        static bool usesFileExtension(const juce::File&);
        static bool canUnderstand(juce::InputStream&);

        juce::Image decodeImage() override;
        bool loadMetadataFromImage(juce::OwnedArray<gin::ImageMetadata>& metadata) override;

    private:
        juce::GIFImageFormat fmt;

    };

    class JPEGImageExtendedFormat : public yuzu::ExtendedImageFileFormat
    {
    public:
        //==============================================================================
        JPEGImageExtendedFormat(juce::InputStream& image);

        ~JPEGImageExtendedFormat() override;
        //==============================================================================
        static juce::String getFormatName();
        static bool usesFileExtension(const juce::File&);
        static bool canUnderstand(juce::InputStream&);

        juce::Image decodeImage() override;
        bool loadMetadataFromImage(juce::OwnedArray<gin::ImageMetadata>& metadata) override;

    private:
        juce::JPEGImageFormat fmt;

    };

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

#if YUZU_LINK_LIBHEIF
        heif_context* ctx = nullptr;
        heif_image_handle* primaryImageHandle = nullptr;
#endif

    };

    // wrapper for compatibility with juce::ImageFileFormat
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