<div align="center"> 
   
# Smart Doorbell dengan ESP32-CAM dan Telegram
   Project ini merupakan **Smart Doorbell** (bel pintu pintar) menggunakan **ESP32-CAM**. Ketika tombol ditekan, perangkat akan membunyikan buzzer, mengambil foto, dan mengirimkannya ke       pengguna melalui bot Telegram.
   
</div>

Terdapat dua varian kode:
- Tanpa Flash `smartbell_telegram.ino`
- Dengan Flash LED `smartbell_flash_telegram.ino` – menyalakan LED flash internal saat memotret untuk penerangan tambahan.

---

## Fitur
- Terhubung ke WiFi via WifiManager
- Membaca input tombol (dengan debounce 5 detik)
- Membunyikan buzzer dengan nada dua tingkat
- Mengambil foto menggunakan kamera ESP32
- Mengirim foto ke Telegram via bot
- (Varian flash) Menyalakan LED flash sebelum memotret

---

## Komponen yang Diperlukan\
- [**ESP32-CAM** (module AI-Thinker)](https://s.shopee.co.id/5VRyA3y81N)
- [**ESP32-CAM-MB Type- C**](https://s.shopee.co.id/5VRyA3y81N)
- [**Push Button** – bisa menggunakan built-in jika ada, atau eksternal](https://s.shopee.co.id/9Uy6vXOHCc)
- [**Buzzer Pasive**](https://s.shopee.co.id/5L8XxyOEzg)
- [**Kabel Jumper**](https://s.shopee.co.id/8fOzwA14Dv)

---

## Pin Koneksi
Kamera menggunakan konfigurasi default AI-Thinker. Berikut pin yang digunakan:
| Komponen | Komponen PIN | ESP32 CAM PIN |
| :--- | :--- | :--- |
| **Push Button** | OFF | **GPIO 14** |
| | GND | **GND** |
| **Buzzer Passive** | + (plus) | **GPIO 12** |
| | - (minus) | **GND** |
| Flash LED | 4 |

> **Catatan:** Tombol menggunakan internal pull-up, sehingga kondisi LOW saat ditekan.

---

## Persiapan Development Environment

#### 1. Install Board ESP32 di Arduino IDE:

- Tambahkan URL berikut ke File > Preferences > Additional Boards Manager URLs: [Arduino ESP32-CAM](https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json).
- Buka Tools > Board > Boards Manager, cari "ESP32" dan install.

#### 2. Install Library yang Diperlukan (umumnya sudah termasuk dalam paket ESP32):

- `WiFi.h`
- `WiFiClientSecure.h`
- `esp_camera.h`
- `WiFiManager.h`

#### 3. Siapkan Bot Telegram:

- Buat bot baru melalui @BotFather di Telegram, dapatkan `BOT_TOKEN`.
- Dapatkan `CHAT_ID` Anda menggunakan bot @userinfobot

---

## Konfigurasi Kode

Buka file `.ino` yang dipilih, lalu isi bagian berikut:

```
String BOT_TOKEN = "TOKEN_BOT";  // Token bot Telegram
String CHAT_ID   = "ID_CHAT_ANDA";  // ID chat Telegram
```

## Cara Upload ke ESP32-CAM dengan MB (Type-C)
1. Pasang modul ESP32-CAM ke MB dengan posisi pin yang benar.
2. Hubungkan MB ke komputer menggunakan kabel USB Type-C.
3. Di Arduino IDE, pilih pengaturan board:
   - Board: AI Thinker ESP32-CAM
   - Port: Pilih port COM yang sesuai (biasanya muncul setelah MB terhubung).
4. Klik tombol Upload.
5. Setelah selesai, tekan tombol RESET sekali untuk menjalankan program.

---
