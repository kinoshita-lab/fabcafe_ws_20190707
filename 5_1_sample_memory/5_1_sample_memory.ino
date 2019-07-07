#include <Audio.h>
 #include <Wire.h>
 #include <SPI.h>
 #include <SD.h>
 #include <SerialFlash.h>
 #include <Bounce2.h> // // スイッチ用ライブラリ
 
 #include "AudioSampleSnare.h"
 
 // GUItool: begin automatically generated code
 AudioPlayMemory          playMem1;       //xy=194,454
 AudioOutputAnalogStereo  dacs1;          //xy=445,454
 AudioConnection          patchCord1(playMem1, 0, dacs1, 0);
 AudioConnection          patchCord2(playMem1, 0, dacs1, 1);
 // GUItool: end automatically generated code
 
 
 constexpr int SWITCH_1_PIN = 2; //2番ピンを使う
 Bounce sw1 = Bounce();
 
 void setup() {
     pinMode(SWITCH_1_PIN, INPUT_PULLUP);
     sw1.attach(SWITCH_1_PIN);
     sw1.interval(10);
     AudioMemory(800);
 
     AudioNoInterrupts();
 
 
     AudioInterrupts();
     Serial.begin(115200);
 
 }
 
 void loop() {
   sw1.update(); // スイッチの状態をスキャン
 
   if (sw1.fell()) {
     playMem1.play(AudioSampleSnare);
   }
 }