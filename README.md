# Non-Invasive-IoT-Based-GLURAKOL-Monitoring-System-Glucose-Uric-Acid-Cholesterol-
Non-invasive IoT-based system for real-time monitoring of glucose, uric acid, and cholesterol using sensors, microcontrollers, and cloud integration.
# 🩺 Non-Invasive IoT-Based GLURAKOL Monitoring System

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![ESP8266](https://img.shields.io/badge/Platform-ESP8266-blue.svg)](https://www.espressif.com/en/products/socs/esp8266)
[![Arduino](https://img.shields.io/badge/IDE-Arduino-00979D.svg)](https://www.arduino.cc/)
[![Blynk](https://img.shields.io/badge/IoT-Blynk-00C7B7.svg)](https://blynk.io/)

> **Non-invasive health monitoring system for real-time tracking of Glucose, Uric Acid, and Cholesterol levels using IoT technology**

![System Overview](https://github.com/user-attachments/assets/6610e1dd-5e8f-43ee-82cb-26f33c424298)

*GLURAKOL monitoring system with ESP8266 and sensor integration*

---

## 📋 Table of Contents
- [About](#about)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Hardware Components](#hardware-components)
- [Software Requirements](#software-requirements)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)
- [Calibration](#calibration)
- [Circuit Diagram](#circuit-diagram)
- [API Documentation](#api-documentation)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

---

## 🎯 About

**GLURAKOL** (Glucose + Uric Acid + Cholesterol) is a final project implementing an IoT-based non-invasive health monitoring system. The system measures three critical health parameters:

- **Glucose (Gula)** - Blood sugar levels
- **Uric Acid (Asam Urat)** - Uric acid concentration
- **Cholesterol (Kolesterol)** - Cholesterol levels

### Key Highlights
✅ **Non-invasive measurement** - No blood sample required  
✅ **Real-time monitoring** - Instant results via LCD display  
✅ **IoT integration** - Cloud connectivity via Blynk  
✅ **Gender-specific calibration** - Accurate readings for male/female  
✅ **Health alerts** - LED and buzzer notifications  

---

## ✨ Features

### 📊 Core Features
- **Multi-parameter monitoring**: Simultaneous measurement of 3 health indicators
- **Linear interpolation calibration**: Gender-specific calibration curves
- **Stability detection**: Ensures accurate readings through signal validation
- **Real-time display**: 16x2 LCD with I2C interface
- **Visual & audio alerts**: 
  - 🟢 Green LED: Normal readings
  - 🔴 Red LED: Abnormal readings
  - 🔊 Buzzer: Warning alerts

### 🌐 IoT Features
- **WiFi connectivity**: ESP8266 wireless communication
- **Blynk integration**: Remote monitoring via mobile app
- **Cloud data storage**: Historical data tracking
- **Real-time sync**: Instant data transmission

### 🔧 Technical Features
- **Adaptive thresholds**: Gender-based health parameter limits
- **Averaging algorithm**: 10-sample averaging for noise reduction
- **State machine**: IDLE → MEASURING → DISPLAY workflow
- **Serial configuration**: Gender selection via Serial Monitor

---

## 🏗️ System Architecture

```
┌─────────────────┐
│   MAX30105      │
│  (IR Sensor)    │
└────────┬────────┘
         │
         ▼
┌─────────────────┐      ┌──────────────┐
│   ESP8266       │◄────►│  WiFi Router │
│   (NodeMCU)     │      └──────┬───────┘
└────┬─────┬──────┘             │
     │     │                    ▼
     │     │            ┌───────────────┐
     │     │            │  Blynk Cloud  │
     │     │            └───────────────┘
     │     │                    ▲
     ▼     ▼                    │
┌────────┐ ┌────────┐    ┌──────────────┐
│  LCD   │ │ LED +  │    │  Mobile App  │
│ 1602   │ │ Buzzer │    │   (Blynk)    │
└────────┘ └────────┘    └──────────────┘
```

### Data Flow
1. **Sensor Reading**: MAX30105 captures analog signal (ADC 0-1023)
2. **Signal Processing**: Averaging & stability validation
3. **Calibration**: Linear interpolation based on gender
4. **Local Display**: Results shown on LCD
5. **IoT Transmission**: Data sent to Blynk cloud
6. **Alert System**: LED/Buzzer activated if abnormal

---



### Pin Configuration

| Component | ESP8266 Pin | Arduino Pin |
|-----------|-------------|-------------|
| Sensor (Analog) | A0 | A0 |
| Buzzer | D2 | GPIO4 |
| LED Green | D3 | GPIO0 |
| LED Red | D4 | GPIO2 |
| LCD SDA | D6 | GPIO12 |
| LCD SCL | D7 | GPIO13 |

---

## 💻 Software Requirements

### Arduino IDE
- **Version**: 1.8.19 or higher
- **Board Manager**: ESP8266 Community v3.0.0+

### Required Libraries
```cpp
#include <Wire.h>                    // I2C communication
#include <LiquidCrystal_I2C.h>      // LCD display
#include <ESP8266WiFi.h>            // WiFi connectivity
#include <BlynkSimpleEsp8266.h>     // Blynk IoT platform
```

Install via Arduino Library Manager:
```
Sketch → Include Library → Manage Libraries
```

Search and install:
- `LiquidCrystal I2C` by Frank de Brabander
- `Blynk` by Volodymyr Shymanskyy
- `ESP8266WiFi` (bundled with ESP8266 board package)

---

## 📥 Installation

### 1. Clone Repository
```bash
git clone https://github.com/yourusername/glurakol-iot-monitoring.git
cd glurakol-iot-monitoring
```

### 2. Hardware Assembly
1. Connect components according to [circuit diagram](#circuit-diagram)
2. Verify all connections with multimeter
3. Power ESP8266 via USB (5V)

### 3. Software Setup

#### A. Install Arduino IDE
```bash
# Download from: https://www.arduino.cc/en/software
```

#### B. Add ESP8266 Board
```
File → Preferences → Additional Board Manager URLs:
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

```
Tools → Board → Board Manager → Search "ESP8266" → Install
```

#### C. Install Libraries
```
Sketch → Include Library → Manage Libraries
```
Install: `LiquidCrystal_I2C`, `Blynk`

### 4. Configure Credentials
Edit `glurakol_monitor.ino`:
```cpp
char auth[] = "YOUR_BLYNK_AUTH_TOKEN";
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";
```

### 5. Upload Code
```
Tools → Board → NodeMCU 1.0 (ESP-12E Module)
Tools → Port → COM3 (your port)
Sketch → Upload
```

---

## ⚙️ Configuration

### Blynk Setup

1. **Create Blynk Account**
   - Download Blynk app (iOS/Android)
   - Sign up and create new project
   - Select ESP8266 device

2. **Virtual Pin Mapping**
   | Virtual Pin | Parameter | Widget |
   |-------------|-----------|--------|
   | V0 | Cholesterol | Gauge |
   | V1 | Glucose | Gauge |
   | V2 | Uric Acid | Gauge |
   | V5 | Gender | Label |

3. **Copy Auth Token**
   ```
   Project Settings → Auth Token → Copy to clipboard
   ```

### WiFi Configuration
```cpp
char ssid[] = "YourWiFiName";      // 2.4GHz only
char pass[] = "YourPassword";       // WPA2
```

### Health Thresholds
```cpp
const float batas_gula = 145.0;    // mg/dL
const float batas_kol = 200.0;     // mg/dL
const float batas_asam = 7.0;      // mg/dL
```

### Gender Selection
Via Serial Monitor (115200 baud):
```
L or l → Male (Laki-laki)
P or p → Female (Perempuan)
```

---

## 🚀 Usage

### Step-by-Step Operation

1. **Power On**
   ```
   LCD: "GLURAKOL READY"
   System initializes WiFi and Blynk
   ```

2. **Set Gender**
   ```
   Open Serial Monitor (Ctrl+Shift+M)
   Baud: 115200
   Type: L (male) or P (female)
   ```

3. **Place Finger**
   ```
   LCD: "Letakkan jari"
   Place finger on MAX30105 sensor
   Wait for buzzer beep (1 second)
   ```

4. **Measurement**
   ```
   LCD: "Mengukur..."
   System takes 5 stable readings
   Duration: ~3-5 seconds
   ```

5. **View Results**
   ```
   LCD Line 1: G:145 A:6.5
   LCD Line 2: K:180 A:65
   
   G = Glucose
   A = Uric Acid (Asam Urat)
   K = Cholesterol (Kolesterol)
   ```

6. **Check Alerts**
   - 🟢 **Green LED ON**: All parameters normal
   - 🔴 **Red LED ON + 3 Beeps**: Abnormal detected

7. **Blynk App**
   - Open Blynk app
   - View real-time data
   - Check historical trends

### Serial Monitor Output
```
ADC=65 Gula=145.2 Asam=6.5 Kol=180.0
Sending to Blynk - Kol: 180.0 Gula: 145.2 Asam: 6.5
```

---

## 🔬 Calibration

### Calibration Data Structure

#### Female (Perempuan)
```cpp
// Glucose calibration points
const int adc_gula_p[] = {56, 57, 58, 62, 65};
const float gula_p[] = {121, 130, 177, 129, 173};

// Uric Acid calibration points
const int adc_asam_p[] = {63, 72, 75};
const float asam_p[] = {4.9, 7.0, 5.5};

// Cholesterol calibration points
const int adc_kol_p[] = {57, 58, 65};
const float kol_p[] = {130, 177, 173};
```

#### Male (Laki-laki)
```cpp
// Glucose
const int adc_gula_l[] = {61, 63, 65};
const float gula_l[] = {115, 161, 170};

// Uric Acid
const int adc_asam_l[] = {61, 63};
const float asam_l[] = {5.5, 6.0};

// Cholesterol
const int adc_kol_l[] = {62, 65};
const float kol_l[] = {129, 140};
```

### Linear Interpolation Algorithm
```cpp
float interpolasiLinear(int adc, const int adc_arr[], 
                        const float val_arr[], int n) {
  // Boundary conditions
  if (adc <= adc_arr[0]) return val_arr[0];
  if (adc >= adc_arr[n-1]) return val_arr[n-1];
  
  // Find interval and interpolate
  for (int i = 0; i < n-1; i++) {
    if (adc >= adc_arr[i] && adc <= adc_arr[i+1]) {
      float slope = (val_arr[i+1] - val_arr[i]) / 
                    (adc_arr[i+1] - adc_arr[i]);
      return val_arr[i] + slope * (adc - adc_arr[i]);
    }
  }
  return val_arr[0];
}
```

### How to Recalibrate

1. **Prepare Reference Measurements**
   - Use GCU Easy Touch meter
   - Measure actual glucose/uric acid/cholesterol
   - Record both sensor ADC and reference values

2. **Update Calibration Arrays**
   ```cpp
   // Add new point at ADC=70, Glucose=150
   const int adc_gula_p[] = {56, 57, 58, 62, 65, 70};
   const float gula_p[] = {121, 130, 177, 129, 173, 150};
   ```

3. **Re-upload Code**
   ```bash
   Arduino IDE → Upload
   ```

4. **Verify Accuracy**
   - Compare sensor readings with reference meter
   - Adjust until error < 5%

---

## 🔌 Circuit Diagram

### Schematic
```
                    ESP8266 NodeMCU
                    ┌─────────────┐
     MAX30105 ──────┤ A0          │
                    │             │
     Buzzer ────────┤ D2 (GPIO4)  │
                    │             │
     LED Green ─────┤ D3 (GPIO0)  │
                    │             │
     LED Red ───────┤ D4 (GPIO2)  │
                    │             │
     LCD SDA ───────┤ D6 (GPIO12) │
                    │             │
     LCD SCL ───────┤ D7 (GPIO13) │
                    │             │
     GND ───────────┤ GND         │
                    │             │
     5V ────────────┤ VIN         │
                    └─────────────┘
```

### Breadboard Layout
![Breadboard](https://github.com/user-attachments/assets/a6959013-7d82-4b2a-a90b-ff521826e9b0)

### PCB Design
![PCB](https://github.com/user-attachments/assets/7a823baf-9e43-475a-9abe-a714b842f029)

---

## 📡 API Documentation

### Blynk Virtual Pins

#### Write Pins (ESP → App)
| Pin | Data Type | Range | Description |
|-----|-----------|-------|-------------|
| V0 | Float | 0-400 | Cholesterol (mg/dL) |
| V1 | Float | 0-500 | Glucose (mg/dL) |
| V2 | Float | 0-15 | Uric Acid (mg/dL) |
| V5 | String | - | Gender ("Laki-laki"/"Perempuan") |

#### Read Pins (App → ESP)
```cpp
BLYNK_WRITE(V0) { 
  // Sync cholesterol from app
  float kol = param.asFloat();
}
```

### Serial Commands
```
Command: L or l
Action: Set gender to Male
Response: "Gender: Laki-laki"

Command: P or p  
Action: Set gender to Female
Response: "Gender: Perempuan"
```

---

## 🐛 Troubleshooting

### WiFi Connection Issues
```
Problem: "WiFi Failed" on LCD
Solutions:
1. Check SSID/password spelling
2. Ensure 2.4GHz WiFi (ESP8266 doesn't support 5GHz)
3. Move closer to router
4. Check router firewall settings
```

### Blynk Connection Failed
```
Problem: "Blynk Failed" on LCD
Solutions:
1. Verify Auth Token is correct
2. Check internet connection
3. Ensure Blynk server is online
4. Re-sync virtual pins (Blynk.syncVirtual)
```

### Inaccurate Readings
```
Problem: Values don't match reference meter
Solutions:
1. Recalibrate sensor (see Calibration section)
2. Clean sensor surface
3. Ensure firm finger placement
4. Check if finger is warm (cold fingers affect readings)
5. Add more calibration points
```

### "Data tidak stabil" Message
```
Problem: Measurement fails with instability error
Solutions:
1. Place finger more firmly on sensor
2. Reduce hand movement
3. Adjust margin_stabil (default: 2)
4. Increase jumlah_pembacaan (default: 5)
```

### LCD Not Displaying
```
Problem: Blank LCD screen
Solutions:
1. Check I2C address (default: 0x27, try 0x3F)
2. Verify SDA/SCL connections
3. Adjust backlight potentiometer
4. Test I2C scanner sketch
```

### Sensor Not Detecting Finger
```
Problem: ADC stays high (>70) even with finger placed
Solutions:
1. Adjust threshold_jari (default: 70)
2. Check sensor power supply (3.3V)
3. Verify analog pin connection (A0)
4. Test with Serial Monitor to see raw ADC values
```

---

## 🤝 Contributing

Contributions are welcome! Please follow these steps:

1. **Fork the repository**
2. **Create feature branch**
   ```bash
   git checkout -b feature/AmazingFeature
   ```
3. **Commit changes**
   ```bash
   git commit -m 'Add some AmazingFeature'
   ```
4. **Push to branch**
   ```bash
   git push origin feature/AmazingFeature
   ```
5. **Open Pull Request**

### Development Guidelines
- Follow Arduino style guide
- Comment complex algorithms
- Test on hardware before PR
- Update documentation if needed


---

## 🎓 Acknowledgments

- **Institut Teknologi Del** - Academic support
- **Blynk Team** - IoT platform
- **ESP8266 Community** - Arduino core
- **Maxim Integrated** - MAX30105 sensor documentation
- **GCU Easy Touch** - Reference meter calibration



---

## 📸 Gallery

### Working Prototype
![Prototype](https://github.com/user-attachments/assets/f1d80dcf-6dce-4178-9498-204f24e1bf3a)


### Test Results
![Results](https://github.com/user-attachments/assets/d7cfdb8f-8c32-45e4-a2b9-e56cd3bfcfb0)
![Results](https://github.com/user-attachments/assets/5b6cd5e7-7123-4a43-8e25-3235e6fe676a)

---


