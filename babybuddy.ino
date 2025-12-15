#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "MAX30105.h"
#include "heartRate.h"

// -------------------- OLED --------------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// -------------------- Sensors --------------------
Adafruit_MPU6050 mpu;
MAX30105 particleSensor;

// -------------------- Pins --------------------
#define SW420_PIN 14   // Kick sensor

// -------------------- Variables --------------------
int babyHR = 0;        // From MATLAB
int motherHR = 0;
bool kickDetected = false;
String motionState = "STABLE";

// MAX30102 variables
const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;

// -------------------- SETUP --------------------
void setup() {
  Serial.begin(115200);
  pinMode(SW420_PIN, INPUT);

  Wire.begin();

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while (1);
  }
  display.clearDisplay();

  // MPU6050 init
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found");
    while (1);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // MAX30102 init
  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("MAX30102 not found");
    while (1);
  }

  particleSensor.setup(); // default settings
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);

  displayMessage("System Ready");
  delay(1500);
}

// -------------------- LOOP --------------------
void loop() {
  readBabyHRFromMATLAB();
  readMotherHR();
  detectKick();
  detectMotion();
  updateOLED();
  delay(300);
}

// -------------------- FUNCTIONS --------------------

// Read Baby HR from MATLAB via Serial
void readBabyHRFromMATLAB() {
  if (Serial.available()) {
    babyHR = Serial.parseInt();
    while (Serial.available()) Serial.read(); // clear buffer
  }
}

// Mother HR from MAX30102
void readMotherHR() {
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue)) {
    long delta = millis() - lastBeat;
    lastBeat = millis();

    float bpm = 60 / (delta / 1000.0);

    if (bpm > 40 && bpm < 180) {
      rates[rateSpot++] = (byte)bpm;
      rateSpot %= RATE_SIZE;

      motherHR = 0;
      for (byte i = 0; i < RATE_SIZE; i++) {
        motherHR += rates[i];
      }
      motherHR /= RATE_SIZE;
    }
  }
}

// Kick detection using SW-420
void detectKick() {
  kickDetected = digitalRead(SW420_PIN);
}

// Motion detection using MPU6050
void detectMotion() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float accMag = sqrt(
    a.acceleration.x * a.acceleration.x +
    a.acceleration.y * a.acceleration.y +
    a.acceleration.z * a.acceleration.z
  );

  if (accMag > 12.5) motionState = "ACTIVE";
  else motionState = "STABLE";
}

// OLED update
void updateOLED() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("Baby HR   : ");
  display.print(babyHR);
  display.println(" bpm");

  display.print("Mother HR : ");
  display.print(motherHR);
  display.println(" bpm");

  display.print("Kick      : ");
  display.println(kickDetected ? "YES" : "NO");

  display.print("Motion    : ");
  display.println(motionState);

  display.display();
}

// Display startup message
void displayMessage(String msg) {
  display.clearDisplay();
  display.setCursor(0, 20);
  display.setTextSize(1);
  display.println(msg);
  display.display();
}
