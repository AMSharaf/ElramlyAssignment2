#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}
PlayerAudio::~PlayerAudio()
{
}
void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    resamplingSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resamplingSource.getNextAudioBlock(bufferToFill);
}

void PlayerAudio::releaseResources()
{
    resamplingSource.releaseResources();
}

void PlayerAudio::setResamplingRatio(float r)
{
    resamplingSource.setResamplingRatio(r);
}

bool PlayerAudio::loadFile(const juce::File& file)
{
    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
            auto metadata = reader->metadataValues;

            auto title = metadata.getValue("TIT2",
                metadata.getValue("INAM",
                    metadata.getValue("title",
                        file.getFileName())));

            auto artist = metadata.getValue("TPE1",
                metadata.getValue("IART",
                    metadata.getValue("artist",
                        metadata.getValue("Author",
                            "Unknown Artist"))));
            displayString = title + "\n" + artist;

            
            // 🔑 Disconnect old source first
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

            // Create new reader source
            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

            // Attach safely
            transportSource.setSource(readerSource.get(),
                0,
                nullptr,
                reader->sampleRate);
            transportSource.start();
        }


    }
    return true;
}

void PlayerAudio::play()
{
    transportSource.start();
}

void PlayerAudio::stop()
{
    transportSource.stop();
}

juce::String PlayerAudio::getMeta()
{
    return displayString;
}

void PlayerAudio::setGain(float gain) {
    transportSource.setGain(gain);
}
void PlayerAudio::setPosition(double pos) {
    transportSource.setPosition(pos);
};
double PlayerAudio::getPosition() const{
    return transportSource.getCurrentPosition();
};
double PlayerAudio::getLength() const{
    return transportSource.getLengthInSeconds();
};

bool PlayerAudio::isPlaying(){
    return transportSource.isPlaying();
}

// This function replaces your old loadFile. It loads a track by its index in the list.
void PlayerAudio::playTrack(int trackIndex)
{
    // Check if the index is valid
    if (trackIndex >= 0 && trackIndex < trackList.size())
    {
        currentTrackIndex = trackIndex;
        auto file = trackList[currentTrackIndex];

        // This is your existing safe loading code
        if (auto* reader = formatManager.createReaderFor(file))
        {
            auto metadata = reader->metadataValues;

            auto title = metadata.getValue("TIT2",
                metadata.getValue("INAM",
                    metadata.getValue("title",
                        file.getFileName())));

            auto artist = metadata.getValue("TPE1",
                metadata.getValue("IART",
                    metadata.getValue("artist",
                        metadata.getValue("Author",
                            "Unknown Artist"))));
            displayString = title + "\n" + artist;

            transportSource.start();
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
            transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
            transportSource.start();

        }
    }
}

void PlayerAudio::playNextTrack()
{
    // Move to the next track, wrapping around to the beginning if at the end
    int nextTrack = (currentTrackIndex + 1) % trackList.size();
    playTrack(nextTrack);
}

void PlayerAudio::addFilesToPlaylist(const juce::Array<juce::File>& files)
{
    trackList.addArray(files);

}

int PlayerAudio::getNumTracks() const
{
    return trackList.size();
}

juce::String PlayerAudio::getTrackTitle(int trackIndex) const
{
    if (trackIndex >= 0 && trackIndex < trackList.size())
    {
        return trackList[trackIndex].getFileName();
    }
    return "Unknown Track";
}