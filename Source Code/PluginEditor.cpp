/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WobblerAudioProcessorEditor::WobblerAudioProcessorEditor (WobblerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 550);
    auto& params = processor.getParameters();
    
    juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(0);
    MixSlider.setBounds(465, 30, 110, 110);
    MixSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    MixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    MixSlider.setRange(audioParam->range.start, audioParam->range.end, 1);
    MixSlider.setValue(*audioParam);
    MixSlider.setColour(MixSlider.textBoxTextColourId, juce::Colours::black);
    MixSlider.addListener(this);
//    MixSlider.setColour(MixSlider.textBoxTextColourId, juce::Colours::white);
    MixSlider.setColour(MixSlider.rotarySliderOutlineColourId, juce::Colours::white);
    MixSlider.setColour(MixSlider.thumbColourId, juce::Colour(21, 28, 116));
    MixSlider.setColour(MixSlider.rotarySliderFillColourId, juce::Colour(21, 28, 116));
    addAndMakeVisible(MixSlider);
    
    juce::AudioParameterFloat* audioParam1 = (juce::AudioParameterFloat*)params.getUnchecked(1);
    AttackSlider.setBounds(25, 30, 110, 110);
    AttackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    AttackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    AttackSlider.setRange(audioParam1->range.start, audioParam1->range.end, 1);
    AttackSlider.setValue(*audioParam1);
    AttackSlider.setColour(AttackSlider.textBoxTextColourId, juce::Colours::black);
    AttackSlider.addListener(this);
//    AttackSlider.setColour(AttackSlider.textBoxTextColourId, juce::Colours::white);
    AttackSlider.setColour(AttackSlider.rotarySliderOutlineColourId, juce::Colours::white);
    AttackSlider.setColour(AttackSlider.thumbColourId, juce::Colour(21, 28, 116));
    AttackSlider.setColour(AttackSlider.rotarySliderFillColourId, juce::Colour(21, 28, 116));
    addAndMakeVisible(AttackSlider);
    
    juce::AudioParameterFloat* audioParam2 = (juce::AudioParameterFloat*)params.getUnchecked(2);
    SpeedSlider.setBounds(160, 30, 110, 110);
    SpeedSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    SpeedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    SpeedSlider.setRange(audioParam2->range.start, audioParam2->range.end, 1);
    SpeedSlider.setValue(*audioParam2);
    SpeedSlider.setColour(MixSlider.textBoxTextColourId, juce::Colours::black);
    SpeedSlider.addListener(this);
//    SpeedSlider.setColour(SpeedSlider.textBoxTextColourId, juce::Colours::white);
    SpeedSlider.setColour(SpeedSlider.rotarySliderOutlineColourId, juce::Colours::white);
    SpeedSlider.setColour(SpeedSlider.thumbColourId, juce::Colour(21, 28, 116));
    SpeedSlider.setColour(SpeedSlider.rotarySliderFillColourId, juce::Colour(21, 28, 116));
    addAndMakeVisible(SpeedSlider);
    
    juce::AudioParameterFloat* audioParam3 = (juce::AudioParameterFloat*)params.getUnchecked(3);
    audioProcessor.RateSlider.setBounds(110, 200, 110, 110);
    audioProcessor.RateSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    audioProcessor.RateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    audioProcessor.RateSlider.setRange(audioParam3->range.start, audioParam3->range.end, 0.1);
    audioProcessor.RateSlider.setValue(*audioParam3);
    audioProcessor.RateSlider.setColour(audioProcessor.RateSlider.textBoxTextColourId, juce::Colours::black);
    audioProcessor.RateSlider.addListener(this);
