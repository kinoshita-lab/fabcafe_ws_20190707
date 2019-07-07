#include <Audio.h>
 #include <Wire.h>
 #include <SPI.h>
 #include <SD.h>
 #include <SerialFlash.h>
 #include <Bounce2.h> // // スイッチ用ライブラリ
 
 // GUItool: begin automatically generated code
 AudioSynthWaveformSine   sine1;          //xy=111,185
 AudioEffectReverb        reverb1;        //xy=375,188
 AudioOutputAnalogStereo  dacs1;          //xy=645,199
 AudioConnection          patchCord1(sine1, reverb1);
 AudioConnection          patchCord2(reverb1, 0, dacs1, 0);
 AudioConnection          patchCord3(reverb1, 0, dacs1, 1);
 // GUItool: end automatically generated code
 
 
 constexpr int SWITCH_1_PIN = 2; //2番ピンを使う
 Bounce sw1 = Bounce();
 
 void setup() {
     pinMode(SWITCH_1_PIN, INPUT_PULLUP);
     sw1.attach(SWITCH_1_PIN);
     sw1.interval(10);
     AudioMemory(800);
 
     AudioNoInterrupts();
 
     sine1.frequency(0);
 
     reverb1.reverbTime(0.0); // 
 
     AudioInterrupts();
     Serial.begin(115200);
 
 }
 
 void loop() {
   sw1.update(); // スイッチの状態をスキャン
 
   if (sw1.fell()) {
     sine1.amplitude(0.3); //最大振幅を少し小くする
   }
 
   if (sw1.rose()) {
     sine1.amplitude(0.001); // ちょっと残す
   }
     
   const auto knob = analogRead(0) >> 2 << 2;
   const auto frequency = map(knob, 0, 1023, 0, 10000); // 0 ~~ 10000Hz
   sine1.frequency(frequency);  
 
    // reverb
    const auto reverbTime = map(analogRead(1), 0, 1023, 0, 100);
    reverb1.reverbTime(reverbTime);
 
 }