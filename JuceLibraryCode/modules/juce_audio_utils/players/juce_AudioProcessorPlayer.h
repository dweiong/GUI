/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2013 - Raw Material Software Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#ifndef __JUCE_AUDIOPROCESSORPLAYER_JUCEHEADER__
#define __JUCE_AUDIOPROCESSORPLAYER_JUCEHEADER__

#include "../../juce_audio_processors/processors/juce_AudioProcessor.h"


//==============================================================================
/**
    An AudioIODeviceCallback object which streams audio through an AudioProcessor.

    To use one of these, just make it the callback used by your AudioIODevice, and
    give it a processor to use by calling setProcessor().

    It's also a MidiInputCallback, so you can connect it to both an audio and midi
    input to send both streams through the processor.

    @see AudioProcessor, AudioProcessorGraph
*/
class JUCE_API  AudioProcessorPlayer    : public AudioIODeviceCallback,
                                          public MidiInputCallback
{
public:
    //==============================================================================
    AudioProcessorPlayer();

    /** Destructor. */
    virtual ~AudioProcessorPlayer();

    //==============================================================================
    /** Sets the processor that should be played.

        The processor that is passed in will not be deleted or owned by this object.
        To stop anything playing, pass in 0 to this method.
    */
    void setProcessor (AudioProcessor* processorToPlay);

    /** Returns the current audio processor that is being played.
    */
    AudioProcessor* getCurrentProcessor() const                     { return processor; }

    /** Returns a midi message collector that you can pass midi messages to if you
        want them to be injected into the midi stream that is being sent to the
        processor.
    */
    MidiMessageCollector& getMidiMessageCollector()                 { return messageCollector; }

    //==============================================================================
    /** @internal */
    void audioDeviceIOCallback (const float** inputChannelData,
                                int totalNumInputChannels,
                                float** outputChannelData,
                                int totalNumOutputChannels,
                                int numSamples);
    /** @internal */
    void audioDeviceAboutToStart (AudioIODevice*);
    /** @internal */
    void audioDeviceStopped();
    /** @internal */
    void handleIncomingMidiMessage (MidiInput*, const MidiMessage&);

private:
    //==============================================================================
    AudioProcessor* processor;
    CriticalSection lock;
    double sampleRate;
    int blockSize;
    bool isPrepared;

    int numInputChans, numOutputChans;
    HeapBlock<float*> channels;
    AudioSampleBuffer tempBuffer;

    MidiBuffer incomingMidi;
    MidiMessageCollector messageCollector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessorPlayer)
};


#endif   // __JUCE_AUDIOPROCESSORPLAYER_JUCEHEADER__
