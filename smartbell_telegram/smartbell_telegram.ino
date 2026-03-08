#include "WiFi.h"
#include "WiFiClientSecure.h"
#include "esp_camera.h"

const char* ssid     = "";     //SSID WiFi
const char* password = "";    //Ganti dengan Password WiFi

String BOT_TOKEN = "";  // API Telegram
String CHAT_ID   = "";  // ID Telegram

// PIN (BUZZER & BUTTON)
#define BUTTON_PIN 14
#define BUZZER_PIN 12

// PIN KAMERA (AI-THINKER)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// untuk mencegah tombol tertekan berkali-kali secara cepat (Debounce)
unsigned long lastPressTime = 0;
const int debounceDelay = 5000; // Jeda 5 detik sebelum bel bisa ditekan lagi

void setup() {
  Serial.begin(115200);
  Serial.println("\n--- Memulai Smart Doorbell ---");

  // Setup Pin Input/Output
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  // Setup Koneksi WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Terhubung!");

  // Konfigurasi Kamera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // Set resolusi gambar
  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA; // 640x480
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }


  config.grab_mode = CAMERA_GRAB_LATEST; 
  // Inisialisasi Kamera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Kamera gagal diinisialisasi dengan error 0x%x\n", err);
    return;
  }
  Serial.println("Kamera siap!");
}

void loop() {
  // Membaca status tombol (LOW karena kita pakai INPUT_PULLUP)
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW && (millis() - lastPressTime > debounceDelay)) {
    Serial.println("Bel ditekan!");
    lastPressTime = millis();

    tone(BUZZER_PIN, 3000, 300); // Nada tinggi dan keras
    delay(350);
    tone(BUZZER_PIN, 2500, 500); // Nada sedikit turun
    
    // 2. Ambil dan Kirim Foto ke Telegram
    sendPhotoTelegram();
  }
}

// Fungsi unruk mengirim foto ke Telegram
String sendPhotoTelegram() {
  const char* myDomain = "api.telegram.org";
  String getAll = "";
  String getBody = "";

  camera_fb_t * fb = NULL;

  fb = esp_camera_fb_get();
  esp_camera_fb_return(fb);

  fb = esp_camera_fb_get();  
  if(!fb) {
    Serial.println("Gagal mengambil foto");
    return "Gagal mengambil foto";
  }
  
  Serial.println("Berhasil mengambil foto, mengirim ke Telegram...");
  
  WiFiClientSecure client;
  client.setInsecure(); // Mengabaikan verifikasi SSL untuk kemudahan

  if (client.connect(myDomain, 443)) {
    String head = "--Boundary\r\nContent-Disposition: form-data; name=\"chat_id\"; \r\n\r\n" + CHAT_ID + "\r\n--Boundary\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--Boundary--\r\n";
    uint16_t imageLen = fb->len;
    uint16_t extraLen = head.length() + tail.length();
    uint16_t totalLen = imageLen + extraLen;
  
    // Kirim Header HTTP POST
    client.println("POST /bot" + BOT_TOKEN + "/sendPhoto HTTP/1.1");
    client.println("Host: " + String(myDomain));
    client.println("Content-Length: " + String(totalLen));
    client.println("Content-Type: multipart/form-data; boundary=Boundary");
    client.println();
    client.print(head);
  
    // Kirim data gambar dalam bentuk chunk array
    uint8_t *fbBuf = fb->buf;
    size_t fbLen = fb->len;
    for (size_t n=0;n<fbLen;n=n+1024) {
      if (n+1024<fbLen) {
        client.write(fbBuf, 1024);
        fbBuf += 1024;
      } else if (fbLen%1024>0) {
        size_t remainder = fbLen%1024;
        client.write(fbBuf, remainder);
      }
    }  
    client.print(tail); // Kirim Tail
    
    esp_camera_fb_return(fb); // Kosongkan buffer memori kamera
    
    // Tunggu respon server
    int waitTime = 5000;   // timeout 10 detik
    long startTimer = millis();
    boolean state = false;
    
    while ((startTimer + waitTime) > millis()){
      Serial.print(".");
      delay(100);      
      while (client.available()) {
        char c = client.read();
        if (state==true) getBody += String(c);        
        if (c == '\n') {
          if (getAll.length()==0) state=true; 
          getAll = "";
        } else if (c != '\r')
          getAll += String(c);
        startTimer = millis();
      }
      if (getBody.length()>0) break;
    }
    client.stop();
    Serial.println("\nFoto berhasil terkirim!");
  } else {
    Serial.println("Gagal terhubung ke Telegram.");
  }
  return getBody;
}