#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);

    

    setSize(800, 600);


    setAudioChannels(0, 2); 
}

MainComponent::~MainComponent()
{
    // Shut down the audio hardware
    shutdownAudio();
}



void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{

    deck1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    deck2.prepareToPlay(samplesPerBlockExpected, sampleRate);


    // This allocates memory for the buffer
    mixBuffer.setSize(2, samplesPerBlockExpected); // 2 channels, block size
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    
    bufferToFill.clearActiveBufferRegion();

   
    deck1.getNextAudioBlock(bufferToFill);

    // Clear your temporary mixBuffer (fill it with silence)
    mixBuffer.clear();

    // Create a temporary "wrapper" for your mixBuffer
    juce::AudioSourceChannelInfo mixBufferInfo(&mixBuffer, 0, bufferToFill.numSamples);


    deck2.getNextAudioBlock(mixBufferInfo);

    // Mix! Add the audio from mixBuffer into the main buffer
    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        bufferToFill.buffer->addFrom(
            channel,                  // destination channel
            bufferToFill.startSample, // destination start sample
            mixBuffer,                // the source buffer (our temp one)
            channel,                  // source channel
            0,                        // source start sample
            bufferToFill.numSamples   // number of samples to add
        );
    }

}

void MainComponent::releaseResources()
{
    deck1.releaseResources();
    deck2.releaseResources();


    mixBuffer.setSize(0, 0);
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();

    

    auto deckWidth = bounds.getWidth() / 2;
    deck1.setBounds(bounds.removeFromLeft(deckWidth));
    deck2.setBounds(bounds);
}
