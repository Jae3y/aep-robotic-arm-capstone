// AEP Capstone: autonomous + manually controlled robotic arm (Wokwi, Arduino Uno)
#include <Servo.h>

Servo base, shoulder, elbow;

const int TRIG = 7;    // HC-SR04 trigger
const int ECHO = 8;    // HC-SR04 echo
const int BUZZ = 4;    // buzzer
const int MODE = 2;    // slide switch: LOW = manual, HIGH = autonomous
const int NEAR_CM = 20; // object closer than this triggers the arm

int sweepPos = 0;
int sweepDir = 1;
bool lastManual = true;

long readCm() {
  digitalWrite(TRIG, LOW);  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long us = pulseIn(ECHO, HIGH, 30000);
  if (us == 0) return 999;            // no echo
  return (long)(us * 0.034 / 2);      // microseconds -> cm
}

void manualMode() {
  base.write(map(analogRead(A0), 0, 1023, 0, 180));
  shoulder.write(map(analogRead(A1), 0, 1023, 0, 180));
  elbow.write(map(analogRead(A2), 0, 1023, 0, 180));
  delay(15);
}

void autoMode() {
  long d = readCm();
  Serial.print("distance_cm=");
  Serial.println(d);
  if (d < NEAR_CM) {
    tone(BUZZ, 1000, 300);            // alert
    shoulder.write(120); elbow.write(60);  delay(600);  // reach out
    shoulder.write(60);  elbow.write(120); delay(600);  // pull back
  } else {
    sweepPos += 2 * sweepDir;          // scan with the base
    if (sweepPos >= 180 || sweepPos <= 0) sweepDir = -sweepDir;
    base.write(sweepPos);
    delay(20);
  }
}

void setup() {
  base.attach(3);
  shoulder.attach(5);
  elbow.attach(6);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUZZ, OUTPUT);
  pinMode(MODE, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  bool manual = (digitalRead(MODE) == LOW);
  if (manual != lastManual) {
    Serial.println(manual ? "MODE: MANUAL" : "MODE: AUTONOMOUS");
    lastManual = manual;
  }
  if (manual) manualMode(); else autoMode();
}
