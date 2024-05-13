/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 8.0.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...

// Copyright (c) 2024 Major Murphy

//[/Headers]

#include "DemoComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace yuzu;
using namespace juce;
//[/MiscUserDefs]

//==============================================================================
DemoComponent::DemoComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    imagePreview.reset (new juce::ImageComponent());
    addAndMakeVisible (imagePreview.get());
    imagePreview->setName ("image preview");

    thumbnailPreview.reset (new juce::ImageComponent());
    addAndMakeVisible (thumbnailPreview.get());
    thumbnailPreview->setName ("thumbnail");

    metadataText.reset (new juce::TextEditor ("metadata"));
    addAndMakeVisible (metadataText.get());
    metadataText->setMultiLine (true);
    metadataText->setReturnKeyStartsNewLine (false);
    metadataText->setReadOnly (true);
    metadataText->setScrollbarsShown (true);
    metadataText->setCaretVisible (false);
    metadataText->setPopupMenuEnabled (true);
    metadataText->setText (juce::String());


    //[UserPreSize]
    menuBar.reset(new MenuBarComponent(this));
    addAndMakeVisible(menuBar.get());
    setApplicationCommandManagerToWatch(&commandManager);
    commandManager.registerAllCommandsForTarget(this);
    commandManager.setFirstCommandTarget(this);
    addKeyListener(commandManager.getKeyMappings());
#if JUCE_MAC
    MenuBarModel::setMacMainMenu(this);
#endif
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    reload(juce::Image(), juce::StringPairArray(), juce::String(), false, juce::Image(), 0);
    //[/Constructor]
}

DemoComponent::~DemoComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
#if JUCE_MAC
    MenuBarModel::setMacMainMenu(nullptr);
#endif
    //[/Destructor_pre]

    imagePreview = nullptr;
    thumbnailPreview = nullptr;
    metadataText = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void DemoComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void DemoComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    imagePreview->setBounds (0, 181, getWidth() - 0, getHeight() - 164);
    thumbnailPreview->setBounds (getWidth() - 4 - proportionOfWidth (0.3158f), 54, proportionOfWidth (0.3158f), 120 - 0);
    metadataText->setBounds (8, 53, proportionOfWidth (0.6640f), 120);
    //[UserResized] Add your own custom resize handling here..
    menuBar->setBounds(getBounds().removeFromTop(LookAndFeel::getDefaultLookAndFeel()
        .getDefaultMenuBarHeight()));
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void DemoComponent::setImage(juce::Image img)
{
    fmt = nullptr;
    reload(img, juce::StringPairArray(), juce::String(), false, juce::Image(), 0);
}
void DemoComponent::setImage(juce::File imgFile)
{
    fmt = ExtendedImageFileFormat::findImageFormatForFile(imgFile);
    if (fmt)
        reload(fmt->decodeImage(), fmt->getExif(), fmt->getXmp(), fmt->containsUltraHDR(), fmt->decodeThumbnail(), fmt->getMotionPhotoSize());
    else
        jassertfalse;
}
void DemoComponent::reload(juce::Image img,
    juce::StringPairArray exif,
    juce::String xmp,
    bool uhdr,
    juce::Image thumbnail,
    int motionPhotoSize)
{
    juce::String mdString;
    imagePreview->setImage(img);

    thumbnailPreview->setImage(thumbnail);
    mdString += ("Primary: " + String(img.getWidth()) + " x " + String(img.getHeight()));

    mdString += ("\nThumbnail: " + String(thumbnail.getWidth()) + " x " + String(thumbnail.getHeight()));
    mdString += ("\nMotion Photo: " + String(motionPhotoSize) + " b");
    mdString += ("\nUltra HDR: " + String(uhdr ? "true" : "false"));
    mdString += "\n";

    auto keys = exif.getAllKeys();
    auto values = exif.getAllValues();
    if (keys.size() == values.size())
    {
        for (int i = 0; i < keys.size(); i++)
        {
            mdString += keys[i] + ":  " + values[i] + "\n";
        }
    }

    if(xmp.isNotEmpty())
        mdString += "XMP:\n" + xmp;
    metadataText->setText(mdString);
}

void DemoComponent::browseForImage()
{
    RuntimePermissions::request(RuntimePermissions::readMediaImages,
        [ptr = DemoComponent::SafePointer(this)](bool granted)
        {
            if (!ptr)
                return;
            else if (!granted)
            {
                NativeMessageBox::showMessageBoxAsync(MessageBoxIconType::WarningIcon, "Sorry", "Permission needed to open photos.");
            }
            else
            {
                String extentions("*.jpg;*.jpeg;*.png;*.gif;*.webp;*.bmp");
                #if YUZU_LINK_LIBHEIF
                    extentions += ";*.heic;*.heif";
                #endif
                ptr->chooser = std::make_unique<juce::FileChooser>("Open Image",
                    File::getSpecialLocation(File::SpecialLocationType::userPicturesDirectory),
                    extentions, true);
                auto folderChooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::FileChooserFlags::canSelectFiles;

                ptr->chooser->launchAsync(folderChooserFlags, [ptr](const FileChooser& chooser)
                    {
                        if (chooser.getResult().existsAsFile())
                        {
                            ptr->setImage(chooser.getResult());
                        }
                    });

            }
        });
}

