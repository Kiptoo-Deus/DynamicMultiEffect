#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DynamicMultiEffectAudioProcessor::DynamicMultiEffectAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo())
        .withOutput("Output", juce::AudioChannelSet::stereo()))
{
    addParameter(distortionGain = new juce::AudioParameterFloat("distGain", "Distortion Gain", 0.0f, 10.0f, 1.0f));
    addParameter(delayTime = new juce::AudioParameterFloat("delayTime", "Delay Time", 0.0f, 1.0f, 0.5f));
    addParameter(reverbSize = new juce::AudioParameterFloat("reverbSize", "Reverb Size", 0.0f, 1.0f, 0.5f));
}

DynamicMultiEffectAudioProcessor::~DynamicMultiEffectAudioProcessor() {}

//==============================================================================
void DynamicMultiEffectAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    juce::dsp::ProcessSpec spec{ sampleRate, static_cast<juce::uint32>(samplesPerBlock), 2 };
    effectChain.prepare(spec);

    // Set maximum delay to 2 seconds
    maxDelaySamples = 2.0f * static_cast<float>(sampleRate);
    effectChain.get<1>().setMaximumDelayInSamples(static_cast<int>(maxDelaySamples));

    // Initialize visualization buffer ( 1024 samples, stereo)
    visualizationBuffer.setSize(2, 1024);
    visualizationBuffer.clear();

    // Configure effects
    effectChain.get<0>().setGainLinear(*distortionGain);
    float delayInSamples = juce::jlimit<float>(0.0f, maxDelaySamples, *delayTime * static_cast<float>(sampleRate));
    effectChain.get<1>().setDelay(delayInSamples);
    juce::dsp::Reverb::Parameters reverbParams;
    reverbParams.roomSize = *reverbSize;
    effectChain.get<2>().setParameters(reverbParams);
}

void DynamicMultiEffectAudioProcessor::releaseResources() {
    visualizationBuffer.setSize(0, 0); // Clear buffer
}

bool DynamicMultiEffectAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}

void DynamicMultiEffectAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    juce::dsp::AudioBlock<float> block(buffer);

    effectChain.get<0>().setGainLinear(*distortionGain);
    float delayInSamples = juce::jlimit<float>(0.0f, maxDelaySamples, *delayTime * static_cast<float>(getSampleRate()));
    effectChain.get<1>().setDelay(delayInSamples);
    juce::dsp::Reverb::Parameters reverbParams;
    reverbParams.roomSize = *reverbSize;
    effectChain.get<2>().setParameters(reverbParams);

    effectChain.process(juce::dsp::ProcessContextReplacing<float>(block));

    // Buffer the processed audio for visualization
    {
        juce::ScopedLock lock(bufferLock);
        int numSamples = juce::jmin(buffer.getNumSamples(), visualizationBuffer.getNumSamples());
        for (int channel = 0; channel < 2; ++channel) {
            visualizationBuffer.copyFrom(channel, 0, buffer.getReadPointer(channel), numSamples);
        }
    }
}

void DynamicMultiEffectAudioProcessor::getAudioDataForVisualization(juce::AudioBuffer<float>& destBuffer) {
    juce::ScopedLock lock(bufferLock);
    int numSamples = juce::jmin(visualizationBuffer.getNumSamples(), destBuffer.getNumSamples());
    for (int channel = 0; channel < 2; ++channel) {
        destBuffer.copyFrom(channel, 0, visualizationBuffer.getReadPointer(channel), numSamples);
    }
}

//==============================================================================
juce::AudioProcessorEditor* DynamicMultiEffectAudioProcessor::createEditor() {
    return new DynamicMultiEffectAudioProcessorEditor(*this);
}

void DynamicMultiEffectAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {}

void DynamicMultiEffectAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new DynamicMultiEffectAudioProcessor();
}