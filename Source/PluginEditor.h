/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


struct CustomRotarySlider : juce::Slider
{
    CustomRotarySlider() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
        juce::Slider::TextEntryBoxPosition::NoTextBox)
    {

    }
};

struct ResponseCurveComponent : juce::Component,
juce::AudioProcessorParameter::Listener,
juce::Timer
{
    ResponseCurveComponent(EQFedeAudioProcessor&);
    ~ResponseCurveComponent();

    void parameterValueChanged(int parameterIndex, float newValue) override;

    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override { }

    void timerCallback() override;

    void paint(juce::Graphics& g) override;
private:
    EQFedeAudioProcessor& audioProcessor;
    juce::Atomic<bool> parametersChanged{ false };
    MonoChain monoChain;
};

//==============================================================================
/**
*/
class EQFedeAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    EQFedeAudioProcessorEditor (EQFedeAudioProcessor&);
    ~EQFedeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    EQFedeAudioProcessor& audioProcessor;


    CustomRotarySlider peakFreqSlider, 
                       peakGainSlider, 
                       peakQualitySlider, 
                       lowCutFreqSlider, 
                       highCutFreqSlider, 
                       lowCutSlopeSlider, 
                       highCutSlopeSlider;

    //Pasar de nombres nemotecnicos a no nemotecnicos no me parece la mejor opcion, pero bueh
    using APVTS = juce::AudioProcessorValueTreeState;
    //attach de paraameter to the slider
    using Attachment = APVTS::SliderAttachment;

    Attachment peakFreqSliderAttachment,
                peakGainSliderAttachment,
                peakQualitySliderAttachment,
                lowCutFreqSliderAttachment,
                highCutFreqSliderAttachment,
                lowCutSlopeSliderAttachment,
                highCutSlopeSliderAttachment;

    ResponseCurveComponent responseCurveComponent;

    std::vector<juce::Component*> getComps();


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQFedeAudioProcessorEditor)
};
