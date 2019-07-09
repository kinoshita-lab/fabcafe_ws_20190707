#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce2.h>

#include "AudioSampleSnare.h"
#include "AudioSampleCh.h"
#include "AudioSampleBd01.h"

#include "Sequencer.h"

#include "MsTimer2.h"

// GUItool: begin automatically generated code

AudioOutputAnalogStereo  dacs1;          //xy=659,282


AudioSynthSimpleDrum drum1;
AudioPlayMemory bd909;
AudioMixer4 bdMixer;
AudioAmplifier drum_amp;

AudioEffectWaveshaper waveshaper;

AudioPlayMemory playMem1;
AudioEffectBitcrusher snareCrush;

AudioPlayMemory playMem2;
AudioEffectBitcrusher chCrush;

AudioSynthNoiseWhite noise1;
AudioEffectEnvelope noise1_envelope;


AudioMixer4              mixer1;
AudioAmplifier mixer1_out_amp;

AudioFilterStateVariable filter;

AudioMixer4 delayFeedbackMixer;
AudioEffectDelay masterDelay;
AudioAmplifier delay_feedback_amp;
//AudioEffectReverb reverb;

AudioConnection          patchCord10(drum1, 0, waveshaper, 0);
AudioConnection          patchCord11(waveshaper, 0, drum_amp, 0);
AudioConnection          patchCord12(drum_amp, 0, bdMixer, 0);
AudioConnection          patchCord13(bd909, 0, bdMixer, 1);
AudioConnection          patchCord14(bdMixer, 0, mixer1, 0);

AudioConnection          patchCord20(playMem1, 0, snareCrush, 0);
AudioConnection          patchCord21(snareCrush, 0, mixer1, 1);

AudioConnection          pathCord30(noise1, noise1_envelope);
AudioConnection          pathCord31(noise1_envelope, 0, mixer1, 2);

AudioConnection          pathCord40(playMem2, 0, chCrush, 0);
AudioConnection          pathCord41(chCrush, 0, mixer1, 3);

AudioConnection          patchCord42(mixer1, 0, mixer1_out_amp, 0);


AudioConnection          patchCord50(mixer1_out_amp, 0, delayFeedbackMixer, 0);
AudioConnection          patchCord51(delayFeedbackMixer, 0, masterDelay, 0);
AudioConnection          patchCord52(masterDelay, 0, delay_feedback_amp, 0);
AudioConnection          patchCord53(delay_feedback_amp, 0, delayFeedbackMixer, 1);
AudioConnection          patchCord54(delayFeedbackMixer, 0, filter, 0);


AudioConnection          patchCord60(filter, 0, dacs1, 0);
AudioConnection          patchCord61(filter, 0, dacs1, 1);


//AudioConnection          patchCord2(sine2, 0, dacs1, 1);
// GUItool: end automatically generated code


constexpr int LED_PIN = 13;
constexpr int SW_1_PIN = 2;
constexpr int SW_2_PIN = 3;
constexpr int SW_3_PIN = 4;
constexpr int SW_4_PIN = 5;
constexpr int SW_5_PIN = 6;
constexpr int SW_6_PIN = 7;

constexpr int EXTERNAL_SYNC_OUT = 33;

Bounce sw1 = Bounce();
Bounce sw2 = Bounce();
Bounce sw3 = Bounce();
Bounce sw4 = Bounce();
Bounce sw5 = Bounce();
Bounce sw6 = Bounce();

float WAVESHAPE_EXAMPLE[17] = {
  -0.588,
  -0.579,
  -0.549,
  -0.488,
  -0.396,
  -0.320,
  -0.228,
  -0.122,
  0,
  0.122,
  0.228,
  0.320,
  0.396,
  0.488,
  0.549,
  0.579,
  0.588
};


bbb::Sequencer sequencer;

int currentPart;

struct Bpm
{
    float bpm;    
    uint32_t count;
    uint32_t countMax;
    uint8_t posIn16th;
    bool tickOperation;
    Bpm() : bpm(120.0), count(0), countMax(0xFFFFFFFF), posIn16th(0){}

