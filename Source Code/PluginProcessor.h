/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Mu45FilterCalc/Mu45FilterCalc.h"
#include "StkLite-4.6.2/BiQuad.h"
#include "Mu45LFO/Mu45LFO.h"

//==============================================================================
/**
*/
class WobblerAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    WobblerAudioProcessor();
    ~WobblerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    juce::AudioVisualiserComponent Visualizer;
    float BPM;
    juce::TextEditor BPMLabel;
    juce::TextButton SwitchRateChoice;
    juce::Slider RateSlider;
    juce::Slider RateChoiceSlider;
    juce::Label RateLabel;
    juce::Label RateChoiceLabel;
    juce::Label RateNoteLabel;
    bool Hz;
    bool SliderHasChanged;
    juce::AudioParameterChoice* RateChoices;

private:
    juce::AudioParameterFloat* Mix;
    juce::AudioParameterFloat* attack;
    juce::AudioParameterFloat* speed;
    juce::AudioParameterFloat* rate;
    juce::AudioParameterFloat* LPfreq;
    juce::AudioParameterChoice* HzOrNote;
    float mWetGain, mDryGain;
    stk::BiQuad filterL, filterR;
    Mu45LFO mLFOL, mLFOR;
    float mFs;
    void calcAlgorithmParams(), calcGains(float MixPerc);
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WobblerAudioProcessor)
};
