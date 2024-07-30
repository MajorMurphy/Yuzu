#include "yuzu_FileHelpers.h"

#if JUCE_WINDOWS
#include <Windows.h>
#include <appmodel.h>
#include <processthreadsapi.h>
#endif


int64_t yuzu::FileHelpers::recursiveDirectorySize(juce::File dir)
{
    if (dir.existsAsFile())
        return dir.getSize();
    if (dir.isDirectory())
    {
        int64 size = 0;
        auto children = dir.findChildFiles(File::findFiles, true);
        for (auto file : children)
        {
            size += file.getSize();
        }
        return size;
    }
    return 0;
}

File yuzu::FileHelpers::getTempFolder()
{
#if JUCE_WINDOWS

    UINT32 bufferLength = 0;
    auto rc = GetCurrentPackageFamilyName(&bufferLength, nullptr);
    if (rc != ERROR_INSUFFICIENT_BUFFER)
    {
        if (rc == APPMODEL_ERROR_NO_PACKAGE)
            wprintf(L"Process has no package identity\n");
        else
            wprintf(L"Error %d in GetPackageFamilyName\n", rc);

        return juce::File::getSpecialLocation(File::tempDirectory);
    }
    PWSTR name = (PWSTR)malloc(bufferLength * sizeof(*name));
    if (name == NULL)
    {
        wprintf(L"Error allocating memory\n");
        jassertfalse;
        return juce::File::getSpecialLocation(File::tempDirectory);
    }

    rc = GetCurrentPackageFamilyName(&bufferLength, name);
    if (rc != ERROR_SUCCESS)
    {
        wprintf(L"Error %d retrieving PackageFamilyName\n", rc);
        jassertfalse;
        return juce::File::getSpecialLocation(File::tempDirectory);
    }
    else
        wprintf(L"%s\n", name);


    auto appdata = juce::File::getSpecialLocation(File::SpecialLocationType::userApplicationDataDirectory).getParentDirectory().getChildFile("Local");
    auto tempFolder = appdata.getChildFile("Packages").getChildFile(String(name)).getChildFile("LocalCache");

    free(name);

    return tempFolder;
#elif JUCE_ANDROID
    return juce::File::getSpecialLocation(File::SpecialLocationType::userApplicationDataDirectory).getChildFile("cache");

#else
    return juce::File::getSpecialLocation(File::tempDirectory);
#endif


}

juce::File yuzu::FileHelpers::getDataFolder()
{
#if JUCE_WINDOWS

    UINT32 bufferLength = 0;
    auto rc = GetCurrentPackageFamilyName(&bufferLength, nullptr);
    if (rc != ERROR_INSUFFICIENT_BUFFER)
    {
        if (rc == APPMODEL_ERROR_NO_PACKAGE)
            wprintf(L"Process has no package identity\n");
        else
            wprintf(L"Error %d in GetPackageFamilyName\n", rc);
        return juce::File::getSpecialLocation(File::userApplicationDataDirectory);
    }
    PWSTR name = (PWSTR)malloc(bufferLength * sizeof(*name));
    if (name == NULL)
    {
        wprintf(L"Error allocating memory\n");
        jassertfalse;
        return juce::File::getSpecialLocation(File::userApplicationDataDirectory);
    }

    rc = GetCurrentPackageFamilyName(&bufferLength, name);
    if (rc != ERROR_SUCCESS)
    {
        wprintf(L"Error %d retrieving PackageFamilyName\n", rc);
        jassertfalse;
        return juce::File::getSpecialLocation(File::userApplicationDataDirectory);
    }
    else
        wprintf(L"%s\n", name);


    auto appdata = juce::File::getSpecialLocation(File::SpecialLocationType::userApplicationDataDirectory).getParentDirectory().getChildFile("Local");
    auto tempFolder = appdata.getChildFile("Packages").getChildFile(String(name)).getChildFile("AppData");

    free(name);

    return tempFolder;
#else
    return juce::File::getSpecialLocation(File::userApplicationDataDirectory);
#endif


}