    auto tick1ms() -> void {
        tickOperation = false;
        count++;

        if (count >= 13) {
            digitalWrite(EXTERNAL_SYNC_OUT, LOW);
        }

        if (count >= countMax) {
            count = 0;
            digitalWrite(EXTERNAL_SYNC_OUT, HIGH);
            posIn16th++;

            if (posIn16th >= 4) {
                posIn16th = 0;
            }

            const auto beatLedStatus = posIn16th == 0 ? HIGH : LOW;
            digitalWrite(LED_PIN, beatLedStatus);

            sequencer.nextStep();

            // dasai
            if (sequencer.noteOnStep(0)) {
                drum1.noteOn();
                bd909.play(AudioSampleBd01);
            }

            if (sequencer.noteOnStep(1)) {
                 playMem1.play(AudioSampleSnare);
            }

            if (sequencer.noteOnStep(2)) {
                noise1_envelope.noteOn();
            }

            if (sequencer.noteOnStep(3)) {
                 playMem2.play(AudioSampleCh);
            }
        }
    };
};

Bpm bpm;

auto timer2Interrupt() -> void {
    bpm.tickOperation = true;
}

auto changeBpm(const auto newBpm) -> void {
	auto tmp = 60 * 1000.0 / newBpm;
	tmp /= 4.0;
	bpm.countMax = (uint32_t)tmp;

    // compute delay time for 3/8th + a little bit longer
    auto delayTime = bpm.countMax * 2 * 3.02; // [ms];
    masterDelay.delay(0, delayTime);
}

auto setup() -> void {
    pinMode(LED_PIN, OUTPUT);
    pinMode(EXTERNAL_SYNC_OUT, OUTPUT);
    pinMode(SW_1_PIN, INPUT_PULLUP);
    pinMode(SW_2_PIN, INPUT_PULLUP);
    pinMode(SW_3_PIN, INPUT_PULLUP);
    pinMode(SW_4_PIN, INPUT_PULLUP);
    pinMode(SW_5_PIN, INPUT_PULLUP);
    pinMode(SW_6_PIN, INPUT_PULLUP);

    drum1.frequency(60);
    drum1.length(3000);
    drum1.secondMix(0.2);
    drum1.pitchMod(0.53);
    waveshaper.shape(WAVESHAPE_EXAMPLE, 17);
    drum_amp.gain(0.5);

    snareCrush.bits(16);
    snareCrush.sampleRate(44100);

    chCrush.bits(16);
    chCrush.sampleRate(44100);    

    noise1.amplitude(0.2);
    noise1_envelope.attack(1);
    noise1_envelope.decay(100);
    noise1_envelope.sustain(0);

    mixer1_out_amp.gain(0.5);

    delay_feedback_amp.gain(0.0);

    filter.resonance(1.2);

    sw1.attach(SW_1_PIN);
    sw1.interval(10);

    sw2.attach(SW_2_PIN);
    sw2.interval(10);

    sw3.attach(SW_3_PIN);
    sw3.interval(10);

    sw4.attach(SW_4_PIN);
    sw4.interval(10);

    sw5.attach(SW_5_PIN);
    sw5.interval(10);

    sw6.attach(SW_6_PIN);
    sw6.interval(10);


    MsTimer2::set(1, timer2Interrupt);
    MsTimer2::start();

    changeBpm(120.0);

    AudioMemory(800);

    AudioNoInterrupts();

    Serial.begin(115200);

    AudioInterrupts();
}

auto mappedBpm = 1200;


auto partTweak1(const auto parameter1) -> void {
    switch (currentPart) {
        case 0: // kick
        drum1.frequency(parameter1);
        break;
        case 1: // snare
        snareCrush.bits(map(parameter1, 0, 127, 1, 16));
        break;
        case 2: // noise
        noise1_envelope.decay(map(parameter1, 0, 127, 10, 500));
        break;
        case 3: // ch
        chCrush.bits(map(parameter1, 0, 127, 1, 16));
        break;
        default: 
        break;
    }
}

