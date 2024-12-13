/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionAudioProcessorEditor::DistortionAudioProcessorEditor (DistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    /*~~~~ここから追加~~~~*/
    addAndMakeVisible(webComponent);
    //webComponent.goToURL("http://localhost:5173/");
    webComponent.goToURL(juce::WebBrowserComponent::getResourceProviderRoot());
    /*~~~~~追加終わり~~~~~*/
    
    setSize(170, 650); // 数値の変更
    //setSize (400, 300);
}

DistortionAudioProcessorEditor::~DistortionAudioProcessorEditor()
{
}

//==============================================================================
void DistortionAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
   
}

void DistortionAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
     /*~~~~ここから追加~~~~*/
    webComponent.setBounds(getLocalBounds());
    /*~~~~~追加終わり~~~~~*/
}

/*~~~~ここから追加~~~~*/
std::optional<juce::WebBrowserComponent::Resource>
DistortionAudioProcessorEditor::getResource(const juce::String& url) {
    const auto urlToRetrive = url == "/"
        ? juce::String{ "index.html" }
    : url.fromFirstOccurrenceOf("/", false, false);

    static auto streamZip = juce::MemoryInputStream(
        juce::MemoryBlock(BinaryData::assets_zip, BinaryData::assets_zipSize),
        true);

    static juce::ZipFile archive{ streamZip };

    if (auto* entry = archive.getEntry(urlToRetrive)) {
        auto entryStream = rawToUniquePtr(archive.createStreamForEntry(*entry));
        std::vector<std::byte> result((size_t)entryStream->getTotalLength());
        entryStream->setPosition(0);
        entryStream->read(result.data(), result.size());

        auto mime = getMimeForExtension(
            entry->filename.fromLastOccurrenceOf(".", false, false).toLowerCase());
        return juce::WebBrowserComponent::Resource{ std::move(result),
                                                   std::move(mime) };
    }
    return std::nullopt;
}

const char* DistortionAudioProcessorEditor::getMimeForExtension(
    const juce::String& extension) {
    static const std::unordered_map<juce::String, const char*> mimeMap = {
        {{"htm"}, "text/html"},
        {{"html"}, "text/html"},
        {{"txt"}, "text/plain"},
        {{"jpg"}, "image/jpeg"},
        {{"jpeg"}, "image/jpeg"},
        {{"svg"}, "image/svg+xml"},
        {{"ico"}, "image/vnd.microsoft.icon"},
        {{"json"}, "application/json"},
        {{"png"}, "image/png"},
        {{"css"}, "text/css"},
        {{"map"}, "application/json"},
        {{"js"}, "text/javascript"},
        {{"woff2"}, "font/woff2"} };

    if (const auto it = mimeMap.find(extension.toLowerCase());
        it != mimeMap.end())
        return it->second;

    jassertfalse;
    return "";
}
/*~~~~~追加終わり~~~~~*/