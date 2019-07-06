# 電子楽器はつくれる！ ワークショップ#1 ハンズオン

## キットの説明
- ブレッドボード
- Teensy 3.6
- イヤホン
- SDカード
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
[シリアルドライバー](https://www.pjrc.com/teensy/serial_install.exe)も必要なので、こちらもダウンロードして、実行してください。

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