//    audioProcessor.RateSlider.setColour(audioProcessor.RateSlider.textBoxTextColourId, juce::Colours::white);
    audioProcessor.RateSlider.setColour(audioProcessor.RateSlider.rotarySliderOutlineColourId, juce::Colours::white);
    audioProcessor.RateSlider.setColour(audioProcessor.RateSlider.thumbColourId, juce::Colour(21, 28, 116));
    audioProcessor.RateSlider.setColour(audioProcessor.RateSlider.rotarySliderFillColourId, juce::Colour(21, 28, 116));
    addAndMakeVisible(audioProcessor.RateSlider);
    
    juce::AudioParameterFloat* audioParam4 = (juce::AudioParameterFloat*)params.getUnchecked(4);
    FCSlider.setBounds(295, 30, 110, 110);
    FCSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    FCSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    FCSlider.setRange(audioParam4->range.start, audioParam4->range.end, 1);
    FCSlider.setValue(*audioParam4);
    FCSlider.setColour(FCSlider.textBoxTextColourId, juce::Colours::black);
    FCSlider.addListener(this);
//    FCSlider.setColour(FCSlider.textBoxTextColourId, juce::Colours::white);
    FCSlider.setColour(FCSlider.rotarySliderOutlineColourId, juce::Colours::white);
    FCSlider.setColour(FCSlider.thumbColourId, juce::Colour(21, 28, 116));
    FCSlider.setColour(FCSlider.rotarySliderFillColourId, juce::Colour(21, 28, 116));
    addAndMakeVisible(FCSlider);
    
    audioProcessor.RateChoiceSlider.setBounds(385, 200, 90, 90);
    audioProcessor.RateChoiceSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    audioProcessor.RateChoiceSlider.setRange(0, 9, 1);
    audioProcessor.RateChoiceSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 0, 0);
    audioProcessor.RateChoiceSlider.setColour(audioProcessor.RateChoiceSlider.textBoxTextColourId, juce::Colours::black);
    audioProcessor.RateChoiceSlider.addListener(this);
//    audioProcessor.RateChoiceSlider.setColour(audioProcessor.RateChoiceSlider.textBoxTextColourId, juce::Colours::white);
    audioProcessor.RateChoiceSlider.setColour(audioProcessor.RateChoiceSlider.rotarySliderOutlineColourId, juce::Colours::white);
    audioProcessor.RateChoiceSlider.setColour(audioProcessor.RateChoiceSlider.thumbColourId, juce::Colour(21, 28, 116));
    audioProcessor.RateChoiceSlider.setColour(audioProcessor.RateChoiceSlider.rotarySliderFillColourId, juce::Colour(21, 28, 116));
    addAndMakeVisible(audioProcessor.RateChoiceSlider);
