#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class DynamicMultiEffectAudioProcessorEditor : public juce::AudioProcessorEditor,
    public juce::Timer,
    public juce::Slider::Listener {
public:
    DynamicMultiEffectAudioProcessorEditor(DynamicMultiEffectAudioProcessor&);
    ~DynamicMultiEffectAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    DynamicMultiEffectAudioProcessor& audioProcessor;
    juce::Slider distortionSlider, delaySlider, reverbSlider;
    juce::AudioBuffer<float> audioData; // Buffer for visualization

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DynamicMultiEffectAudioProcessorEditor)
};