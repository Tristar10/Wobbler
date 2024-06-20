/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WobblerAudioProcessor::WobblerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), Visualizer(1)
#endif
{
    addParameter(Mix = new juce::AudioParameterFloat("Mix", // parameterID,
            "Mix", // parameterName,
            0.0f, // minValue,
            100.0f, // maxValue,
            100.0f)); // defaultValue
    addParameter(attack = new juce::AudioParameterFloat("Attack", // parameterID,
        "Attack", // parameterName,
        0.0f, // minValue,
        100.0f, // maxValue,
        50.0f)); // defaultValue
    addParameter(speed = new juce::AudioParameterFloat("Speed", // parameterID,
        "Speed", // parameterName,
        1.0f, // minValue,
        100.0f, // maxValue,
        10.0f)); // defaultValue
    addParameter(rate = new juce::AudioParameterFloat("Rate (Hz)", // parameterID,
        "Rate (Hz)", // parameterName,
        0.1f, // minValue,
        20.0f, // maxValue,
        5.0f)); // defaultValue
    addParameter(LPfreq = new juce::AudioParameterFloat("High Cut (Hz)", // parameterID,
        "High Cut (Hz)", // parameterName,
        30.0f, // minValue,
        22000.0f, // maxValue,
        5000.0f)); // defaultValue
    addParameter(RateChoices = new juce::AudioParameterChoice ("Rate Choices", "Rate Choices", { "1/2", "1/2T", "1/4", "1/4T", "1/8", "1/8T", "1/16", "1/16T", "1/32", "1/32T" }, 2));
    addParameter(HzOrNote = new juce::AudioParameterChoice("Rate Reference", "Rate Reference", {"Hz", "Note"}, 0));
    Visualizer.setRepaintRate(1.1);
    Hz = true;
}

void WobblerAudioProcessor::calcGains(float MixPerc)
{
    // Ensure mix is within the valid range [0, 100]
    float mix = std::max(0.0f, std::min(100.0f, MixPerc));
    // Convert mix percentage to a scale factor in the range [0, 1]
    float mixFactor = mix / 100.0f;
    // Calculate dry and wet gains to maintain constant perceived volume
    mDryGain = sqrt(1.0f - mixFactor);
    mWetGain = sqrt(mixFactor);
}

WobblerAudioProcessor::~WobblerAudioProcessor()
{
}

//==============================================================================
const juce::String WobblerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WobblerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WobblerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WobblerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WobblerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WobblerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WobblerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WobblerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WobblerAudioProcessor::getProgramName (int index)
{
    return {};
}

void WobblerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WobblerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mFs = sampleRate;
    calcAlgorithmParams();
}

void WobblerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WobblerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WobblerAudioProcessor::calcAlgorithmParams() {
    SwitchRateChoice.onClick = [&]()
    {
        if(HzOrNote->getCurrentValueAsText().compare("Hz") == 0){
            HzOrNote->operator=(1);
        }
        else{
            HzOrNote->operator=(0);
        }
    };
    if(BPMLabel.getText().toStdString() != ""){
        BPM = stof(BPMLabel.getText().toStdString());
    }
    mLFOL.updateParams(attack->get(), speed->get(), rate->get(), mFs);
    mLFOR.updateParams(attack->get(), speed->get(), rate->get(), mFs);
    if(HzOrNote->getCurrentValueAsText().compare("Hz") == 0){
        mLFOL.setFreq(rate->get(), mFs);
        mLFOR.setFreq(rate->get(), mFs);
        RateSlider.setValue(rate->get());
    }
    else{
        std::string rateChoice = RateChoices->getCurrentChoiceName().toStdString();
        
        if(rateChoice == "1/2"){
            mLFOL.setFreq(BPM/120.0, mFs);
            mLFOR.setFreq(BPM/120.0, mFs);
        }
        else if(rateChoice == "1/2T"){
            mLFOL.setFreq(BPM/80.0, mFs);
            mLFOR.setFreq(BPM/80.0, mFs);
        }
        else if(rateChoice == "1/4"){
            mLFOL.setFreq(BPM/60.0, mFs);
            mLFOR.setFreq(BPM/60.0, mFs);
        }
        else if(rateChoice == "1/4T"){
            mLFOL.setFreq(BPM/40.0, mFs);
            mLFOR.setFreq(BPM/40.0, mFs);
        }
        else if(rateChoice == "1/8"){
            mLFOL.setFreq(BPM/30.0, mFs);
            mLFOR.setFreq(BPM/30.0, mFs);
        }
        else if(rateChoice == "1/8T"){
            mLFOL.setFreq(BPM/20.0, mFs);
            mLFOR.setFreq(BPM/20.0, mFs);
        }
        else if(rateChoice == "1/16"){
            mLFOL.setFreq(BPM/15.0, mFs);
            mLFOR.setFreq(BPM/15.0, mFs);
        }
        else if(rateChoice == "1/16T"){
            mLFOL.setFreq(BPM/10.0, mFs);
            mLFOR.setFreq(BPM/10.0, mFs);
        }
        else if(rateChoice == "1/32"){
            mLFOL.setFreq(BPM/7.5, mFs);
            mLFOR.setFreq(BPM/7.5, mFs);
        }
        else if(rateChoice == "1/32T"){
            mLFOL.setFreq(BPM/5.0, mFs);
            mLFOR.setFreq(BPM/5.0, mFs);
        }
        RateNoteLabel.setText(rateChoice, juce::dontSendNotification);
        RateChoiceSlider.setValue(RateChoices->getIndex());
    }
    calcGains(Mix->get());
    float coeffs[5];
    Mu45FilterCalc::calcCoeffsLPF(coeffs, LPfreq->get(), 0.707, mFs);
    filterL.setCoefficients(coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);
    filterR.setCoefficients(coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);
    Visualizer.setBufferSize((1/(rate->get()))*512);
}

void WobblerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
//    if(auto* playhead = getPlayHead()){
//        juce::AudioPlayHead::CurrentPositionInfo info;
//        playHead->getCurrentPosition(info);
//    }
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels  = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples());
    auto* channelDataR = buffer.getWritePointer(1);
    auto* channelDataL = buffer.getWritePointer(0);
    calcAlgorithmParams();
    float lSamp, rSamp;
    const int numSamples = buffer.getNumSamples();
    // This version assumes we always have 2 channels
    calcAlgorithmParams();
    // The "inner loop"
    for (int i = 0; i < numSamples; ++i)
    {
        lSamp = channelDataL[i];
        rSamp = channelDataR[i];
        channelDataL[i] = lSamp * mDryGain + filterL.tick(lSamp) * mLFOL.tick() * mWetGain;
        channelDataR[i] = rSamp * mDryGain + filterR.tick(rSamp) * mLFOR.tick() * mWetGain;
    }
    Visualizer.pushBuffer(buffer);
}

//==============================================================================
bool WobblerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WobblerAudioProcessor::createEditor()
{
    return new WobblerAudioProcessorEditor (*this);
}

//==============================================================================
void WobblerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WobblerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WobblerAudioProcessor();
}
