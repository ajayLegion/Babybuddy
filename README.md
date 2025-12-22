#  BabyBuddy – TinyML Based Baby Monitoring System
[](babybuddy.jpg)

## 📌 Project Overview

**BabyBuddy** is an ESP32-based smart baby monitoring system designed to observe fetal and maternal health parameters using embedded sensors and TinyML.
The system monitors **baby heart rate**, **mother heart rate**, **kick detection**, and **motion activity**, then applies a **TinyML model** to determine the **baby’s mood** (CALM, ACTIVE, ALERT).
All results are displayed on an **OLED graphical interface**.

This project demonstrates the integration of **IoT, Embedded Systems, Signal Processing, MATLAB, and TinyML**.

---

## 🎯 Objectives

* Monitor baby heart rate generated from MATLAB
* Measure mother heart rate using MAX30102
* Detect baby kicks using SW-420 vibration sensor
* Measure movement using MPU6050
* Apply TinyML for baby mood classification
* Display real-time data on OLED
* Develop a low-power, non-invasive monitoring solution

---

## 🧠 Baby Mood Classification (TinyML Output)

| Mood       | Description                             |
| ---------- | --------------------------------------- |
| **CALM**   | Normal heart rate, low motion, no kicks |
| **ACTIVE** | Kick detected or increased movement     |
| **ALERT**  | Abnormal heart rate or excessive motion |

---

## 🔧 Hardware Components

| Component    | Description                             |
| ------------ | --------------------------------------- |
| ESP32        | Main controller                         |
| MAX30102     | Mother heart rate sensor                |
| MPU6050      | Motion & acceleration sensor            |
| SW-420       | Kick / vibration detection              |
| SSD1306 OLED | Graphical display                       |
| 3.5mm Jack   | Baby heartbeat audio input (via MATLAB) |
| Jumper Wires | Connections                             |
| Breadboard   | Prototyping                             |

---

## 🔌 Pin Connections

### 🟦 I2C Devices

| Device   | SDA     | SCL     |
| -------- | ------- | ------- |
| OLED     | GPIO 21 | GPIO 22 |
| MPU6050  | GPIO 21 | GPIO 22 |
| MAX30102 | GPIO 21 | GPIO 22 |

### 🟨 Digital Inputs

| Sensor | ESP32 Pin |
| ------ | --------- |
| SW-420 | GPIO 14   |

---

## 🖥️ Software & Libraries

### Arduino IDE Libraries

* Adafruit SSD1306
* Adafruit GFX
* Adafruit MPU6050
* MAX30105
* TensorFlowLite_ESP32
* Wire.h

### MATLAB

* Generates baby heart rate values
* Sends heart rate via Serial to ESP32

---

## 🧪 MATLAB Baby Heart Rate Generation

MATLAB simulates fetal heartbeat signals and sends heart rate values to ESP32 using serial communication.

```matlab
hr = randi([120 160],1,1);
fprintf(serialportObj,"%d\n",hr);
pause(1);
```

---

## 🧠 TinyML Implementation

* Uses **TensorFlow Lite for Microcontrollers**
* Lightweight embedded model
* Inputs:

  * Baby HR
  * Mother HR
  * Kick status
  * Motion magnitude
* Output:

  * Baby Mood classification

✔ No cloud processing
✔ Runs fully on ESP32
✔ Low memory footprint

---

## 📊 OLED Display Features

* Graphical layout
* Separate HR sections
* Motion & kick status
* Highlighted baby mood banner
* Real-time updates

---

## 📐 System Flow

```
Sensors → ESP32 → TinyML Inference → OLED Display
            ↑
         MATLAB HR
```

---

## 📷 Sample OLED Output

```
BABYBUDDY
Baby HR : 142 bpm
Mom HR  : 76 bpm
Kick    : YES
Motion  : ACTIVE
Mood    : ACTIVE
```

---

## ⚡ Advantages

* Non-invasive monitoring
* Low power consumption
* On-device AI (TinyML)
* Cost-effective
* Real-time feedback

---

## 🚀 Applications

* Prenatal health monitoring
* Smart maternity systems
* Medical IoT research
* Academic TinyML projects
* Healthcare monitoring systems

---

## 🔮 Future Enhancements

* Real fetal heartbeat sensor integration
* Wireless mobile app
* Cloud data storage
* Emergency alert system
* Trained Edge Impulse model
* NeoPixel mood indicators

---

## 🎓 Academic Value

* Demonstrates **TinyML deployment**
* Combines **MATLAB + IoT + AI**
* Suitable for:

  * Mini Project
  * Final Year Project
  * Research Demonstration
  * Hackathons

---

## 👨‍💻 Author

**Project Name:** BabyBuddy
**Domain:** Embedded Systems, TinyML, Healthcare IoT
**Platform:** ESP32
**Language:** Arduino C++

---

## 📄 License
