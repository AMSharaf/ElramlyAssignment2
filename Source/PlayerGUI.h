#pragma once						// PlayerGUI.h
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void resized() override;
    void paint(juce::Graphics& g) override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    void timerCallback();

private:
    PlayerAudio playerAudio;

    // GUI elements
        // GUI Controls
    juce::TextButton loadButton{ "Load Files" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton pauseButton{ "pause || " };
    juce::TextButton playButton{ "play " };
    juce::TextButton goToStartButton{ "|<" };
    juce::TextButton goToEndButton{ ">|" };
    juce::ToggleButton muteButton{ "Mute" };
    juce::ToggleButton loopButton{ "Loop" };
    juce::Slider volumeSlider;
    juce::Slider speedSlider;
    juce::Slider positionSlider;
    juce::Slider loopSlider;
    juce::Label positionLabel;
    juce::Label volumeLabel;
    juce::Label speedLabel;
    juce::Label metadataLabel;

    std::unique_ptr<juce::FileChooser> fileChooser;


    // Event handlers
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};

