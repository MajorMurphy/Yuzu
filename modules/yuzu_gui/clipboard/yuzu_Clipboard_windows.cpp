// Copyright (c) 2024 Major Murphy

#include "yuzu_Clipboard.h"

#if JUCE_WINDOWS
#include <Windows.h>

using namespace yuzu;
using namespace juce;
//==============================================================================
Image yuzu::SystemClipboard::getImageFromClipboard()
{
    Image img;
    if (!IsClipboardFormatAvailable(CF_DIB))
    {
        jassertfalse;
        auto error = GetLastError();
        DBG("CF_DIB clipboard format not available. Error: " + String(error));
        return img;
    }

    if (!OpenClipboard(NULL))
    {
        jassertfalse;
        return img;
    }

    auto hCb = GetClipboardData(CF_DIB);
    if (!hCb || hCb == INVALID_HANDLE_VALUE)
    {
        jassertfalse;
        return img;
    }
    auto data = GlobalLock(hCb);
    if (data)
    {
        auto infoHeader = (BITMAPINFOHEADER*)data;
        if ((infoHeader->biBitCount == 32 || infoHeader->biBitCount == 24)
            && (infoHeader->biCompression == BI_RGB || infoHeader->biCompression == BI_BITFIELDS)
            )
        {
            if (infoHeader->biCompression == BI_BITFIELDS)
                img = Image(Image::ARGB, infoHeader->biWidth, abs(infoHeader->biHeight), true);
            else
                img = Image(Image::RGB, infoHeader->biWidth, abs(infoHeader->biHeight), true);

            auto imageData = (char*)(data)+sizeof(BITMAPINFOHEADER);
            auto stride = ((((infoHeader->biWidth * infoHeader->biBitCount) + 31) & ~31) >> 3);

            for (int y = 0; y < abs(infoHeader->biHeight); y++)
            {
                for (int x = 0; x < infoHeader->biWidth; x++)
                {
                    auto pixelPtr = (uint8*)imageData + y * stride + x * infoHeader->biBitCount / 8;
                    Colour pixel;
                    if (infoHeader->biBitCount == 32)
                        pixel = Colour(*(uint32*)pixelPtr);
                    else if (infoHeader->biBitCount == 24)
                    {
                        auto g = *(uint32*)(pixelPtr - 2);
                        auto r = *(uint32*)(pixelPtr - 1);
                        auto b = *(uint32*)pixelPtr;
                        pixel = Colour((uint8)r, (uint8)g, (uint8)b);
                    }
                    else jassertfalse;
                    img.setPixelAt(x, abs(infoHeader->biHeight) - y - 1, pixel);
                }
            }
        }
        else if (
            infoHeader->biCompression == BI_PNG ||
            infoHeader->biCompression == BI_JPEG
            )
        {

            auto imageData = (char*)(data)+sizeof(BITMAPINFOHEADER);
            img = ImageFileFormat::loadFrom(imageData, infoHeader->biSizeImage);
        }
        else
            jassertfalse;

        GlobalUnlock(data);
    }
    CloseClipboard();
    return img;
}

void yuzu::SystemClipboard::copyImageToClipboard(Image image)
{
    if (!OpenClipboard(NULL))
    {
        jassertfalse;
        return;
    }
    image = image.convertedToFormat(Image::PixelFormat::RGB);

    BITMAPINFOHEADER header;
    header.biSize = sizeof(BITMAPINFOHEADER);
    header.biWidth = image.getWidth();
    header.biHeight = image.getHeight();
    header.biPlanes = 1; // always one
    header.biBitCount = image.hasAlphaChannel() ? 32 : 24;
    header.biCompression = image.hasAlphaChannel() ? BI_BITFIELDS : BI_RGB;
    auto stride = ((((header.biWidth * header.biBitCount) + 31) & ~31) >> 3);
    header.biSizeImage = stride * header.biWidth; // can be zero for uncompressed images
    header.biXPelsPerMeter = 0; // not relevent
    header.biYPelsPerMeter = 0; // not relevent
    header.biClrUsed = 0;
    header.biClrImportant = 0;


    auto totalSize = sizeof(BITMAPINFOHEADER) + header.biSizeImage;
    auto handle = GlobalAlloc(GMEM_MOVEABLE, totalSize);
    if (!handle)
    {
        jassertfalse;
        CloseClipboard();
        return;
    }
    auto cbData = GlobalLock(handle);
    if (!cbData)
    {
        jassertfalse;
        if (handle)
            GlobalFree(handle);
        CloseClipboard();
        return;
    }

    memcpy_s(cbData, header.biSizeImage, &header, sizeof(header));

    Image::BitmapData bmp(image, Image::BitmapData::readOnly);
    jassert(bmp.lineStride == stride);

    auto currentLine = (void*)((int64)cbData + (sizeof(header)));
    for (int y = image.getHeight() - 1; y >= 0; y--)
    {
        // assemble bitmap bottom-up, line by line
        memcpy_s(currentLine, stride, bmp.getLinePointer(y), bmp.lineStride);
        currentLine = (void*)((int64)currentLine + stride);
    }

    GlobalUnlock(handle);
    EmptyClipboard();
    if (!SetClipboardData(CF_DIB, handle))
    {
        jassertfalse;
        DBG("SetClipboardData error: " + String(GetLastError()));
    }
    CloseClipboard();

    if (handle)
        GlobalFree(handle);

    return;

}
#endif