#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    // Add buttons
    for (auto* btn : { &loadButton, &restartButton , &stopButton,&pauseButton,&playButton,&goToStartButton,&goToEndButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }
    addAndMakeVisible(&muteButton);
    addAndMakeVisible(&loopButton);

    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
    volumeSlider.setSliderStyle(juce::Slider::Rotary);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.attachToComponent(&volumeSlider, true);


    //Speed Slider
    speedSlider.setRange(0.5, 2.0, 0.01);
    speedSlider.setValue(1);
    speedSlider.setSliderStyle(juce::Slider::Rotary);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, true);

    loopSlider.addListener(this);
    addAndMakeVisible(loopSlider);
    loopSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 40);
    loopSlider.setSliderStyle(juce::Slider::TwoValueHorizontal);
    loopSlider.setRange(0.0, 1.0);
    loopSlider.setEnabled(false);

    //Position Slider
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    positionSlider.setRange(0.0, 1.0);
    positionSlider.setEnabled(false);


    addAndMakeVisible(positionLabel);
    positionLabel.setText("00:00/00:00", juce::dontSendNotification);
    positionLabel.setJustificationType(juce::Justification::centred);


    addAndMakeVisible(metadataLabel);
    metadataLabel.setFont(juce::Font(16.0f));
    metadataLabel.setJustificationType(juce::Justification::centredLeft);

    //playlist Box
    addAndMakeVisible(playlistBox);
    playlistBox.setModel(this);


    startTimerHz(30);




}

PlayerGUI::~PlayerGUI()
{
}

void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    restartButton.setBounds(140, y, 80, 40);
    stopButton.setBounds(240, y, 80, 40);
    goToStartButton.setBounds(340,y, 80, 40);
    goToEndButton.setBounds(440, y, 80, 40);
    muteButton.setBounds(540, y, 80, 40);
    loopButton.setBounds(620,  y, 80, 40);
    /*prevButton.setBounds(340, y, 80, 40);
    nextButton.setBounds(440, y, 80, 40);*/
    
    volumeSlider.setBounds(400, 4*y, 100, 100);
    speedSlider.setBounds(550,4*y,100,100);
    loopSlider.setBounds(20, 7* y-5, 300, 25);
    positionSlider.setBounds(20, 7* y, 300, 15);
    
    positionLabel.setBounds(220, 4* y, 100, 40);
    
    pauseButton.setBounds(20, 4*y, 80, 40);
    playButton.setBounds(120, 4*y, 80, 40);
    
    metadataLabel.setBounds(20, 8*y,400, 60);

    playlistBox.setBounds(20,12*y,400,400);
    
}

void PlayerGUI::paint(juce::Graphics& g)
{

    g.fillAll(juce::Colour(0xff44444E));

    auto& lookAndFeel = juce::LookAndFeel::getDefaultLookAndFeel();

    // Set the default colours for all TextButtons
    lookAndFeel.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff450693));
    // Set the default colours for all Sliders
    lookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colour(0xff450693));
    lookAndFeel.setColour(juce::Slider::trackColourId, juce::Colour(0xffFFC400));
    lookAndFeel.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffFFC400));
    loopSlider.setColour(juce::Slider::trackColourId, juce::Colours::grey);
    loopSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffFFC400));

}

void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::blueviolet); // Highlight the selected track
    }

    // Get the track title from the audio engine
    auto title = playerAudio.getTrackTitle(rowNumber);

    g.setColour(juce::Colours::white);
    g.drawText(title, 5, 0, width - 10, height, juce::Justification::centredLeft);
}