auto partTweak2(const auto parameter2) -> void {

    switch (currentPart) {
        case 0: {// kick 
            const int offsetRemoved = parameter2 - 64;
            const float tweakValue = 0.5 + (offsetRemoved / (127.0 * 10));
            drum1.pitchMod(tweakValue);
            break;
        }
        case 1: // snare
        snareCrush.sampleRate(map(parameter2, 0, 127, 1000, 44100));
        break;
        case 2: {// noise
        const float gain = parameter2 / (127.0 * 2);
        noise1.amplitude(gain);
        }
        break;
        case 3: // ch
        chCrush.sampleRate(map(parameter2, 0, 127, 1000, 44100));
        break;
    }
}

auto lastParam1 = 0;
auto lastParam2 = 0;
bool muteMode = false;

auto loop() -> void {
    // update switches
    sw1.update();
    sw2.update();
    sw3.update();
    sw4.update();
    sw5.update();
    sw6.update();

    if (sw1.fell()) {
        currentPart = 0;
        if (muteMode) {
            sequencer.toggleMute(0);            
        } else {
            drum1.noteOn();
            bd909.play(AudioSampleBd01);
            sequencer.rec(0, (float)bpm.count / bpm.countMax);
        }
    }

    if (sw2.fell()) {
        currentPart = 1;
        if (muteMode) {
            sequencer.toggleMute(1);
        } else {
            playMem1.play(AudioSampleSnare);
            sequencer.rec(1, (float)bpm.count / bpm.countMax);
        }
    }

    if (sw3.fell()) {
        currentPart = 2;
        if (muteMode) {
            sequencer.toggleMute(2);
        } else {
            noise1_envelope.noteOn();
            sequencer.rec(2, (float)bpm.count / bpm.countMax);
        }
    }

    if (sw4.fell()) {
        currentPart = 3;
        if (muteMode) {
            sequencer.toggleMute(3);
        } else {
            playMem2.play(AudioSampleCh);
            sequencer.rec(3, (float)bpm.count / bpm.countMax);
        }
    }

    if (sw5.fell()) {
        sequencer.clear(currentPart);
    }

    if (sw6.fell()) {
        muteMode = true;
        Serial.println("mute on");
    }
    if (sw6.rose()) {
        muteMode = false;
        Serial.println("mute off");
    }

    // update bpm
    const auto bpmAdValue = analogRead(0) >> 4 << 4;
    const auto mappedValue = map(bpmAdValue, 0, 1023, 200, 3000);

    if (mappedBpm != mappedValue) {
        mappedBpm = mappedValue;
        changeBpm(mappedValue / 10.0);
    }

    // update params
    // for part tweaking
    const auto param1 = analogRead(1) >> 3;
    const auto param2 = analogRead(2) >> 3;

    const int delta1 = abs(lastParam1 - param1);
    const int delta2 = abs(lastParam2 - param2);

    if (delta1 >= 2) {
        partTweak1(param1);
        lastParam1 = param1;
    }

    if (delta2 >= 2) {
        partTweak2(param2);
        lastParam2 = param2;
    }

    // effects

    // delay
    const auto delayValue = analogRead(3) >> 2 << 2;
    const auto mappedDelayValue = map(delayValue, 0, 1023, 0, 10000);
    const auto floatDelayValue = mappedDelayValue / 10000.0;
    delay_feedback_amp.gain(floatDelayValue);

    // filter
    const auto filterValue = analogRead(4) >> 2 << 2;
    const auto mappedFilterValue = map(filterValue, 0, 1023, 0, 10000);
    const auto floatFilterValue = mappedFilterValue / 10000.0;
    const auto squaredFilterValue = floatFilterValue * floatFilterValue;
    const auto actualFilterFrequency = map(squaredFilterValue, 0.0, 1.0, 50, 15000);
    filter.frequency(actualFilterFrequency);

    if (bpm.tickOperation) {
        bpm.tick1ms();
    }
}
