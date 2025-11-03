#pragma once
#include <JuceHeader.h>
#include "Deck.h"

// ✅ 1. MAKE SURE IT INHERITS
class MainComponent : public juce::AudioAppComponent,
	                  public juce::Slider::Listener

{
public:
    MainComponent();
    ~MainComponent() override; // Need a destructor to shut down audio

    // ✅ 2. DECLARE THE AUDIO CALLBACKS
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;

private:
    Deck deck1;
    Deck deck2;

    juce::Slider masterVolumeSlider;

    juce::AudioBuffer<float> mixBuffer;

};