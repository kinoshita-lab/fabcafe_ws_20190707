#include <Audio.h>
 #include <Wire.h>
 #include <SPI.h>
 #include <SD.h>
 #include <SerialFlash.h>
 #include <Bounce2.h> // // スイッチ用ライブラリ
 
 // GUItool: begin automatically generated code
 AudioSynthWaveformSine   sine1;          //xy=111,185
 AudioEffectDelay         delay1;         //xy=304,372
 AudioMixer4              delayFeedback_mixer;         //xy=373,204
 AudioAmplifier           delayFeedback_amp;           //xy=548,406
 AudioOutputAnalogStereo  dacs1;          //xy=645,199
 AudioConnection          patchCord1(sine1, 0, delayFeedback_mixer, 0);
 AudioConnection          patchCord2(delay1, 0, delayFeedback_amp, 0);
 AudioConnection          patchCord3(delayFeedback_mixer, 0, dacs1, 0);
 AudioConnection          patchCord4(delayFeedback_mixer, 0, dacs1, 1);
 AudioConnection          patchCord5(delayFeedback_mixer, delay1);
 AudioConnection          patchCord6(delayFeedback_amp, 0, delayFeedback_mixer, 1);
 // GUItool: end automatically generated code
 
 
 constexpr int SWITCH_1_PIN = 2; //2番ピンを使う
 Bounce sw1 = Bounce();
 
 void setup() {
     pinMode(SWITCH_1_PIN, INPUT_PULLUP);
     sw1.attach(SWITCH_1_PIN);
     sw1.interval(10);
     AudioMemory(800);
 
     AudioNoInterrupts();
 
     sine1.frequency(1000);
 
     delayFeedback_amp.gain(0.0); // 
 
     AudioInterrupts();
     Serial.begin(115200);
 
 }
 
 void loop() {
   sw1.update(); // スイッチの状態をスキャン
 
   if (sw1.fell()) {
     sine1.amplitude(0.4);
   }
 
   if (sw1.rose()) {
     sine1.amplitude(0.0);
   }
     
   const auto knob = analogRead(0) >> 2 << 2;
   const auto frequency = map(knob, 0, 1023, 0, 10000); // 0 ~~ 10000Hz
   sine1.frequency(frequency);  
 
    // delay
    const auto delayTime = map(analogRead(1), 0, 1023, 0, 750);
    delay1.delay(0, delayTime);
    const auto delayValue = analogRead(2) >> 2 << 2;
    const auto mappedDelayValue = map(delayValue, 0, 1023, 0, 9000);
    const auto floatDelayValue = mappedDelayValue / 10000.0;
    delayFeedback_amp.gain(floatDelayValue);
 
 }