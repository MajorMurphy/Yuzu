
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
}

yuzu::WebPhotoViewer::~WebPhotoViewer()
{
}

void yuzu::WebPhotoViewer::loadImage(juce::File file)
{
	if (!file.existsAsFile())
		return;

	if (!yuzu::JPEGImageExtendedFormat::usesFileExtension(file))
		return;

	URL url(file);
	goToURL(url.toString(true));
	evaluateJavascript("document.addEventListener('contextmenu', event => event.preventDefault());");
}
#endif