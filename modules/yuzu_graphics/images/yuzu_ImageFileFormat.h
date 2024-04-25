#pragma once
#if YUZU_LINK_LIBHEIF
#include <libheif/heif.h>
#endif

// Samsung uses this signature in JPEG files to denote start of video file
#define SAMSUNG_MOTIONPHOTO_SIGNATURE_V1 "\x4D\x6F\x74\x69\x6F\x6E\x50\x68\x6F\x74\x6F\x5F\x44\x61\x74\x61" // "MotionPhoto_data"

// Samsungs uses this signatures in HEIC files
#define SAMSUNG_MOTIONPHOTO_SIGNATURE_V2 "\x4D\x6F\x74\x69\x6F\x6E\x50\x68\x6F\x74\x6F\x5F\x44\x61\x74\x61\x6d\x70\x76\x32"

/* Pixel 2 format includes metadata for "Motion Video Offset", but this seems to no longer include an offset as of Pixel 8
* So since we'll be searching entire file for the signature anyway, just search for mp4 signature now
 */
#define GOOGLE_MOTIONPHOTO_SIGNATURE_V1 "\x18\x66\x74\x79\x70\x6D\x70\x34\x32" // ftypmp42

 // As of Pixel 8, Google no longer includes an offset in the metadata, and uses new video file signature
#define GOOGLE_MOTIONPHOTO_SIGNATURE_V2 "\x1C\x66\x74\x79\x70\x69\x73\x6F\x6D" // ftypisom


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
        virtual juce::Image decodeThumbnail() { return juce::Image(); }
        virtual bool loadMetadataFromImage(juce::OwnedArray<gin::ImageMetadata>& metadata) = 0;

        virtual juce::uint32 getMotionPhotoSize() { return 0; }
        bool extractVideo(juce::OutputStream& os);
        
    protected:
        juce::MemoryBlock rawFileData;
        juce::uint32 videoPosition = 0;
        juce::uint32 videoSize = 0;
        bool hasCheckedForMotionPhoto = false;
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

        JUCE_LEAK_DETECTOR(PNGImageExtendedFormat)
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
        JUCE_LEAK_DETECTOR(WEBPImageExtendedFormat)
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
        JUCE_LEAK_DETECTOR(BMPImageExtendedFormat)
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
        JUCE_LEAK_DETECTOR(GIFImageExtendedFormat)
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
        juce::Image decodeThumbnail() override;
        bool loadMetadataFromImage(juce::OwnedArray<gin::ImageMetadata>& metadata) override;
        juce::uint32 getMotionPhotoSize() override;

    private:
        juce::JPEGImageFormat fmt;
        JUCE_LEAK_DETECTOR(JPEGImageExtendedFormat)
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
        juce::Image decodeThumbnail() override;
        bool loadMetadataFromImage(juce::OwnedArray<gin::ImageMetadata>& metadata) override;
        juce::uint32 getMotionPhotoSize() override;

    private:
#if YUZU_LINK_LIBHEIF
        heif_context* ctx = nullptr;
        heif_image_handle* primaryImageHandle = nullptr;
        heif_image_handle* primaryThumbHandle = nullptr;
#endif
        JUCE_LEAK_DETECTOR(HEIFImageExtendedFormat)
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

    private:
        JUCE_LEAK_DETECTOR(HEIFImageFormat)
    };

}