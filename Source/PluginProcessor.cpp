/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

ClobberAudioProcessor::ClobberAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                          .withInput ("Input", juce::AudioChannelSet::stereo(), true)
#endif
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
}

ClobberAudioProcessor::~ClobberAudioProcessor() {}

//==============================================================================

const juce::String ClobberAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ClobberAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool ClobberAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool ClobberAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double ClobberAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ClobberAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
        // so this should be at least 1, even if you're not really implementing programs.
}

int ClobberAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ClobberAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ClobberAudioProcessor::getProgramName (int index)
{
    return {};
}

void ClobberAudioProcessor::changeProgramName (int index, const juce::String& newName) {}

//==============================================================================

void ClobberAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto spec = juce::dsp::ProcessSpec { getSampleRate(), static_cast<juce::uint32> (getBlockSize()), static_cast<juce::uint32> (getTotalNumInputChannels()) };
    mix.prepare (spec);
}

void ClobberAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ClobberAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ClobberAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    buffer.applyGain (inputGain);
    mix.pushDrySamples (buffer);
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            channelData[i] = softClip (channelData[i]);
        }
    }
    mix.mixWetSamples (buffer);
    buffer.applyGain (outputGain);
}

float ClobberAudioProcessor::softClip (float sample)
{
    juce::NormalisableRange<float> alphaRange { 1.50f, 15.0f };
    const auto normalisedDrive = driveRange.convertTo0to1 (drive);
    const auto alpha = alphaRange.convertFrom0to1 (normalisedDrive);
    return 2.0f / juce::MathConstants<float>::pi * atan (alpha * sample);
}

//==============================================================================

bool ClobberAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ClobberAudioProcessor::createEditor()
{
    return new ClobberAudioProcessorEditor (*this);
}

//==============================================================================

void ClobberAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ClobberAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ClobberAudioProcessor();
}
