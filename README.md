# Smart Doorbell dengan ESP32-CAM dan Telegram

Project ini merupakan smart doorbell (bel pintu pintar) menggunakan ESP32-CAM. Ketika tombol ditekan, perangkat akan membunyikan buzzer, mengambil foto, dan mengirimkannya ke pengguna melalui bot Telegram.

Terdapat dua varian kode:

- Tanpa Flash (smartbell_telegram.ino)
- Dengan Flash LED (smartbell_flash_telegram.ino) – menyalakan LED flash internal saat memotret untuk penerangan tambahan.

## Fitur

- Terhubung ke WiFi
- Membaca input tombol (dengan debounce 5 detik)
- Membunyikan buzzer dengan nada dua tingkat
- Mengambil foto menggunakan kamera ESP32
- Mengirim foto ke Telegram via bot
- (Varian flash) Menyalakan LED flash sebelum memotret

## Komponen yang Diperlukan

- ESP32-CAM (module AI-Thinker)
- ESP32-CAM-MB Type- C
- Tombol (push button) – bisa menggunakan built-in jika ada, atau eksternal
- Buzzer pasif
- Kabel jumper
- Power supply (5V) – misal dari USB atau adaptor

## Pin Koneksi

Kamera menggunakan konfigurasi default AI-Thinker. Berikut pin yang digunakan:
| Komponen | Pin GPIO |
|----------------------|---------------------|
| Tombol | 14 |
| Buzzer | 12 |
| Flash LED (jika ada) | 4 |

> Catatan: Tombol menggunakan internal pull-up, sehingga kondisi LOW saat ditekan.

## Persiapan Development Environment

#### 1. Install Board ESP32 di Arduino IDE:

- Tambahkan URL berikut ke File > Preferences > Additional Boards Manager URLs: [Arduino ESP32-CAAM](https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json).
- Buka Tools > Board > Boards Manager, cari "ESP32" dan install.

#### 2. Install Library yang Diperlukan (umumnya sudah termasuk dalam paket ESP32):

- WiFi.h
- WiFiClientSecure.h
- esp_camera.h

#### 3. Siapkan Bot Telegram:

- Buat bot baru melalui @BotFather di Telegram, dapatkan BOT_TOKEN.
- Dapatkan CHAT_ID Anda menggunakan bot @userinfobot

## Konfigurasi Kode

Buka file .ino yang dipilih, lalu isi bagian berikut:

```
const char* ssid     = "NAMA_WIFI_ANDA";     // Ganti dengan SSID WiFi
const char* password = "PASSWORD_WIFI_ANDA"; // Ganti dengan password WiFi

String BOT_TOKEN = "TOKEN_BOT";  // Token bot Telegram
String CHAT_ID   = "ID_CHAT_ANDA";  // ID chat Telegram
```

## Cara Upload ke ESP32-CAM dengan MB (Type-C)

1 .Pasang modul ESP32-CAM ke MB dengan posisi pin yang benar. 2. Hubungkan MB ke komputer menggunakan kabel USB Type-C. 3. Di Arduino IDE, pilih pengaturan board: - Board: AI Thinker ESP32-CAM - Port: Pilih port COM yang sesuai (biasanya muncul setelah MB terhubung). 4. Klik tombol Upload. 5. Setelah selesai, tekan tombol RESET sekali untuk menjalankan program.

## Lisensi

Proyek ini dilisensikan di bawah MIT License. Anda bebas menggunakan, memodifikasi, dan mendistribusikannya.
