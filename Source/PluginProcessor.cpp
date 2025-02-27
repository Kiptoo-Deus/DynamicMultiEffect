#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

DynamicMultiEffectAudioProcessor::DynamicMultiEffectAudioProcessor()
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
{
    gainParam = new juce::AudioParameterFloat("gain", "Gain", 0.0f, 10.0f, 1.0f);
    levelParam = new juce::AudioParameterFloat("level", "Level", 0.0f, 1.0f, 1.0f);
    addParameter(gainParam);
    addParameter(levelParam);
}

DynamicMultiEffectAudioProcessor::~DynamicMultiEffectAudioProcessor() {}

const juce::String DynamicMultiEffectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DynamicMultiEffectAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool DynamicMultiEffectAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool DynamicMultiEffectAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double DynamicMultiEffectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DynamicMultiEffectAudioProcessor::getNumPrograms()
{
    return 1;
}

int DynamicMultiEffectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DynamicMultiEffectAudioProcessor::setCurrentProgram(int /*index*/) {}

const juce::String DynamicMultiEffectAudioProcessor::getProgramName(int /*index*/)
{
    return {};
}

void DynamicMultiEffectAudioProcessor::changeProgramName(int /*index*/, const juce::String& /*newName*/) {}

void DynamicMultiEffectAudioProcessor::prepareToPlay(double newSampleRate, int /*samplesPerBlock*/)
{
    sampleRate = static_cast<float>(newSampleRate);
}

void DynamicMultiEffectAudioProcessor::releaseResources() {}

bool DynamicMultiEffectAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

void DynamicMultiEffectAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    float gain = *gainParam;
    float level = *levelParam;

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float input = channelData[sample] * gain;
            channelData[sample] = juce::jlimit(-1.0f, 1.0f, input) * level;
        }
    }
}

//==============================================================================

bool DynamicMultiEffectAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* DynamicMultiEffectAudioProcessor::createEditor()
{
    return new DynamicMultiEffectAudioProcessorEditor(*this);
}

void DynamicMultiEffectAudioProcessor::getStateInformation(juce::MemoryBlock& /*destData*/) {}

void DynamicMultiEffectAudioProcessor::setStateInformation(const void* /*data*/, int /*sizeInBytes*/) {}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DynamicMultiEffectAudioProcessor();
}
