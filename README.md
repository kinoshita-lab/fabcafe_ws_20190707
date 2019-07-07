# 電子楽器はつくれる！ ワークショップ#1 ハンズオンの内容

こちらに、当日みなさまにお伝えする内容をシェアします。
説明しながら進めることですので、今の段階では内容がわからなくても、大丈夫です。
受講後には、ワークショップのまとめとしてお役立てくだされば幸いです。


## キットの説明
- ブレッドボード
- Teensy 3.6
- イヤホン
- マイクロSDカード
- ボリューム
- スイッチ
- ケーブル

## 開発環境の準備
### Arduinoをインストール
[https://www.arduino.cc/en/Main/Software](Arduinoのダウンロードページ) から、ご自身の使っているOS用のソフトをダウンロードして、
インストールしてください。

#### macの方 
一度Arduinoを起動して、終了してください。

### Teensyduinoのインストール
[Teensyduinoのダウンロードページ](https://www.pjrc.com/teensy/td_download.html)から、ご自身の使っているOS用のソフトをダウンロードして、
インストールしてください。

#### windowsの方
[シリアルドライバー](https://www.pjrc.com/teensy/serial_install.exe)も必要なので、こちらもダウンロードして、実行してください。（実行すると、「不要です」というメッセージが出る場合もあります。)

### うまくいったか確認
Arduinoを起動します。
メニューから、ツール->ボード を選び、Teensyが追加できているかを確認します。
今回使うのは Teensy 3.6なので、それを選びます。

## ハンズオン
### 1 とりあえず音を出す
teensyで音を出すソフトを書くツール(webベース)
[Audio System Design Tool for Teensy Audio Library](https://www.pjrc.com/teensy/gui/index.html)

左の箱の"output" にある dacs をドラッグして何もない白いところに置く（ちょっと右よりくらい）
左の箱の"synth" にある sin をドラッグして何もない白いところに置く(dacsより左)

![このようになります](https://i.gyazo.com/58f10f417b10a9d2364bf5e4e61ac3a4.png)

sine1の右端の小さい■をドラッグして、dacs1の左端の上の■のところでマウスを離します。

sine1の右端の小さい■をドラッグして、dacs1の左端の下の■のところでマウスを離します。（もしうまくいかない場合、dacs1の方からやってみてください）

画面上部の赤い "Export"を押します。

この画面が表示されたら、コードをコピーします。コピーするには、ctrl+cボタンを押します(macの方はcmd+c)

![Export画面](https://i.gyazo.com/01acbcd322d42f86039fad48ad109b55.png) 

Arduinoを起動します。

先頭の部分に先ほどコピーしたコードをペーストします。
![Arduinoの画面](https://i.gyazo.com/308ea36bd99d54259dcfee1a1a22d2ab.png)

void setup()の中にコードを追加して、以下のようにします。

```
void setup() {
     AudioMemory(15);  // 最低限必要 TeensyでAudioが使うメモリーの量を決めます。
     AudioNoInterrupts();  // 最低限必要 Audio全体の設定をするので、一度動作を止めます。
 
     sine1.frequency(1000); // 最低限必要 作った sine の周波数を決めます。
     sine1.amplitude(1.0); // 最低限必要 作った sineの音量を決めます。
 
     AudioInterrupts();  // 最低限必要 設定が終わったので、Audioを動かします。
 
}
```

micoro USBケーブルでPCとteensyをつなぎます。

Arduinoの画面左上の、左から2番目のボタン、「マイコンボードに書き込む」を押します。

うまくいけば、ヘッドフォンから音がします。


### 2 ノブで音の高さを変える
ノブを取りだします。35 と書かれている所に差しこみます。

黒い線、赤い線、緑の線を図のように差します。

![ノブの付けかた](https://i.gyazo.com/thumb/1000/a4d6fc4992429fc40c97962e2731fd77-png.png)


先ほどのプログラムの、 void loop() のところを、以下のように書きかえます。
```
 void loop() {
   const auto knob = analogRead(0) >> 2 << 2; // knobの値を読む。最小は0で、最大は1023になります。
   const auto frequency = map(knob, 0, 1023, 0, 10000); // 0 ~~ 10000Hzの範囲にする
   sine1.frequency(frequency);  // sine1の周波数を設定する
 
 }
 ```
 Arduinoの画面左上の、左から2番目のボタン、「マイコンボードに書き込む」を押します。
 うまくいけば、ノブを回した時に音の高さが変わります。
 
 参考 ボリュームを増やしたい時（あとで追加します)
 ![参考 ボリュームを増やしたい時](https://i.gyazo.com/c9e8f01a9058058072fe597665d70348.png)
 
参考 teensyのピン配置 
![参考 Teensy3.6のピン配置](http://www.hobbytronics.co.uk/image/cache/data/teensy/teensy36-4-500x500.jpg)

### 3 ボタンを押したら音が出る
60 と書いてあるところの下にボタンを差します。
(ボタンには縦横の向きがあります。金属が自分に向かうような方向でとりつけます。)
黒い線、緑の線を図のように差します。

![ボタンを追加](https://i.gyazo.com/040223759c745b1ecbd589b30926c77f.png)

「ボタンを押す」をできるようにコードに追加していきます。
// 追加！ というところが増えたところです。

```
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce2.h> // 追加！ ボタン用クラスを使いたい
 
// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=189,184
AudioOutputAnalogStereo  dacs1;          //xy=443,188
AudioConnection          patchCord1(sine1, 0, dacs1, 0);
AudioConnection          patchCord2(sine1, 0, dacs1, 1);
// GUItool: end automatically generated code
 
constexpr int SWITCH_1_PIN = 2; // 追加！2番ピンを使う
Bounce sw1 = Bounce(); // 追加！ ボタン用クラスを作った

void setup() {
    pinMode(SWITCH_1_PIN, INPUT_PULLUP); // 追加！ ピンの設定 2番のピンを入力として使う
    sw1.attach(SWITCH_1_PIN); // 追加！ ボタン用クラスは2番ピンを使う
    sw1.interval(10);// 追加！ ボタンが押されたと判断するまでの時間間隔(10msec)
    AudioMemory(15);

    AudioNoInterrupts();

    sine1.frequency(1000);
    // sine1.amplitude(1.0); // 追加！ ボタンで操作したいので、 コメントにする

    AudioInterrupts();
}
 
void loop() {
  sw1.update(); // 追加！ スイッチの状態をスキャン
 
 
  // 押した // 追加！ 
  if (sw1.fell()) { // 追加！ 押したときは・・・
    sine1.amplitude(1.0); // 追加！ 音量を最大にする = on
  } // 追加！ 
 
  // 離した // 追加！ 
  if (sw1.rose()) { // 追加！ 離したときは・・・
    sine1.amplitude(0.0); // 追加！ 音量を0にする = off
  } // 追加！ 
    
  const auto knob = analogRead(0) >> 2 << 2;
  const auto frequency = map(knob, 0, 1023, 0, 10000); // 0 ~ 10000Hz
  sine1.frequency(frequency);  
 }
 ```

参考 スイッチを増やすには

![スイッチたくさん](https://i.gyazo.com/845c3e506da68af3b7cfee03a45ad3f6.png)


### 4 エフェクトをかける

ボリュームを1つ追加します。(緑の線は51に差します)
![エフェクト用](https://i.gyazo.com/faf6104d56dbc89342c87cd19d0eaa06.png)

#### リバーブをかけてみる
[Audio System Design Tool for Teensy Audio Library](https://www.pjrc.com/teensy/gui/index.html) を開きます。

sine1とdacs1の間の線をクリックしてオレンジにして、deleteボタンを押します（2本ともやる)。

sine1をちょっと左にずらします。

左の箱の"effect" にある reverb をドラッグして、sine1とdacs1の間に置きます。

sine1とreverb1、reverb1とdacs1を図のように■をドラッグしてつなぎます。

![リバーブ](https://i.gyazo.com/814991113543854e8b6158a1f36fcac5.png)

「1 とりあえず音を出す」でやったように、"Export"を押してコピー、Arduinoにペーストします。ボタン用に
```
#include <Bounce2.h>
```
を追加していますので、ここだけ追加します。

このようになります。
```
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
```

リバーブの時間を、ボリュームでコントロールできるようにします。
「3.ボタンを押したら音が出る」で作ったコードに追加します。
// 追加！ というところが増えたところです。
```
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
 
    reverb1.reverbTime(0.0); // 追加！ リバーブを初期設定しています。

    AudioInterrupts();
}
 
void loop() {
  sw1.update(); // スイッチの状態をスキャン

  if (sw1.fell()) {
    sine1.amplitude(0.3); //最大振幅を少し小くする
  }
 
  if (sw1.rose()) {
    sine1.amplitude(0.001); // ちょっと残す(不自然じゃないように)
  }
     
  const auto knob = analogRead(0) >> 2 << 2;
  const auto frequency = map(knob, 0, 1023, 0, 10000); // 0 ~~ 10000Hz
  sine1.frequency(frequency);  

   // reverb // 追加！ 
   const auto reverbTime = map(analogRead(1), 0, 1023, 0, 100); // 追加！ リバーブの時間をボリュームから読んで・・・
   reverb1.reverbTime(reverbTime); // 追加！ それを反映させます。
 }
```

#### ディレイをかけてみる
手順は「リバーブをかけてみる」と同じですが複雑です。

ボリュームを1つ追加します。
![ディレイ](https://i.gyazo.com/8d966576e492f02dca01654e0f811e30.png)

![ディレイ](https://i.gyazo.com/adbaca562036b27cd5ff23a3a8b08f12.png)
```
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
````

### 5 サンプルを使う
#### 5.1 Teensyのメモリを使う
[https://www.pjrc.com/teensy/td_libs_AudioPlayMemory.html](wav2sketch)
をコンパイルして使う(配布データに同梱)
Arduinoのプログラムと同じフォルダに、.wavファイルと、wav2sketchを入れて
./wav2sketch
と打てば、Arduinoで必要なファイルが自動的に作られます。


[Audio System Design Tool for Teensy Audio Library](https://www.pjrc.com/teensy/gui/index.html) を開きます。
下図のようにつなぎます。playMemは"play"にあります。
![playmem](https://i.gyazo.com/8d8cd96646cec2405b359d3e56457e59.png)


```
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
``` 

#### 5.2 マイクロSDカードから読む
マイクロSDカードをPCに差し込み、直下(フォルダではないところ)に、wavファイルを置く。
ファイル名は大文字で8文字まで、拡張子も大文字で .WAVにする。 例 MEOW.WAV

マイクロSDカードをTeensyに差して、以下のプログラムを書き込みます。


```
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
 }
 
void loop() {
  sw1.update(); // スイッチの状態をスキャン

  if (sw1.fell()) {
    playSdWav1.play("MEOW2.WAV");
  } 
}
```
