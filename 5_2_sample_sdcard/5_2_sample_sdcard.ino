 #include <Audio.h>
 #include <Wire.h>
 #include <SPI.h>
 #include <SD.h>
 #include <SerialFlash.h>
 #include <Bounce2.h> // // スイッチ用ライブラリ
 #include "play_sd_wav_modified.h"
 
 // GUItool: begin automatically generated code
 AudioPlaySdWav_modified           playSdWav1;     //xy=153,263
 AudioOutputAnalogStereo  dacs1;          //xy=486,258
 AudioConnection          patchCord1(playSdWav1, 0, dacs1, 0);
 AudioConnection          patchCord2(playSdWav1, 1, dacs1, 1);
 // GUItool: end automatically generated code
 
 constexpr int SWITCH_1_PIN = 2; //2番ピンを使う
 Bounce sw1 = Bounce();
 
 void setup() {
     pinMode(SWITCH_1_PIN, INPUT_PULLUP);


    if (!SD.begin(BUILTIN_SDCARD)) {
    return;
    }
 
     sw1.attach(SWITCH_1_PIN);
     sw1.interval(10);
     AudioMemory(100);
 
     AudioNoInterrupts();
     
     AudioInterrupts();
 }
 
 void loop() {
  
  
   sw1.update(); // スイッチの状態をスキャン
 
   if (sw1.fell()) {
     playSdWav1.play("LONG.WAV");
   }
 
 }
