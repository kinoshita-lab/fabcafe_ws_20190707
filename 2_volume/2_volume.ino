 #include <Audio.h>
 #include <Wire.h>
 #include <SPI.h>
 #include <SD.h>
 #include <SerialFlash.h>
 
 // GUItool: begin automatically generated code
 AudioSynthWaveformSine   sine1;          //xy=189,184
 AudioOutputAnalogStereo  dacs1;          //xy=443,188
 AudioConnection          patchCord1(sine1, 0, dacs1, 0);
 AudioConnection          patchCord2(sine1, 0, dacs1, 1);
 // GUItool: end automatically generated code
 
 
 void setup() {
     AudioMemory(15);
 
     AudioNoInterrupts();
 
     sine1.frequency(1000);
     sine1.amplitude(1.0);
 
     AudioInterrupts();
 
 }
 
 void loop() {
   const auto knob = analogRead(0) >> 2 << 2; // knobの値を読む
   const auto frequency = map(knob, 0, 1023, 0, 10000); // 0 ~~ 10000Hzの範囲にする
   sine1.frequency(frequency);  // sine1の周波数を設定する
 
 }