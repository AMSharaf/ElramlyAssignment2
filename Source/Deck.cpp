#include "Deck.h"

Deck::Deck() : playerGUI(playerAudio)
{
    addAndMakeVisible(playerGUI);
}

void Deck::resized()
{
    playerGUI.setBounds(getLocalBounds());
}

void Deck::setGain(float gain)
{
    playerAudio.setGain(gain);
}

// ✅ ADD THESE IMPLEMENTATIONS:
void Deck::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // Pass the call to this deck's engine
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void Deck::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Pass the call to this deck's engine
    playerAudio.getNextAudioBlock(bufferToFill);
}

void Deck::releaseResources()
{
    // Pass the call to this deck's engine
    playerAudio.releaseResources();
}