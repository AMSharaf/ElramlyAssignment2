#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManager,
    juce::AudioThumbnailCache& thumbnailCache) :
    thumbnail(512, formatManager, thumbnailCache)
{
    // Register to be notified when the thumbnail changes (i.e., is generated)
    thumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay() {}

void WaveformDisplay::loadFile(const juce::File& audioFile)
{
    currentPlaybackPosition = 0.0;
    // Tell the thumbnail to use the new file
    thumbnail.setSource(new juce::FileInputSource(audioFile));
}

void WaveformDisplay::setPlaybackPosition(double newPosition)
{
    currentPlaybackPosition = newPosition;
    repaint(); // Trigger a repaint to move the playhead
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &thumbnail)
    {
        // The thumbnail has been updated, so repaint our component
        repaint();
    }
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);

    // Check if the thumbnail has a waveform to draw
    if (thumbnail.isFullyLoaded())
    {
        // Draw the waveform
        g.setColour(juce::Colours::lightgreen);
        thumbnail.drawChannels(g,
            getLocalBounds(),
            0.0,                  // start time
            thumbnail.getTotalLength(), // end time
            1.0f);                // vertical zoom
    }
    else
    {
        // If the thumbnail is still loading, show some text
        g.setColour(juce::Colours::white);
        g.drawText("Loading waveform...", getLocalBounds(), juce::Justification::centred, false);
    }

    // Draw the playhead
    g.setColour(juce::Colours::red);
    auto totalLength = thumbnail.getTotalLength();
    if (totalLength > 0.0)
    {
        // Map the current time to an x-coordinate
        auto playheadX = juce::jmap(currentPlaybackPosition, 0.0, totalLength, 0.0, (double)getWidth());
        g.drawVerticalLine((int)playheadX, 0.0f, (float)getHeight());
    }
}