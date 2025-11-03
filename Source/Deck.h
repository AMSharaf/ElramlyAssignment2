#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"
#include "PlayerGUI.h"

class Deck : public juce::Component
{
public:
    Deck();
    void resized() override;
    void setGain(float gain);
    double getGain();
	void setPosition(double pos);
	double getPosition();
    void stop();
    void play();
    void ChangeVolumeSlider();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

private:
    PlayerAudio playerAudio;
    PlayerGUI   playerGUI;
};