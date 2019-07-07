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
     AudioMemory(15);  // 最低限必要
 
     AudioNoInterrupts();  // 最低限必要
 
     sine1.frequency(1000); // 最低限必要
     sine1.amplitude(1.0); // 最低限必要
 
     AudioInterrupts();  // 最低限必要
 
 }
 
 void loop() {
   // put your main code here, to run repeatedly:
 
 }