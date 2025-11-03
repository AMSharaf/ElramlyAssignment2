#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);
    
    //addAndMakeVisible(SyncButton);
    //SyncButton.addListener(this);

    addAndMakeVisible(masterVolumeSlider);
    masterVolumeSlider.addListener(this);
    masterVolumeSlider.setRange(0.0, 1.0); // 0.0 (silent) to 1.0 (full)
    masterVolumeSlider.setValue(0.5);
    masterVolumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);


	addAndMakeVisible(SyncButton);
    SyncButton.addListener(this);
	addAndMakeVisible(StopAllButton);
    StopAllButton.addListener(this);
	addAndMakeVisible(PlayAllButton);
    PlayAllButton.addListener(this);

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
	masterVolumeSlider.setBounds(bounds.removeFromBottom(50).reduced(10));
	
	SyncButton.setBounds(getWidth()/2-40, getHeight() - 100, 80, 40);
	StopAllButton.setBounds(getWidth()/2-140, getHeight() - 100, 80, 40);
	PlayAllButton.setBounds(getWidth()/2+60, getHeight() - 100, 80, 40);
    auto deckWidth = bounds.getWidth()/ 2;
    deck1.setBounds(bounds.removeFromLeft(deckWidth));
    deck2.setBounds(bounds.removeFromLeft(deckWidth));
}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &masterVolumeSlider)
    {
        float volume = (masterVolumeSlider.getValue());
        deck1.setGain(1-volume);
        deck2.setGain(volume);
        deck1.ChangeVolumeSlider();
        deck2.ChangeVolumeSlider();
	}
}

void MainComponent::buttonClicked(juce::Button* button)
{
    if(button == &SyncButton)
    {
        if(deck1.getPosition() > deck2.getPosition())
        {
            deck1.setPosition(deck2.getPosition());
        }
        else
        {
            deck2.setPosition(deck1.getPosition());
		}
	}
    if(button == &StopAllButton)
    {
        deck1.stop();
        deck2.stop();
	}
    if(button == &PlayAllButton)
    {
        deck1.play();
        deck2.play();
	}
}

