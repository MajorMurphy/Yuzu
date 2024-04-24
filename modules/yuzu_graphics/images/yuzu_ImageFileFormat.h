#pragma once

namespace yuzu
{

    class ImageFileFormat : public juce::ImageFileFormat
    {
    public: 
        //==============================================================================
        /** Tries the built-in formats to see if it can find one to read this stream.
            There are currently built-in decoders for PNG, JPEG and GIF formats.
            HEIC can be decoded if YUZU_LINK_LIBHEIF or JUCE_USING_COREIMAGE_LOADER
            The object that is returned should not be deleted by the caller.
            @see canUnderstand, decodeImage, loadFrom
        */
        static juce::ImageFileFormat* findImageFormatForStream(juce::InputStream& input);

        /** Looks for a format that can handle the given file extension.
            There are currently built-in formats for PNG, JPEG and GIF formats.
            The object that is returned should not be deleted by the caller.
        */
        static juce::ImageFileFormat* findImageFormatForFileExtension(const juce::File& file);

        //==============================================================================
        /** Tries to load an image from a stream.

            This will use the findImageFormatForStream() method to locate a suitable
            codec, and use that to load the image.

            @returns        the image that was decoded, or an invalid image if it fails.
        */
        static juce::Image loadFrom(juce::InputStream& input);

        /** Tries to load an image from a file.

            This will use the findImageFormatForStream() method to locate a suitable
            codec, and use that to load the image.

            @returns        the image that was decoded, or an invalid image if it fails.
        */
        static juce::Image loadFrom(const juce::File& file);

        /** Tries to load an image from a block of raw image data.

            This will use the findImageFormatForStream() method to locate a suitable
            codec, and use that to load the image.

            @returns        the image that was decoded, or an invalid image if it fails.
        */
        static juce::Image loadFrom(const void* rawData,  size_t numBytesOfData);


        virtual bool loadMetadataFromImage(juce::InputStream& is, juce::OwnedArray<gin::ImageMetadata>& metadata) = 0;

    };

    //==============================================================================
    /**
    A subclass of ImageFileFormat for reading and writing HEICfiles.

    @see ImageFileFormat, PNGImageFormat

    @tags{Graphics}
    */
    class HEIFImageFormat : public yuzu::ImageFileFormat
    {
    public:
        //==============================================================================
        HEIFImageFormat();
        ~HEIFImageFormat() override;
        //==============================================================================
        juce::String getFormatName() override;
        bool usesFileExtension(const juce::File&) override;
        bool canUnderstand(juce::InputStream&) override;
        juce::Image decodeImage(juce::InputStream&) override;
        bool writeImageToStream(const juce::Image& sourceImage, juce::OutputStream& destStream) override;
        bool loadMetadataFromImage(juce::InputStream& is, juce::OwnedArray<gin::ImageMetadata>& metadata) override;

    private:
        bool lossless = true;
        float quality;
    };
}