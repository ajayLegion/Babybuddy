#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <MAX30105.h>
#include "heartRate.h"

// ---------------- OLED ----------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------------- Sensors ----------------
Adafruit_MPU6050 mpu;
MAX30105 particleSensor;

// ---------------- Pins ----------------
#define SW420_PIN 14

// ---------------- Variables ----------------
int babyHR = 0;          // From MATLAB
int motherHR = 0;
bool kickDetected = false;
String motionState = "STABLE";
String babyMood = "CALM";

// MAX30102 HR variables
const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);
  pinMode(SW420_PIN, INPUT);
  Wire.begin();

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1);
  }

  // MPU6050
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // MAX30102
  particleSensor.begin(Wire, I2C_SPEED_STANDARD);
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);

  displayMessage("BabyBuddy Ready");
  delay(1500);
}

// ---------------- LOOP ----------------
void loop() {
  readBabyHR();
  readMotherHR();
  detectKick();
  detectMotion();
  babyMoodAI();      // AI LOGIC
  updateOLED();
  delay(300);
}

// ---------------- FUNCTIONS ----------------

// Baby HR from MATLAB
void readBabyHR() {
  if (Serial.available()) {
    babyHR = Serial.parseInt();
    while (Serial.available()) Serial.read();
  }
}

// Mother HR
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
      for (byte i = 0; i < RATE_SIZE; i++) motherHR += rates[i];
      motherHR /= RATE_SIZE;
    }
  }
}

// Kick detection
void detectKick() {
  kickDetected = digitalRead(SW420_PIN);
}

// Motion detection
float accMag = 0;
void detectMotion() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  accMag = sqrt(
    a.acceleration.x * a.acceleration.x +
    a.acceleration.y * a.acceleration.y +
    a.acceleration.z * a.acceleration.z
  );

  if (accMag > 12.5)
    motionState = "ACTIVE";
  else
    motionState = "STABLE";
}

// ---------------- AI BABY MOOD LOGIC ----------------
void babyMoodAI() {

  // ALERT conditions
  if (babyHR < 110 || babyHR > 160) {
    babyMood = "ALERT";
    return;
  }

  if (kickDetected && accMag > 13.5) {
    babyMood = "ALERT";
    return;
  }

  // ACTIVE conditions
  if (kickDetected || motionState == "ACTIVE") {
    babyMood = "ACTIVE";
    return;
  }

  // Default
  babyMood = "CALM";
}

// ---------------- OLED ----------------
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

  display.print("Mood(AI)  : ");
  display.println(babyMood);

  display.display();
}

// Startup message
void displayMessage(String msg) {
  display.clearDisplay();
  display.setCursor(10, 25);
  display.println(msg);
  display.display();
}
