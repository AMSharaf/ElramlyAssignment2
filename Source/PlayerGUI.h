#pragma once						// PlayerGUI.h
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,

    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer,
    public juce::ListBoxModel
{
public:
    PlayerGUI(PlayerAudio& audioEngine);
    ~PlayerGUI() override;

    void resized() override;
    void paint(juce::Graphics& g) override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    void timerCallback();

    //ListBox Functions 
    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void listBoxItemClicked(int rowNumber, const juce::MouseEvent& e) override;

    bool wasPlaying=false;

private:
    PlayerAudio& playerAudio;

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

    juce::ListBox playlistBox;
    std::unique_ptr<juce::FileChooser> fileChooser;



    // Event handlers
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};

