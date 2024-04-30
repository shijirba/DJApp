/*
  ==============================================================================

    DeckGUI.cpp
    Created: 31 Aug 2021 1:40:23pm
    Author:  SHIJIRBA

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"


//==============================================================================
DeckGUI::DeckGUI(int _id,
    DJAudioPlayer* _player,
    juce::AudioFormatManager& formatManager,
    juce::AudioThumbnailCache& thumbCache
) : player(_player),
id(_id),
waveformDisplay(id, formatManager, thumbCache)
{
    // add all components and make visible
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    /*loadImage = juce::ImageCache::getFromMemory(BinaryData::load_png, BinaryData::load_pngSize);
    loadButton.setImages(true, true, true, loadImage, 0.5f, {}, loadImage, 0.5f, {}, loadImage, 0.5f, {});*/

    addAndMakeVisible(loadButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(posLabel);
    addAndMakeVisible(waveformDisplay);

    // add listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
 
    //configure volume slider and label
    double volDefaultValue = 0.5;
    volSlider.setRange(0.0, 1.0);
    volSlider.setNumDecimalPlacesToDisplay(2);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, volSlider.getTextBoxHeight());
    volSlider.setValue(volDefaultValue);
    volSlider.setSkewFactorFromMidPoint(volDefaultValue);
    volSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    volSlider.setTextValueSuffix(" Volume");
    volLabel.attachToComponent(&volSlider, true);

    //configure speed slider and label
    double speedDefaultValue = 1.0;
    speedSlider.setRange(0.25, 4.0); //reaches breakpoint if sliderValue == 0
    speedSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, speedSlider.getTextBoxHeight());
    speedSlider.setValue(speedDefaultValue);
    speedSlider.setSkewFactorFromMidPoint(speedDefaultValue);
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    speedSlider.setTextValueSuffix(" Speed");
    speedLabel.attachToComponent(&speedSlider, true);

    //configure position slider and label
    posSlider.setRange(0.0, 1.0);
    posSlider.setNumDecimalPlacesToDisplay(2);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true,100, posSlider.getTextBoxHeight());
    posSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    posSlider.setTextValueSuffix(" Position");
    posLabel.attachToComponent(&posSlider, true);

    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint(juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.
       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

void DeckGUI::resized()
{
    /*This method is where you should set the bounds of any child
    components that your component contains..*/
       
    //                   x start, y start, width, height
    playButton.setBounds(0, 0, getWidth() / 3, getHeight() / 8);
    stopButton.setBounds(getWidth() / 3, 0, getWidth() / 3, getHeight() / 8);
    loadButton.setBounds(2 * getWidth() / 3, 0, getWidth() / 3, getHeight() / 8);
    volLabel.setBounds(0, getHeight() / 8, getWidth() / 3, 3 * (getHeight() / 8));
    posLabel.setBounds(2 * getWidth() / 3, getHeight() / 8, getWidth() / 3, 3 * (getHeight() / 8));
    speedLabel.setBounds(getWidth() / 3, getHeight() / 8, getWidth() / 3, 3 * (getHeight() / 8));
    volSlider.setBounds(0, getHeight() / 8, getWidth() / 3, 3 * (getHeight() / 8));
    speedSlider.setBounds(getWidth() / 3, getHeight() / 8, getWidth() / 3, 3 * (getHeight() / 8));
    posSlider.setBounds(2 * getWidth() / 3, getHeight() / 8, getWidth() / 3, 3 * (getHeight() / 8));
    waveformDisplay.setBounds(0, 4 * getHeight() / 8, getWidth(), 4 * getHeight() / 8);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        player->play();
    }
    if (button == &stopButton)
    {
        player->stop();
    }
    if (button == &loadButton)
    {
        juce::FileChooser chooser{ "Select a file" };
        if (chooser.browseForFileToOpen())
        {
            loadFile(juce::URL{ chooser.getResult() });
        }
    }
}


void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
     return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    if (files.size() == 1)
    {
        player->loadURL(juce::URL{ juce::File{files[0]} });
    }
}

void DeckGUI::loadFile(juce::URL audioURL)
{
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}

void DeckGUI::timerCallback()
{
    if (player->getPositionRelative() > 0)
    {
        waveformDisplay.setPositionRelative(player->getPositionRelative());
    }
}
