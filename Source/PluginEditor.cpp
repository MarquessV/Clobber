/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
ClobberAudioProcessorEditor::ClobberAudioProcessorEditor (
    ClobberAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    inputGain.setSliderStyle (juce::Slider::LinearBarVertical);
    inputGain.setRange (0.0, 2.0, 0.01);
    inputGain.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    inputGain.setValue (1.0);
    inputGain.addListener (this);
    addAndMakeVisible (&inputGain);

    outputGain.setSliderStyle (juce::Slider::LinearBarVertical);
    outputGain.setRange (0.0, 2.0, 0.01);
    outputGain.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    outputGain.setValue (1.0);
    outputGain.addListener (this);
    addAndMakeVisible (&outputGain);

    drive.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    drive.setRange (0.0, 1.0, 0.001);
    drive.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    drive.setValue (0.0);
    drive.addListener (this);
    addAndMakeVisible (&drive);

    mix.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    mix.setRange (0.0, 1.0, 0.001);
    mix.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    mix.setValue (0.5);
    mix.addListener (this);
    addAndMakeVisible (&mix);
}

ClobberAudioProcessorEditor::~ClobberAudioProcessorEditor() {}

//==============================================================================
void ClobberAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a
    // solid colour)
    g.fillAll (
        getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void ClobberAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    inputGain.setBounds (20, 30, 20, getHeight() - 60);
    outputGain.setBounds (getWidth() - 40, 30, 20, getHeight() - 60);

    const int knobSize = 75;
    drive.setBounds (getWidth() / 2 - knobSize, getHeight() / 2, knobSize, knobSize);
    mix.setBounds (getWidth() / 2, getHeight() / 2, knobSize, knobSize);
}

void ClobberAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &inputGain)
        audioProcessor.inputGain = slider->getValue();
    if (slider == &outputGain)
        audioProcessor.inputGain = slider->getValue();
    if (slider == &drive)
        audioProcessor.drive = slider->getValue();
    if (slider == &mix)
        audioProcessor.mix.setWetMixProportion (slider->getValue());
}
