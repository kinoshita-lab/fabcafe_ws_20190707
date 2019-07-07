 #include <Audio.h>
 #include <Wire.h>
 #include <SPI.h>
 #include <SD.h>
 #include <SerialFlash.h>
 #include <Bounce2.h> // // スイッチ用ライブラリ
 
 
 // GUItool: begin automatically generated code
 AudioPlaySdWav           playSdWav1;     //xy=153,263
 AudioOutputAnalogStereo  dacs1;          //xy=486,258
 AudioConnection          patchCord1(playSdWav1, 0, dacs1, 0);
 AudioConnection          patchCord2(playSdWav1, 1, dacs1, 1);
 // GUItool: end automatically generated code
 
 // SDカードから読むのに必要な設定
 // Use these with the Teensy 3.5 & 3.6 SD card
 #define SDCARD_CS_PIN    BUILTIN_SDCARD
 #define SDCARD_MOSI_PIN  11  // not actually used
 #define SDCARD_SCK_PIN   13  // not actually used
 
 constexpr int SWITCH_1_PIN = 2; //2番ピンを使う
 Bounce sw1 = Bounce();
 
 void setup() {
     pinMode(SWITCH_1_PIN, INPUT_PULLUP);
 
   // SDカードのセットアップ
   SPI.setMOSI(SDCARD_MOSI_PIN);
   SPI.setSCK(SDCARD_SCK_PIN);
   if (!(SD.begin(SDCARD_CS_PIN))) {
     // stop here, but print a message repetitively
     while (1) {
       Serial.println("Unable to access the SD card");
       delay(500);
     }
   }
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
     playSdWav1.play("MEOW2.WAV");
   }
 
 }