void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
     {

         juce::FileChooser chooser("Select audio files...",
             juce::File{},
             "*.wav;*.mp3");
     
         fileChooser = std::make_unique<juce::FileChooser>(
             "Select an audio file...",
             juce::File{},
             "*.wav;*.mp3");
            
         fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode|juce::FileBrowserComponent::canSelectFiles,
             [this](const juce::FileChooser& fc)
             {

                 auto files = fc.getResults();

                 if (files.size() > 0)
                 {
                     // We have files! Add them to the audio engine
                     playerAudio.addFilesToPlaylist(files);

                     // Update the list box to show the new tracks
                     playlistBox.updateContent();

                     // Optional: play the first track in the new list
                     playerAudio.playTrack(0);
                 }
             });
     }
            
    muteButton.onClick = [this]()
        {
            if (muteButton.getToggleState())
                playerAudio.setGain(0);
            else
                playerAudio.setGain((float)volumeSlider.getValue());
        };
    
    if (button == &restartButton)
    {
        playerAudio.play();
    }
    
    if (button == &stopButton)
    {
        playerAudio.stop();
        playerAudio.setPosition(0.0);
    }
    
    if (button == &pauseButton) {
        playerAudio.stop();
    }
    if (button == &playButton) {
    
        playerAudio.play();
    }
    if (button == &goToStartButton) {
        playerAudio.setPosition(0.0);
    }
    if (button == &goToEndButton) {
        playerAudio.setPosition(playerAudio.getLength()-5);
    }
    
}


void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider && !muteButton.getToggleState() ) 
    {
        playerAudio.setGain((float)slider->getValue());
    }
    if (slider == &speedSlider)
    {
        playerAudio.setResamplingRatio((float)slider->getValue());
    }
    if (slider == &positionSlider)
    {
        playerAudio.setPosition(slider->getValue());
    }
}

void PlayerGUI::timerCallback()
{
    bool isPlaying = playerAudio.isPlaying();
    if (wasPlaying && !isPlaying)
    {
        playerAudio.playNextTrack();
    }
    // Update the flag for the next timer tick
    wasPlaying = isPlaying;
    if (playerAudio.getLength() > 0 && playerAudio.getLength()!=positionSlider.getMaximum()+0.1)
    {
        // A new file has been detected! Configure the slider now.
        positionSlider.setRange(0.0, playerAudio.getLength()-0.1);
        positionSlider.setValue(0.0);
        positionSlider.setEnabled(true);
        loopSlider.setRange(0.0, playerAudio.getLength());
        loopSlider.setMinAndMaxValues(0.0, playerAudio.getLength()-0.1);
        loopSlider.setEnabled(true);
    }


    // Check if the transport source is playing
    if (isPlaying)
    {
        if (loopButton.getToggleState()) {
            if(playerAudio.getPosition() >= loopSlider.getMaxValue()-0.1)
                playerAudio.setPosition(loopSlider.getMinValue());
            if(loopSlider.getMinValue() > playerAudio.getPosition())
                playerAudio.setPosition(loopSlider.getMinValue());
        }


        double currentPosition = playerAudio.getPosition();

        // Update the slider's position.
        // The 'dontSendNotification' is CRITICAL to prevent a feedback loop where
        // this update triggers sliderValueChanged, which then tries to set the position again.
        positionSlider.setValue(currentPosition, juce::dontSendNotification);

        // Format the time as MM:SS
        int minutes = static_cast<int>(currentPosition) / 60;
        int seconds = static_cast<int>(currentPosition) % 60;
        int totalMinutes = static_cast<int>(playerAudio.getLength()) / 60;
        int totalSeconds = static_cast<int>(playerAudio.getLength()) % 60;
        juce::String timeString = juce::String::formatted("%02d:%02d/%02d:%02d", minutes, seconds,totalMinutes,totalSeconds);
        metadataLabel.setText(playerAudio.getMeta(), juce::dontSendNotification);
        positionLabel.setText(timeString, juce::dontSendNotification);
    }


}

int PlayerGUI::getNumRows()
{
    return playerAudio.getNumTracks();
}

void PlayerGUI::listBoxItemClicked(int rowNumber, const juce::MouseEvent& e)
{
    playerAudio.playTrack(rowNumber);

}