void DemoComponent::exportVideo()
{
    RuntimePermissions::request(RuntimePermissions::writeExternalStorage,
        [this](bool /* wasGranted */)
        {

            auto suggestedFileName = File::getSpecialLocation(File::userMoviesDirectory).getNonexistentChildFile("MotionPhoto", ".mp4", true);

            chooser = std::make_unique<juce::FileChooser>("Export Motion Photo",
                suggestedFileName,
                "*.mp4");

            auto folderChooserFlags = FileBrowserComponent::warnAboutOverwriting | FileBrowserComponent::canSelectFiles | FileBrowserComponent::saveMode;

            chooser->launchAsync(folderChooserFlags, [this](const FileChooser& chooser)
                {
                    auto file = chooser.getResult();
                    if (file == File())
                        // user canceled
                        return;

                    if (!file.hasFileExtension("mp4"))
                        file = file.withFileExtension(".mp4");

                    file.deleteFile();

                    auto os = file.createOutputStream();
                    if (os->openedOk() && fmt)
                    {
                        fmt->extractVideo(*os);
                        if (file.existsAsFile())
                            file.revealToUser();
                    }


                });

        });
}

void DemoComponent::copyImageToClipboard()
{
    yuzu::SystemClipboard::copyImage(imagePreview->getImage());
}

void DemoComponent::pasteImageFromClipboard()
{
    setImage(yuzu::SystemClipboard::pasteImage());
}

void DemoComponent::about()
{
    String text = JUCEApplication::getInstance()->getApplicationName();
    text += " - v" + JUCEApplication::getInstance()->getApplicationVersion() + "\n";
    text += "\nCopyright (C) Major Murphy";
    NativeMessageBox::showMessageBoxAsync(MessageBoxIconType::InfoIcon, "About", text);
}
inline juce::StringArray DemoComponent::getMenuBarNames()
{
    return { "File", "Edit", "Help" };
}
inline void DemoComponent::getAllCommands(juce::Array<juce::CommandID>& c)
{
    juce::Array<juce::CommandID> commands
    {
        CommandIDs::openImageFileCmd,
        CommandIDs::exportVideoCmd,
        CommandIDs::copyImageToClipboardCmd,
        CommandIDs::pasteImageFromClipboardCmd,
        CommandIDs::aboutCmd
    };
    c.addArray(commands);
}
inline juce::PopupMenu DemoComponent::getMenuForIndex(int menuIndex, const juce::String&)
{
    juce::PopupMenu menu;

    if (menuIndex == 0)
    {
        menu.addCommandItem(&commandManager, CommandIDs::openImageFileCmd);
        menu.addCommandItem(&commandManager, CommandIDs::exportVideoCmd);
    }
    else if (menuIndex == 1)
    {
        menu.addCommandItem(&commandManager, CommandIDs::copyImageToClipboardCmd);
        menu.addCommandItem(&commandManager, CommandIDs::pasteImageFromClipboardCmd);
    }
    else if (menuIndex == 2)
    {
        menu.addCommandItem(&commandManager, CommandIDs::aboutCmd);
    }

    return menu;
}
inline void DemoComponent::getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result)
{
    switch (commandID)
    {
    case CommandIDs::openImageFileCmd:
        result.setInfo("Open Image", "Open Image File", "Menu", 0);
        result.addDefaultKeypress('o', juce::ModifierKeys::commandModifier);
        break;
    case CommandIDs::exportVideoCmd:
        result.setInfo("Export Video", "Export Video File", "Menu", 0);
        result.setActive(fmt && fmt->getMotionPhotoSize() > 0);
        result.addDefaultKeypress('e', juce::ModifierKeys::commandModifier);
        break;
    case CommandIDs::copyImageToClipboardCmd:
        result.setInfo("Copy Image", "Copy Image to Clipboard", "Menu", 0);
#if JUCE_WINDOWS
        // only Windows for now
        result.setActive(imagePreview->getImage().isValid());
#else
        result.setActive(false);
#endif
        result.addDefaultKeypress('c', juce::ModifierKeys::commandModifier);
        break;
    case CommandIDs::pasteImageFromClipboardCmd:
        result.setInfo("Paste Image", "Paste Image from Clipboard", "Menu", 0);
        result.setActive(yuzu::SystemClipboard::checkForImage());
        result.addDefaultKeypress('v', juce::ModifierKeys::commandModifier);
        break;
    case CommandIDs::aboutCmd:
        result.setInfo("About", "About this application", "Menu", 0);
        result.addDefaultKeypress(juce::KeyPress::F1Key, juce::ModifierKeys::noModifiers);
        break;
    default:
        break;
    }
}
inline bool DemoComponent::perform(const InvocationInfo& info)
{
    switch (info.commandID)
    {
    case CommandIDs::openImageFileCmd:
        browseForImage();
        break;
    case CommandIDs::exportVideoCmd:
        exportVideo();
        break;
    case CommandIDs::copyImageToClipboardCmd:
        copyImageToClipboard();
        break;
    case CommandIDs::pasteImageFromClipboardCmd:
        pasteImageFromClipboard();
        break;
    case CommandIDs::aboutCmd:
        about();
        break;
    default:
        return false;
    }

    repaint();
    return true;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="DemoComponent" componentName=""
                 parentClasses="public juce::Component, public juce::ApplicationCommandTarget, public juce::MenuBarModel"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GENERICCOMPONENT name="image preview" id="bf43439ea64874c4" memberName="imagePreview"
                    virtualName="" explicitFocusOrder="0" pos="0 181 0M 164M" class="juce::ImageComponent"
                    params=""/>
  <GENERICCOMPONENT name="thumbnail" id="29e9dcfea34f1bba" memberName="thumbnailPreview"
                    virtualName="" explicitFocusOrder="0" pos="4Rr 54 31.579% 0M"
                    posRelativeH="5bdef0c0aaac48d6" class="juce::ImageComponent"
                    params=""/>
  <TEXTEDITOR name="metadata" id="5bdef0c0aaac48d6" memberName="metadataText"
              virtualName="" explicitFocusOrder="0" pos="8 53 66.404% 120"
              initialText="" multiline="1" retKeyStartsLine="0" readonly="1"
              scrollbars="1" caret="0" popupmenu="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

