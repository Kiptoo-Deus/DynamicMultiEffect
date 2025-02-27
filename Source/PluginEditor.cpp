#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DynamicMultiEffectAudioProcessorEditor::DynamicMultiEffectAudioProcessorEditor(DynamicMultiEffectAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
    addAndMakeVisible(distortionSlider);
    distortionSlider.setRange(0.0, 10.0);
    distortionSlider.addListener(this);

    addAndMakeVisible(delaySlider);
    delaySlider.setRange(0.0, 1.0);
    delaySlider.addListener(this);

    addAndMakeVisible(reverbSlider);
    reverbSlider.setRange(0.0, 1.0);
    reverbSlider.addListener(this);

    audioData.setSize(2, 1024); // Match processor buffer size
    audioData.clear();

    startTimerHz(30);
    setSize(600, 400);
}

DynamicMultiEffectAudioProcessorEditor::~DynamicMultiEffectAudioProcessorEditor() {}

//==============================================================================
void DynamicMultiEffectAudioProcessorEditor::paint(juce::Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);

    // Draw stereo waveform (left and right channels)
    auto width = getWidth();
    auto height = getHeight() / 2;
    juce::Path leftWaveform, rightWaveform;
    leftWaveform.startNewSubPath(0, height / 2);
    rightWaveform.startNewSubPath(0, height * 1.5f);

    int numSamples = audioData.getNumSamples();
    for (int i = 0; i < numSamples; ++i) {
        float x = juce::jmap(float(i), 0.0f, float(numSamples), 0.0f, float(width));
        float leftY = juce::jmap(audioData.getSample(0, i), -1.0f, 1.0f, float(height), 0.0f);
        float rightY = juce::jmap(audioData.getSample(1, i), -1.0f, 1.0f, float(height * 2), float(height));
        leftWaveform.lineTo(x, leftY);
        rightWaveform.lineTo(x, rightY);
    }

    g.strokePath(leftWaveform, juce::PathStrokeType(1.0f));
    g.strokePath(rightWaveform, juce::PathStrokeType(1.0f));
}

void DynamicMultiEffectAudioProcessorEditor::resized() {
    distortionSlider.setBounds(10, 10, 150, 20);
    delaySlider.setBounds(10, 40, 150, 20);
    reverbSlider.setBounds(10, 70, 150, 20);
}

void DynamicMultiEffectAudioProcessorEditor::timerCallback() {
    audioProcessor.getAudioDataForVisualization(audioData);
    repaint();
}

void DynamicMultiEffectAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    if (slider == &distortionSlider)
        audioProcessor.distortionGain->setValueNotifyingHost(slider->getValue() / 10.0f);
    else if (slider == &delaySlider)
        audioProcessor.delayTime->setValueNotifyingHost(slider->getValue());
    else if (slider == &reverbSlider)
        audioProcessor.reverbSize->setValueNotifyingHost(slider->getValue());
}