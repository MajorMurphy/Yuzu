
#include "yuzu_WebPhotoViewer.h"

#if JUCE_WEB_BROWSER
using namespace juce;

yuzu::WebPhotoViewer::WebPhotoViewer() :
	juce::WebBrowserComponent(Options()
		.withBackend(Options::Backend::webview2)
		.withWinWebView2Options(WebBrowserComponent::Options::WinWebView2{}
			.withUserDataFolder(yuzu::FileHelpers::getTempFolder())))
{
	setOpaque(true);
	disableContextMenu();
}

yuzu::WebPhotoViewer::~WebPhotoViewer()
{
}

void yuzu::WebPhotoViewer::loadImage(juce::File file)
{
	if (!file.existsAsFile())
		return;

	if (!yuzu::JPEGImageExtendedFormat::usesFileExtension(file) &&
		!yuzu::PNGImageExtendedFormat::usesFileExtension(file) &&
		!yuzu::GIFImageExtendedFormat::usesFileExtension(file) &&
		!yuzu::WEBPImageExtendedFormat::usesFileExtension(file))
	{
		auto image = ExtendedImageFileFormat::findImageFormatForFile(file);
		if (image)
			loadImage(image->decodeImage());
		else jassert(false);

		return;
	}

	URL url(file);
	goToURL(url.toString(true));
	disableContextMenu();
	
}

void yuzu::WebPhotoViewer::loadImage(juce::Image image)
{
	if (image.isNull())
	{
		return;
	}

	juce::JPEGImageFormat jpg;
	jpg.setQuality(0.9f);
	auto file = FileHelpers::getTempFolder().getChildFile("tempimg.jpg");
	file.deleteFile();
	auto os = file.createOutputStream();
	if (os)
	{
		jpg.writeImageToStream(image, *os);
		os->flush();
	}
	else jassert(false);

	URL url(file);
	goToURL(url.toString(true));
	disableContextMenu();
}

void yuzu::WebPhotoViewer::disableContextMenu()
{
	evaluateJavascript("document.body.style.backgroundColor = \"#00000000\";");
	evaluateJavascript("document.addEventListener('contextmenu', event => event.preventDefault());");
}
#endif