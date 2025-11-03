#pragma once						
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
    bool hasFinished() ;
    void play();
    void stop();
    void setGain(float gain);
    void setPosition(double pos);
    double getPosition() const;
    double getLength() const;
    double getGain();
    

    //playlist functions
    void addFilesToPlaylist(const juce::Array<juce::File>& files);
    void playTrack(int trackIndex);
    void playNextTrack();
    void playPrviousTrack();

    int getNumTracks() const;
    juce::String getTrackTitle(int trackIndex) const;

    juce::String getMeta();
private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource playerAudio;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resamplingSource{ &transportSource, false };
    juce::String displayString;

    juce::Array<juce::File> trackList;
    int currentTrackIndex = -1;

   // JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};

