Monitoring Kelembapan Tanah Berbasis IoT

Proyek ini menggunakan NodeMCU ESP8266 dan sensor Soil Moisture untuk mengukur tingkat kelembapan tanah. Data hasil pembacaan sensor diproses oleh mikrokontroler untuk menentukan kondisi tanah berdasarkan tiga kategori, yaitu KERING, NORMAL, dan BASAH. Selanjutnya, data dikirim melalui jaringan WiFi ke platform ThingSpeak agar dapat disimpan, dipantau, dan divisualisasikan secara online.

1. Perangkat yang Digunakan

Hardware:

NodeMCU ESP8266

Sensor Soil Moisture

Kabel jumper

Kabel USB

Software:

Arduino IDE

ThingSpeak

Library ESP8266WiFi

Library ESP8266HTTPClient

2. Rangkaian Sensor

Hubungkan sensor Soil Moisture ke NodeMCU ESP8266 dengan konfigurasi berikut:

VCC Sensor ke 3V3 NodeMCU ESP8266

AOUT Sensor ke A0 NodeMCU ESP8266

GND Sensor ke GND NodeMCU ESP8266

Pastikan seluruh kabel terpasang dengan benar sebelum perangkat dinyalakan.

Catatan: Konfigurasi tersebut menggunakan keluaran analog (AOUT) dari sensor untuk membaca nilai kelembapan tanah.

3. Persiapan Arduino IDE

Sebelum menjalankan program, pastikan Arduino IDE sudah terinstal di komputer.

Instalasi Board ESP8266

Jika board ESP8266 belum tersedia, lakukan langkah berikut:

Buka Arduino IDE.

Pilih File → Preferences.

Masukkan URL berikut pada bagian Additional Boards Manager URLs:

https://arduino.esp8266.com/stable/package_esp8266com_index.json

Klik OK.

Buka Tools → Board → Boards Manager.

Cari ESP8266, kemudian instal paket ESP8266 Community.

Instalasi Library

Library yang digunakan dalam proyek ini adalah:

ESP8266WiFi

ESP8266HTTPClient

WiFiClient

Library tersebut sudah tersedia dalam paket board ESP8266 yang telah diinstal melalui Boards Manager, sehingga tidak perlu diinstal secara terpisah melalui Library Manager.

4. Konfigurasi Program

Buka file program soil_moisture_monitoring.ino menggunakan Arduino IDE.

Sebelum program dijalankan, sesuaikan beberapa konfigurasi berikut.

Konfigurasi WiFi

Masukkan nama dan password jaringan WiFi yang akan digunakan oleh NodeMCU ESP8266.

const char* ssid = "NAMA_WIFI";
const char* password = "PASSWORD_WIFI";

Pastikan jaringan WiFi tersedia dan dapat digunakan untuk mengakses internet.

Konfigurasi ThingSpeak

Buat channel baru melalui https://thingspeak.mathworks.com/.

Aktifkan tiga field pada channel dengan konfigurasi berikut:

Field 1: Kering

Field 2: Basah

Field 3: Normal

Setelah channel dibuat, buka bagian API Keys. Salin Write API Key, kemudian masukkan ke dalam program.

const char* writeAPIKey = "WRITE_API_KEY";

Write API Key digunakan sebagai identitas untuk mengirimkan data dari NodeMCU ESP8266 ke channel ThingSpeak.

Konfigurasi Kalibrasi Sensor

Program menggunakan dua nilai kalibrasi untuk mengonversi pembacaan ADC menjadi persentase kelembapan tanah.

const int ADC_DRY = 800;
const int ADC_WET = 350;

Keterangan:

ADC_DRY = 800 merupakan nilai pembacaan sensor ketika tanah berada dalam kondisi kering.

ADC_WET = 350 merupakan nilai pembacaan sensor ketika tanah berada dalam kondisi basah.

Nilai kalibrasi tersebut merupakan nilai awal dan dapat disesuaikan berdasarkan hasil pengukuran sensor yang digunakan.

Konfigurasi Klasifikasi Tanah

Hasil pengukuran kelembapan tanah dikategorikan menjadi tiga kondisi:

KERING: kelembapan kurang dari atau sama dengan 33%.

NORMAL: kelembapan lebih dari 33% hingga 66%.

BASAH: kelembapan lebih dari 66%.

Klasifikasi dilakukan berdasarkan persentase kelembapan yang telah dihitung dari nilai ADC sensor.

Interval Pengiriman Data

