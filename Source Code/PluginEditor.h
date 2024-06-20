/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class WobblerAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    WobblerAudioProcessorEditor (WobblerAudioProcessor&);
    ~WobblerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WobblerAudioProcessor& audioProcessor;
    juce::Slider FCSlider;
    juce::Slider MixSlider;
    juce::Slider AttackSlider;
    juce::Slider SpeedSlider;
    juce::Label FCLabel;
    juce::Label MixLabel;
    juce::Label AttackLabel;
    juce::Label SpeedLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WobblerAudioProcessorEditor)
};
