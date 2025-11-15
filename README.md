# Raspberry Pi 5 Baremetal OS in C

Raspberry Pi 5用のベアメタルOS実装（C言語版）

参考リポジトリ: [indigoblue6/pi5_os](https://github.com/indigoblue6/pi5_os)

## 概要

このプロジェクトは、Raspberry Pi 5上で動作するベアメタルOSのC言語実装です。基本的なハードウェア制御機能を提供します。

## 特徴

- **ベアメタル実装**: OSやライブラリに依存しない直接ハードウェア制御
- **UART通信**: BCM2712 UART (PL011) による入出力
- **システムタイマー**: BCM2712システムタイマー（1MHz）
- **GPIO制御**: RP1チップ経由のGPIO制御
- **シンプルなシェル**: 基本的なコマンドラインインターフェース

## ディレクトリ構成

```
pi5_os_in_C/
├── src/
│   ├── startup.s      # ARM64アセンブリ起動コード
│   ├── main.c         # メインカーネル
│   ├── uart.c         # UART通信ドライバ
│   ├── timer.c        # タイマードライバ
│   └── gpio.c         # GPIOドライバ
├── include/
│   ├── uart.h         # UARTヘッダー
│   ├── timer.h        # タイマーヘッダー
│   └── gpio.h         # GPIOヘッダー
├── target/
│   └── bcm2712.cfg    # OpenOCD設定
├── ldscript.ld        # リンカースクリプト
├── config.txt         # Pi5起動設定
├── Makefile           # ビルドスクリプト
└── README.md          # このファイル
```

## 必要な環境

### ハードウェア

- Raspberry Pi 5
- microSDカード（16GB以上推奨）
- UART通信用ケーブル（デバッグ用）

### ソフトウェア

- aarch64クロスコンパイラ
- GNU Make

## インストールとセットアップ

### 1. クロスコンパイルツールの準備

```bash
# Debian/Ubuntu
sudo apt install gcc-aarch64-linux-gnu make

# Arch Linux
sudo pacman -S aarch64-linux-gnu-gcc make
```

### 2. プロジェクトのビルド

```bash
# ビルド
make

# kernel8.imgが生成されることを確認
ls -la kernel8.img
```

### 3. SDカードの準備

1. Raspberry Pi OS Liteをインストール
2. `/boot/firmware/`に以下のファイルをコピー：
   - `kernel8.img`（ビルドで生成されたファイル）
   - `config.txt`（このリポジトリのもの）

### 4. 起動とシェル操作

1. SDカードをPi5に挿入して起動
2. UART接続でシェルにアクセス
3. 基本コマンドの実行例：

```
pi5os> help
pi5os> uptime
pi5os> test
pi5os> clear
```

## シェルコマンド

- `help` - ヘルプ表示とコマンド一覧
- `uptime` - システム稼働時間
- `test` - ハードウェアテスト実行
- `clear` - 画面クリア
- `reboot` - システム再起動

## 技術仕様

- **言語**: C + ARM64 Assembly
- **ターゲット**: aarch64 (ARM Cortex-A76)
- **最小メモリ**: 1GB RAM
- **UART設定**: 115200 baud, 8N1
- **起動アドレス**: 0x200000

## ハードウェアアドレス

- **UART Base**: 0x107d001000 (BCM2712 PL011)
- **Timer Base**: 0x107c003000 (BCM2712 System Timer)
- **GPIO Base**: 0x1f000d0000 (RP1 GPIO)

## ビルドコマンド

```bash
# カーネルのビルド
make

# ビルド成果物のクリーン
make clean

# カーネル情報の表示
make info

# 逆アセンブル
make disassemble

# UART接続
make uart

# ヘルプ
make help
```

## デバッグ

### UART接続

```bash
# Linux/macOS
screen /dev/ttyUSB0 115200

# または
make uart
```

### OpenOCD デバッグ

```bash
openocd -f interface/cmsis-dap.cfg -f target/bcm2712.cfg
```

## 参考資料

- [indigoblue6/pi5_os](https://github.com/indigoblue6/pi5_os) - Rust版の参考実装
- [tnishinaga/pi5_hack](https://github.com/tnishinaga/pi5_hack) - Pi5ベアメタルプログラミング
- [Raspberry Pi 5 Documentation](https://www.raspberrypi.com/documentation/computers/raspberry-pi.html)
- [BCM2712 ARM Peripherals](https://datasheets.raspberrypi.com/bcm2712/bcm2712-peripherals.pdf)
- [RP1 Peripherals](https://datasheets.raspberrypi.com/rp1/rp1-peripherals.pdf)

## ライセンス

MIT License

## 謝辞

このプロジェクトは以下のリポジトリを参考にして開発されました：

- [indigoblue6/pi5_os](https://github.com/indigoblue6/pi5_os) - Rust実装の参考
- [tnishinaga/pi5_hack](https://github.com/tnishinaga/pi5_hack) - Pi5ベアメタル開発の基礎
