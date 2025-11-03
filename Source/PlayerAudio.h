#pragma once							// PlayerAudio.h
#include <JuceHeader.h>

class PlayerAudio
{
public:
    PlayerAudio();
    ~PlayerAudio();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    void setResamplingRatio(float r);
    bool loadFile(const juce::File& file);
    bool isPlaying();
    void play();
    void stop();
    juce::String getMeta();
    void setGain(float gain);
    void setPosition(double pos);
    double getPosition() const;
    double getLength() const;

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource playerAudio;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resamplingSource{ &transportSource, false };
    juce::String displayString;

   // JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};

