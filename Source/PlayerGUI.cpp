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
    //volumeSlider.setSliderStyle(juce::Slider::Rotary);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
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
    
    
    
    pauseButton.setBounds(20, 4*y, 80, 40);
    playButton.setBounds(120, 4*y, 80, 40);
    
}

void PlayerGUI::paint(juce::Graphics& g)
{

    g.fillAll(juce::Colour(0xff44444E));

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
                 auto file = fc.getResult();
                 if (file.existsAsFile())
                 {
                     playerAudio.loadFile(file);
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
    
    loopButton.onClick = [this]() {
        playerAudio.loop(loopButton.getToggleState());
        };
}


void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider && !muteButton.getToggleState() ) 
    {
        playerAudio.setGain((float)slider->getValue());
    }
}

