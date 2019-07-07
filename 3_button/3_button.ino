#include <Audio.h>
 #include <Wire.h>
 #include <SPI.h>
 #include <SD.h>
 #include <SerialFlash.h>
 #include <Bounce2.h> // // スイッチ用ライブラリ
 
 // GUItool: begin automatically generated code
 AudioSynthWaveformSine   sine1;          //xy=189,184
 AudioOutputAnalogStereo  dacs1;          //xy=443,188
 AudioConnection          patchCord1(sine1, 0, dacs1, 0);
 AudioConnection          patchCord2(sine1, 0, dacs1, 1);
 // GUItool: end automatically generated code
 
 constexpr int SWITCH_1_PIN = 2; //2番ピンを使う
 Bounce sw1 = Bounce();
 
 void setup() {
     pinMode(SWITCH_1_PIN, INPUT_PULLUP);
     sw1.attach(SWITCH_1_PIN);
     sw1.interval(10);
     AudioMemory(15);
 
     AudioNoInterrupts();
 
     sine1.frequency(1000);
     // sine1.amplitude(1.0); コメントする
 
     AudioInterrupts();
     Serial.begin(115200);
 
 }
 
 void loop() {
   sw1.update(); // スイッチの状態をスキャン
 
 
   // 押した
   if (sw1.fell()) {
     sine1.amplitude(1.0);
   }
 
   // 離した
   if (sw1.rose()) {
     sine1.amplitude(0.0);
   }
     
   const auto knob = analogRead(0) >> 2 << 2;
   const auto frequency = map(knob, 0, 1023, 0, 10000); // 0 ~~ 10000Hz
   sine1.frequency(frequency);  
 
 }