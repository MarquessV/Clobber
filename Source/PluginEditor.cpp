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
}

void ClobberAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &inputGain)
        audioProcessor.inputGain = slider->getValue();
}
