# Yuzu
Image-Format Modules for JUCE

## Features
* Load HEIC files (by linking libheif and libde265)
* Extract and decode embedded thumbnail images from JPEG/HEIC
* Extract embedded motion-photos as MP4 from JPEG/HEIC
* Load EXIF metadata for JPEG/HEIC
* Copy/paste image to clipboard (Windows-only for now)

## Requirements
* [JUCE](https://github.com/juce-framework/JUCE)
* [Gin](https://github.com/FigBug/Gin)
* [libheif](https://github.com/strukturag/libheif)
* [libde265](https://github.com/strukturag/libde265)

## Notes
* YUZU_LINK_LIBRARY=0 to disable libheif and libde265

## Demo
Use [Projucer](https://github.com/juce-framework/JUCE/tree/master/extras/Projucer/Builds) to export [Demo.jucer](https://github.com/MajorMurphy/Yuzu/blob/53550426ac632eac7598eaf1da5a0556a352db39/examples/Demo/Demo.jucer) to Visual Studio or Xcode

## Copyright
(c) 2024 Major Murphy