//
//    juce::AudioParameterFloat* audioParam6 = (juce::AudioParameterFloat*)params.getUnchecked(6);
    
    audioProcessor.Visualizer.setBounds(25, 375, 550, 150);
    addAndMakeVisible(audioProcessor.Visualizer);
    
    MixLabel.setText ("Mix(%)", juce::dontSendNotification);
    MixLabel.setJustificationType(juce::Justification::centred);
    MixLabel.attachToComponent(&MixSlider, false);
    MixLabel.setColour(MixLabel.textColourId, juce::Colours::black);
    addAndMakeVisible (MixLabel);
    
    AttackLabel.setText ("Attack", juce::dontSendNotification);
    AttackLabel.setJustificationType(juce::Justification::centred);
    AttackLabel.attachToComponent(&AttackSlider, false);
    AttackLabel.setColour(AttackLabel.textColourId, juce::Colours::black);
    addAndMakeVisible(AttackLabel);
    
    SpeedLabel.setText ("Speed", juce::dontSendNotification);
    SpeedLabel.setJustificationType(juce::Justification::centred);
    SpeedLabel.attachToComponent(&SpeedSlider, false);
    SpeedLabel.setColour(SpeedLabel.textColourId, juce::Colours::black);
    addAndMakeVisible (SpeedLabel);
    
    audioProcessor.RateLabel.setText ("Rate(Hz)", juce::dontSendNotification);
    audioProcessor.RateLabel.setJustificationType(juce::Justification::centred);
    audioProcessor.RateLabel.attachToComponent(&audioProcessor.RateSlider, false);
    audioProcessor.RateLabel.setColour(audioProcessor.RateLabel.textColourId, juce::Colours::black);
    addAndMakeVisible(audioProcessor.RateLabel);
    
    audioProcessor.RateChoiceLabel.setText ("Rate(Note)", juce::dontSendNotification);
    audioProcessor.RateChoiceLabel.setJustificationType(juce::Justification::centred);
    audioProcessor.RateChoiceLabel.attachToComponent(&audioProcessor.RateChoiceSlider, false);
    audioProcessor.RateChoiceLabel.setColour(audioProcessor.RateChoiceLabel.textColourId, juce::Colours::black);
    addAndMakeVisible (audioProcessor.RateChoiceLabel);
    
    FCLabel.setText ("HiCut(Hz)", juce::dontSendNotification);
    FCLabel.setJustificationType(juce::Justification::centred);
    FCLabel.attachToComponent(&FCSlider, false);
    FCLabel.setColour(FCLabel.textColourId, juce::Colours::black);
    addAndMakeVisible (FCLabel);
    
    audioProcessor.RateNoteLabel.setJustificationType(juce::Justification::centred);
    audioProcessor.RateNoteLabel.setBounds(395, 290, 70, 20);
    audioProcessor.RateNoteLabel.setColour(audioProcessor.RateNoteLabel.textColourId, juce::Colours::black);
    addAndMakeVisible(audioProcessor.RateNoteLabel);
    
    audioProcessor.BPMLabel.setBounds(260, 225, 80, 25);
    audioProcessor.BPMLabel.setColour(audioProcessor.BPMLabel.textColourId, juce::Colours::black);
    audioProcessor.BPMLabel.setColour(audioProcessor.BPMLabel.backgroundColourId, juce::Colours::grey);
    addAndMakeVisible(audioProcessor.BPMLabel);
    
    audioProcessor.SwitchRateChoice.setBounds(250, 270, 100, 25);
    audioProcessor.SwitchRateChoice.setButtonText("Apply Tempo");
    audioProcessor.SwitchRateChoice.setColour(audioProcessor.SwitchRateChoice.buttonColourId, juce::Colours::white);
    audioProcessor.SwitchRateChoice.setColour(audioProcessor.SwitchRateChoice.textColourOnId, juce::Colours::black);
    audioProcessor.SwitchRateChoice.setColour(audioProcessor.SwitchRateChoice.buttonOnColourId, juce::Colours::grey);
    audioProcessor.SwitchRateChoice.setColour(audioProcessor.SwitchRateChoice.textColourOffId, juce::Colours::black);
    addAndMakeVisible(audioProcessor.SwitchRateChoice);
    
}

void WobblerAudioProcessorEditor::sliderValueChanged(juce::Slider * slider)
{
    auto& params = processor.getParameters();
    if (slider == &MixSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(0);
        *audioParam = MixSlider.getValue(); // set the AudioParameter
    }
    else if (slider == &AttackSlider)
    {
        juce::AudioParameterFloat* audioParam1 = (juce::AudioParameterFloat*)params.getUnchecked(1);
        *audioParam1 = AttackSlider.getValue(); // set the param
    }
    else if (slider == &SpeedSlider)
    {
        juce::AudioParameterFloat* audioParam2 = (juce::AudioParameterFloat*)params.getUnchecked(2);
        *audioParam2 = SpeedSlider.getValue(); // set the param
    }
    else if (slider == &audioProcessor.RateSlider)
    {
        juce::AudioParameterFloat* audioParam3 = (juce::AudioParameterFloat*)params.getUnchecked(3);
        *audioParam3 = audioProcessor.RateSlider.getValue(); // set the param
    }
    else if (slider == &FCSlider)
    {
        juce::AudioParameterFloat* audioParam4 = (juce::AudioParameterFloat*)params.getUnchecked(4);
        *audioParam4 = FCSlider.getValue(); // set the param
    }
    else if (slider == &audioProcessor.RateChoiceSlider)
    {
        //audioProcessor.SliderHasChanged = true;
        audioProcessor.RateChoices->operator=(audioProcessor.RateChoiceSlider.getValue());
    }
}

WobblerAudioProcessorEditor::~WobblerAudioProcessorEditor()
{
}

//==============================================================================
void WobblerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    juce::Image background = juce::ImageCache::getFromMemory (BinaryData::BG_png, BinaryData::BG_pngSize);
    g.drawImageAt (background, 0, 0);
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void WobblerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
