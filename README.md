# pico-bluejay-configurator

## Pico用UF2 ダウンロード

**[⬇ Raspberry Pi Pico (RP2040) 用UF2を直接ダウンロード](https://github.com/temesotejam/pico-bluejay-configurator/releases/download/pico-latest/pico-bluejay-configurator-pico.uf2)**

[ダウンロード一覧（GitHub Releases）](https://github.com/temesotejam/pico-bluejay-configurator/releases/tag/pico-latest) · [最新のビルド結果（Actions）](https://github.com/temesotejam/pico-bluejay-configurator/actions/workflows/build-pico.yml)

- **Pico専用**（RP2040）。Pico 2 / RP2350は非対応です。
- Picoの **BOOTSEL** を押しながらUSB接続し、`RPI-RP2` にダウンロードしたUF2をコピーします。
- モーター配線は既存FCと同一（FR=GPIO15、FL=GPIO14、RR=GPIO3、RL=GPIO2）。
- **プロペラを外すこと。** 初期確認ではESCの設定を読み出すだけにしてください。実機ではESC 1/2の認識・設定画面表示を確認済みですが、設定の書き込み・復元および全ESCでの互換性は未検証です。

UF2はmainブランチのビルド成功時にGitHub Releaseへ自動更新されます。上のリンクからZIPを展開せずにそのままUF2を取得できます。

---

Raspberry Pi **Pico (RP2040)** を Bluejay / BLHeli_S ESC 用の **USB - BLHeli 4-way passthrough** インターフェースとして使う実験用ファームウェアです。FC用のモータ信号線を、そのままESC設定通信用に使います。飛行制御・DShot出力は行いません。

> **実機検証中**: Pico + Bluejay 0.14搭載ESCで少なくともESC 1/2の認識と設定画面表示を確認しています。ただし設定データの妥当性や保存処理は未検証です。ESCのファームウェア書換え（Flash）は、実機での読み出し・設定変更・再読出しが成功するまで行わないでください。

## 対応・範囲

- Raspberry Pi Pico / RP2040 用（Arduino-Pico / Earle Philhower コア）
- PC: Windows / ChromeまたはEdge / [ESC Configurator](https://esc-configurator.com/)
- ESC: Bluejayなど BLHeli_S/SiLabs(EFM8)の **BLHeli bootloaderを持つ機種** を対象
- USB CDC 115200 baud → MSP v1 / BLHeli 4-way passthrough → 19200 bps相当の単線半二重通信（PIO）
- **Pico 2 / RP2350 は未対応**。RP2040での動作検証後に追加予定
- Bluejayに使う SiLabs bootloader の識別・読出し・書込み・512 Bページ消去を追加

## モータ信号線のGPIO（pico-copter-fc と一致）

[既存フライトコントローラの pwm.hpp](https://github.com/temesotejam/pico-copter-fc/blob/main/firmware/pico_copter/pwm.hpp) の割り当てを維持しています。

| ESC | モーター位置 | Pico GPIO | Pico物理ピン |
| --- | --- | ---: | ---: |
| ESC 1 | FR（前右） | **15** | **20** |
| ESC 2 | FL（前左） | **14** | **19** |
| ESC 3 | RR（後右） | **3** | **5** |
| ESC 4 | RL（後左） | **2** | **4** |

> ESC Configurator の **ESC 1～4 は上の順序**です。既存ボードの配線を外さず差し替えられるようGPIOを合わせています。実際のESC番号と機体のモーター位置が一致していることを確認してください。

**必須配線**: Pico GND ↔ ESC GND、Pico GPIO15/14/3/2 ↔ 対応ESCの **signal**。PicoはPCのUSBから、ESCの主電源はESCの定格に合う外部電源から供給します。**ESC主電源（VBAT）をPico GPIO/3V3/VSYSへ接続しないこと。** ESC信号側が5 Vを強制駆動する機種はPicoのGPIOに直結しないでください（RP2040は5 V耐性なし）。単方向レベルシフタを挟むとESCからの返信が失われます。

## ビルド・書き込み

Arduino IDE を使う場合:
1. [Arduino-Pico core](https://arduino-pico.readthedocs.io/en/latest/install.html) を追加する
2. `firmware/pico_bluejay_passthrough/pico_bluejay_passthrough.ino` を開く
3. ボードを **Raspberry Pi Pico**、USB Stack を **Pico SDK（既定）** に設定
4. コンパイル・アップロード、または生成されたUF2を BOOTSEL 状態の `RPI-RP2` にコピー

GitHub Actions `Build RP2040 UF2` でビルドし、成功時にUF2をArtifactsとGitHub Releases（固定のダウンロードリンク）へ公開します。実機での設定書き込み・更新の安全性は未確認です。

## 接続・設定手順

1. **必ず全プロペラを外す**。ESCの電圧/電流が適切な電源を用意し、できれば電流制限を使う
2. PicoにUF2を書き込み、PicoをPCにUSB接続
3. ESC信号線とGNDの配線を確認してから、ESCに外部電源を供給
4. Chrome/Edgeで [ESC Configurator](https://esc-configurator.com/) を開く（Betaflight Configuratorやシリアルモニタは閉じる）
5. シリアルポートを選択し Connect → Read Settings
6. **まずは読み出しだけを試験**。Bluejayのバージョン・対象ESCの識別・現在設定のバックアップを取る
7. 読み出しが安定し再接続可能なことを確認できたら、Min/Max Startup Power を一項目ずつ変更 → Save Settings → 電源再投入 → 再読出しで検証

「接続できる」ことと「ESCから正しく設定を読み書きできる」ことは別です。認識できても書き込みを急がないでください。**Flash All ESCsは最初の段階では実行しないでください。**

## 実装と注意事項

- **上流**: [BrushlessPower/BlHeli-Passthrough - RP2040](https://github.com/BrushlessPower/BlHeli-Passthrough/tree/main/RP2040) を移植・修正。元コードの著作権・GPL-3.0に従い同じライセンスで公開
- ベース実装はARM bootloader向けのため、SiLabs署名判別（0xE8xx～0xF8xx）、4-wayモード=1、ページサイズ512 Bを追加
- PicoのGPIOは **3.3 V専用**。ESCの信号線が3.3 Vで論理HIGHと認識され、かつ信号線を5 Vで駆動しないことを確認
- コマンド/応答とCRCを確認して、不正な識別情報を成功と見なさない
- ESC設定領域はFlashの一部です。停電・配線断・誤認識で設定破損の可能性があります
- Pico 2、特定のBluejayバージョン、すべてのESC機種の互換性は保証しません

## ライセンス・参照

本リポジトリの上流移植部分は GNU GPL v3. `LICENSE`、`NOTICE.md` を参照。特に [Betaflight serial_4way.c](https://github.com/betaflight/betaflight/blob/master/src/main/io/serial_4way.c) および [serial_4way_avrootloader.c](https://github.com/betaflight/betaflight/blob/master/src/main/io/serial_4way_avrootloader.c) のモード・デバイス識別・コマンド仕様も照合しています。