Program menggunakan interval pengiriman data selama 20 detik.

const unsigned long interval = 20000;

Dengan pengaturan tersebut, NodeMCU ESP8266 akan membaca sensor dan mengirimkan data ke ThingSpeak setiap 20 detik.

5. Cara Menjalankan Program

Hubungkan NodeMCU ESP8266 ke komputer menggunakan kabel USB.

Buka file program melalui Arduino IDE.

Pilih Tools → Board → ESP8266 Boards → NodeMCU 1.0 (ESP-12E Module).

Pilih port COM yang digunakan oleh NodeMCU ESP8266.

Pastikan konfigurasi WiFi dan ThingSpeak sudah sesuai.

Klik Verify untuk memeriksa program.

Klik Upload untuk mengunggah program ke NodeMCU ESP8266.

Setelah proses upload selesai, buka Serial Monitor dengan baud rate 115200.

Jika program berjalan dengan baik, NodeMCU ESP8266 akan terhubung ke jaringan WiFi, membaca nilai kelembapan tanah melalui sensor, menghitung persentase kelembapan, dan menentukan kondisi tanah.

Selanjutnya, data akan dikirimkan ke ThingSpeak setiap 20 detik sesuai dengan kategori kondisi tanah yang terdeteksi.

6. Monitoring Data pada ThingSpeak

Data yang berhasil dikirim akan tersimpan pada channel ThingSpeak dan dapat dipantau melalui visualisasi grafik.

Terdapat tiga parameter field yang digunakan:

Field 1 (Kering): menyimpan persentase kelembapan ketika tanah terdeteksi dalam kondisi KERING.

Field 2 (Basah): menyimpan persentase kelembapan ketika tanah terdeteksi dalam kondisi BASAH.

Field 3 (Normal): menyimpan persentase kelembapan ketika tanah terdeteksi dalam kondisi NORMAL.

Setiap pembacaan hanya mengirimkan nilai kelembapan ke field yang sesuai dengan hasil klasifikasi. Dengan demikian, ketiga field dapat digunakan untuk memantau perubahan kondisi kelembapan tanah dari waktu ke waktu.

Data yang tersimpan pada ThingSpeak juga dapat diunduh dalam format CSV untuk keperluan pengolahan dan analisis lebih lanjut.

7. Troubleshooting

Beberapa kendala yang mungkin terjadi selama penggunaan perangkat:

NodeMCU ESP8266 tidak terdeteksi: periksa kabel USB, driver USB-to-Serial, dan port COM yang digunakan.

Upload program gagal: pastikan board dan port sudah dipilih dengan benar.

WiFi tidak terhubung: periksa kembali nama dan password WiFi serta pastikan jaringan tersedia.

Sensor tidak memberikan pembacaan yang sesuai: periksa sambungan pin VCC, GND, dan AOUT serta lakukan kalibrasi ulang.

Persentase kelembapan tidak sesuai kondisi tanah: sesuaikan nilai ADC_DRY dan ADC_WET berdasarkan hasil pengujian sensor.

Data tidak masuk ke ThingSpeak: periksa kembali Write API Key, koneksi internet, dan respons HTTP pada Serial Monitor.

Data gagal disimpan: pastikan interval pengiriman tidak terlalu cepat dan periksa respons dari server ThingSpeak.

8. Catatan Penggunaan

Pastikan NodeMCU ESP8266 mendapatkan suplai daya selama proses monitoring berlangsung.

Gunakan jaringan WiFi yang memiliki akses internet agar data dapat dikirim ke ThingSpeak.

Pastikan sensor Soil Moisture terpasang dengan baik pada media tanah yang akan diukur.

Lakukan kalibrasi sensor sebelum pengambilan data agar hasil pengukuran lebih sesuai dengan kondisi sebenarnya.

Perangkat harus tetap aktif dan terhubung ke jaringan WiFi selama proses monitoring.

Pengiriman data dilakukan setiap 20 detik.

Jangan mengunggah password WiFi atau Write API Key asli ke repository publik GitHub.

Informasi Proyek

Mikrokontroler: NodeMCU ESP8266

Sensor: Soil Moisture Sensor

Bahasa Pemrograman: C++

Platform Pemrograman: Arduino IDE

Komunikasi: WiFi (HTTP POST)

Platform IoT: ThingSpeak

Parameter Monitoring: Kelembapan Tanah (%)

Klasifikasi: KERING, NORMAL, BASAH

Interval Pengiriman: 20 detik