#pragma once

#include <JuceHeader.h>

class WaveformDisplay : public juce::Component,
    public juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManager,
        juce::AudioThumbnailCache& thumbnailCache);
    ~WaveformDisplay() override;

    void paint(juce::Graphics& g) override;

    // This is called when the thumbnail has finished generating
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    // Call this from your MainComponent when a new file is loaded
    void loadFile(const juce::File& audioFile);

    // Call this from your timer to update the playhead
    void setPlaybackPosition(double newPosition);

private:
    juce::AudioThumbnail thumbnail;
    double currentPlaybackPosition = 0.0; // Stored in seconds

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};