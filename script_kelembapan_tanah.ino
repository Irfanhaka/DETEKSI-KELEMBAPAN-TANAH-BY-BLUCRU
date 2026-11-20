    #include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

// =================================
// 1. KONFIGURASI WIFI
// =================================
const char* ssid = "DHAR AL KHAER";
const char* password = "idamukhson";

// =================================
// 2. KONFIGURASI THINGSPEAK
// =================================
const char* writeAPIKey = "HK1V7Q0KWQB40XSG";

const char* server =
  "http://api.thingspeak.com/update";

WiFiClient client;

// =================================
// 3. KONFIGURASI SENSOR
// =================================
#define SOIL_PIN A0

// Nilai kalibrasi awal ESP8266
// Sesuaikan dengan hasil pengukuran sensor
const int ADC_DRY = 800;
const int ADC_WET = 350;

// Interval pengiriman 20 detik
unsigned long waktuTerakhir = 0;
const unsigned long interval = 20000;

int nomorKejadian = 0;

// =================================
// 4. MENGHITUNG KELEMBAPAN
// =================================
float hitungKelembapan(int adc) {

  float persen =
    ((float)(ADC_DRY - adc) /
    (ADC_DRY - ADC_WET)) * 100.0;

  return constrain(persen, 0, 100);
}

// =================================
// 5. KLASIFIKASI TANAH
// =================================
String klasifikasi(float persen) {

  if (persen <= 33) {
    return "KERING";
  }
  else if (persen <= 66) {
    return "NORMAL";
  }
  else {
    return "BASAH";
  }
}

// =================================
// 6. KONEKSI WIFI
// =================================
void hubungkanWiFi() {

  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  Serial.println("Menghubungkan WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi berhasil terhubung!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// =================================
// 7. KIRIM DATA KE THINGSPEAK
// =================================
void kirimThingSpeak(
  float kelembapan,
  String kondisi
) {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi terputus!");
    return;
  }

  HTTPClient http;

  if (!http.begin(client, server)) {
    Serial.println("Koneksi HTTP gagal!");
    return;
  }

  http.addHeader(
    "Content-Type",
    "application/x-www-form-urlencoded"
  );

  String data =
    "api_key=" + String(writeAPIKey);

  // Field 1 = KERING
  if (kondisi == "KERING") {
    data += "&field1=" + String(kelembapan, 2);
  }

  // Field 2 = BASAH
  else if (kondisi == "BASAH") {
    data += "&field2=" + String(kelembapan, 2);
  }

  // Field 3 = NORMAL
  else if (kondisi == "NORMAL") {
    data += "&field3=" + String(kelembapan, 2);
  }

  int httpCode = http.POST(data);

  Serial.println();
  Serial.println("--- THINGSPEAK ---");

  Serial.print("HTTP Code: ");
  Serial.println(httpCode);

  if (httpCode == 200) {

    String response = http.getString();

    Serial.print("Entry ID: ");
    Serial.println(response);

    if (response.toInt() > 0) {
      Serial.println("DATA BERHASIL DIKIRIM!");
    }
    else {
      Serial.println("DATA GAGAL DISIMPAN!");
    }

  }
  else {
    Serial.println("Pengiriman gagal!");
  }

  http.end();
}

// =================================
// 8. SETUP
// =================================
void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println("==============================");
  Serial.println("SOIL MOISTURE MONITORING");
  Serial.println("NODEMCU ESP8266");
  Serial.println("THINGSPEAK");
  Serial.println("KERING - BASAH - NORMAL");
  Serial.println("==============================");

  hubungkanWiFi();

  // Pengiriman pertama langsung dilakukan
  waktuTerakhir = millis() - interval;
}

// =================================
// 9. LOOP
// =================================
void loop() {

  hubungkanWiFi();

  unsigned long sekarang = millis();

  if (sekarang - waktuTerakhir >= interval) {

    waktuTerakhir = sekarang;

    // Membaca sensor melalui A0
    int nilaiADC = analogRead(SOIL_PIN);

    // Menghitung kelembapan
    float kelembapan =
      hitungKelembapan(nilaiADC);

    // Menentukan kondisi tanah
    String kondisi =
      klasifikasi(kelembapan);

    nomorKejadian++;

    // Menampilkan hasil pengujian
    Serial.println();
    Serial.println("==============================");

    Serial.print("PENGUJIAN KE-");
    Serial.println(nomorKejadian);

    Serial.print("KELEMBAPAN: ");
    Serial.print(kelembapan, 2);
    Serial.println("%");

    Serial.print("KONDISI TANAH: ");
    Serial.println(kondisi);

    // Mengirim data ke ThingSpeak
    kirimThingSpeak(
      kelembapan,
      kondisi
    );

    Serial.println("==============================");
  }
}