

#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Monitoring GLURAKOL"
#define BLYNK_FIRMWARE_TYPE "ESP8266"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

/* =======================
   CONFIGURATION SECTION
   ======================= */
char auth[] = "YOUR_BLYNK_AUTH_TOKEN";
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

/* =======================
   PIN CONFIGURATION
   ======================= */
#define PIN_SENSOR_ANALOG A0
#define PIN_BUZZER        D2
#define PIN_LED_HIJAU     D3
#define PIN_LED_MERAH     D4
#define SDA_PIN           D6
#define SCL_PIN           D7

LiquidCrystal_I2C lcd(0x27, 16, 2);

/* =======================
   SYSTEM PARAMETERS
   ======================= */
const int threshold_jari   = 70;
const int margin_stabil    = 2;
const int jumlah_pembacaan = 5;
char gender = 'P';

/* =======================
   HEALTH LIMIT VALUES
   ======================= */
const float batas_gula = 145.0;
const float batas_kol  = 200.0;
const float batas_asam = 7.0;

/* =======================
   CALIBRATION DATA (P)
   ======================= */
const int   adc_gula_p[] = {56, 57, 58, 62, 65};
const float gula_p[]     = {121, 130, 177, 129, 173};

const int   adc_asam_p[] = {63, 72, 75};
const float asam_p[]     = {4.9, 7.0, 5.5};

const int   adc_kol_p[]  = {57, 58, 65};
const float kol_p[]      = {130, 177, 173};

const int n_gula_p = sizeof(adc_gula_p) / sizeof(adc_gula_p[0]);
const int n_asam_p = sizeof(adc_asam_p) / sizeof(adc_asam_p[0]);
const int n_kol_p  = sizeof(adc_kol_p)  / sizeof(adc_kol_p[0]);

/* =======================
   STATE MACHINE
   ======================= */
enum State { IDLE, MENGUKUR, TAMPIL };
State state = IDLE;

unsigned long waktuMulai = 0;
int   hasil_adc  = 0;
float hasil_gula = 0;
float hasil_asam = 0;
float hasil_kol  = 0;
bool  hasil_normal = true;

/* =======================
   HELPER FUNCTIONS
   ======================= */
float interpolasiLinear(int adc, const int adc_arr[], const float val_arr[], int n) {
  if (adc <= adc_arr[0]) return val_arr[0];
  if (adc >= adc_arr[n - 1]) return val_arr[n - 1];

  for (int i = 0; i < n - 1; i++) {
    if (adc >= adc_arr[i] && adc <= adc_arr[i + 1]) {
      float slope = (val_arr[i + 1] - val_arr[i]) /
                    float(adc_arr[i + 1] - adc_arr[i]);
      return val_arr[i] + slope * (adc - adc_arr[i]);
    }
  }
  return val_arr[0];
}

float kalibrasiGula(int adc, char g) {
  if (g == 'P' || g == 'p')
    return interpolasiLinear(adc, adc_gula_p, gula_p, n_gula_p);

  const int   adc_l[]  = {61, 63, 65};
  const float gula_l[] = {115, 161, 170};
  return interpolasiLinear(adc, adc_l, gula_l, 3);
}

float kalibrasiAsam(int adc, char g) {
  if (g == 'P' || g == 'p')
    return interpolasiLinear(adc, adc_asam_p, asam_p, n_asam_p);

  const int   adc_l[]  = {61, 63};
  const float asam_l[] = {5.5, 6.0};
  return interpolasiLinear(adc, adc_l, asam_l, 2);
}

float kalibrasiKol(int adc, char g) {
  if (g == 'P' || g == 'p')
    return interpolasiLinear(adc, adc_kol_p, kol_p, n_kol_p);

  const int   adc_l[] = {62, 65};
  const float kol_l[] = {129, 140};
  return interpolasiLinear(adc, adc_l, kol_l, 2);
}

int bacaSensorRataRata(int jumlah = 10) {
  long total = 0;
  for (int i = 0; i < jumlah; i++) {
    total += analogRead(PIN_SENSOR_ANALOG);
    delay(10);
  }
  return total / jumlah;
}

bool isStabil(int data[], int n, int margin) {
  for (int i = 1; i < n; i++) {
    if (abs(data[i] - data[i - 1]) > margin) return false;
  }
  return true;
}

/* =======================
   SETUP
   ======================= */
void setup() {
  Serial.begin(115200);

  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED_HIJAU, OUTPUT);
  pinMode(PIN_LED_MERAH, OUTPUT);

  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("GLURAKOL READY");
  delay(2000);
  lcd.clear();

  WiFi.begin(ssid, pass);
  lcd.print("Connecting WiFi");

  unsigned long t = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t < 10000) {
    delay(500);
    lcd.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    lcd.clear();
    lcd.print("WiFi Connected");
    Blynk.config(auth);
    Blynk.connect(5000);
  } else {
    lcd.clear();
    lcd.print("WiFi Failed");
    delay(2000);
  }

  lcd.clear();
  Serial.println("Set gender via Serial (L/P)");
}

/* =======================
   LOOP
   ======================= */
void loop() {
  if (Blynk.connected()) Blynk.run();

  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'L' || c == 'l') gender = 'L';
    if (c == 'P' || c == 'p') gender = 'P';
  }

  int adc_now = analogRead(PIN_SENSOR_ANALOG);
  bool jari = (adc_now < threshold_jari);

  switch (state) {
    case IDLE:
      lcd.setCursor(0, 0);
      lcd.print("Letakkan jari   ");
      lcd.setCursor(0, 1);
      lcd.print("ADC: ");
      lcd.print(adc_now);
      lcd.print("   ");

      if (jari) {
        state = MENGUKUR;
        waktuMulai = millis();
        lcd.clear();
        lcd.print("Mengukur...");
      }
      break;

    case MENGUKUR:
      if (millis() - waktuMulai > 1000) {
        int data[jumlah_pembacaan];
        for (int i = 0; i < jumlah_pembacaan; i++)
          data[i] = bacaSensorRataRata();

        if (isStabil(data, jumlah_pembacaan, margin_stabil)) {
          hasil_adc  = data[jumlah_pembacaan - 1];
          hasil_gula = kalibrasiGula(hasil_adc, gender);
          hasil_asam = kalibrasiAsam(hasil_adc, gender);
          hasil_kol  = kalibrasiKol(hasil_adc, gender);

          hasil_normal = (hasil_gula <= batas_gula &&
                          hasil_kol  <= batas_kol  &&
                          hasil_asam <= batas_asam);

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("G:");
          lcd.print(hasil_gula, 0);
          lcd.print(" A:");
          lcd.print(hasil_asam, 1);

          lcd.setCursor(0, 1);
          lcd.print("K:");
          lcd.print(hasil_kol, 0);

          if (Blynk.connected()) {
            Blynk.virtualWrite(V0, hasil_kol);
            Blynk.virtualWrite(V1, hasil_gula);
            Blynk.virtualWrite(V2, hasil_asam);
          }

          digitalWrite(PIN_LED_HIJAU, hasil_normal);
          digitalWrite(PIN_LED_MERAH, !hasil_normal);

          waktuMulai = millis();
          state = TAMPIL;
        } else {
          lcd.clear();
          lcd.print("Data tidak");
          lcd.setCursor(0, 1);
          lcd.print("stabil");
          delay(2000);
          state = IDLE;
        }
      }
      break;

    case TAMPIL:
      if (millis() - waktuMulai > 5000) {
        lcd.clear();
        state = IDLE;
      }
      break;
  }

  delay(100);
}
