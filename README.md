# Yuzu
Image-Format Modules for JUCE

## Features
* Load HEIC files via libheif and libde265
* Load EXIF metadata for JPEG/HEIC
* Extract and decode embedded thumbnail images from JPEG/HEIC
* Extract embedded motion-photos as MP4 from JPEG/HEIC
* Copy/paste image to clipboard (Windows-only for now)

## Requirements
* [JUCE](https://github.com/juce-framework/JUCE)
* [Gin](https://github.com/FigBug/Gin)
* [libheif](https://github.com/strukturag/libheif)
* [libde265](https://github.com/strukturag/libde265)

## Notes
* YUZU_LINK_LIBRARY=0 to disable libheif and libde265

## Copyright
(c) 2024 Major Murphy
