// AEP capstone: three-joint robotic arm with manual and autonomous modes.
#include <Servo.h> // Include Arduino's built-in Servo library.
Servo baseServo; // Servo for the rotating base.
Servo shoulderServo; // Servo for the shoulder joint.
Servo elbowServo; // Servo for the elbow joint.
const byte trigPin = 7; // HC-SR04 trigger pin.
const byte echoPin = 8; // HC-SR04 echo pin.
const byte buzzerPin = 4; // Buzzer output pin.
const byte modePin = 2; // Slide switch input, using INPUT_PULLUP.
const int nearThresholdCm = 20; // A valid object closer than this triggers the action.
int sweepPosition = 0; // Current base search angle.
int sweepDirection = 1; // Positive or negative direction for the base sweep.
long readDistanceCm() { // Return approximate distance or -1 if no echo arrives.
  digitalWrite(trigPin, LOW); // Start with trigger low.
  delayMicroseconds(2); // Allow the trigger line to settle.
  digitalWrite(trigPin, HIGH); // Start the sensor trigger pulse.
  delayMicroseconds(10); // Hold the trigger pulse for 10 microseconds.
  digitalWrite(trigPin, LOW); // End the sensor trigger pulse.
  unsigned long echoTimeUs = pulseIn(echoPin, HIGH, 30000UL); // Read echo duration with a maximum wait.
  if (echoTimeUs == 0) { // Check whether the sensor timed out.
    return -1; // Mark the distance as unknown.
  } // End timeout check.
  return (long)(echoTimeUs * 0.0343f / 2.0f); // Convert round-trip echo duration to centimetres.
} // End distance-reading function.
void manualMode() { // Control each arm joint from its own potentiometer.
  int baseAngle = map(analogRead(A0), 0, 1023, 0, 180); // Map A0 to the base angle.
  int shoulderAngle = map(analogRead(A1), 0, 1023, 0, 180); // Map A1 to the shoulder angle.
  int elbowAngle = map(analogRead(A2), 0, 1023, 0, 180); // Map A2 to the elbow angle.
  baseServo.write(baseAngle); // Command the base joint.
  shoulderServo.write(shoulderAngle); // Command the shoulder joint.
  elbowServo.write(elbowAngle); // Command the elbow joint.
  delay(15); // Give the servos time to respond before reading again.
} // End manual mode function.
void autonomousMode() { // Search with the base and act on nearby objects.
  long distanceCm = readDistanceCm(); // Read current sensor distance.
  if (distanceCm < 0) { // Handle a missing echo safely.
    noTone(buzzerPin); // Do not alarm for an unknown measurement.
    Serial.println("No echo; autonomous search continues."); // Report the sensor timeout.
  } else { // Process a valid measurement.
    Serial.print("distance_cm="); // Print a clear distance label.
    Serial.println(distanceCm); // Print the measured centimetres.
  } // End valid/invalid measurement choice.
  if (distanceCm >= 0 && distanceCm < nearThresholdCm) { // Act only on a valid object inside 20 cm.
    tone(buzzerPin, 1000, 300); // Sound a short detection alert.
    shoulderServo.write(120); // Move shoulder to the reach position.
    elbowServo.write(60); // Move elbow to the reach position.
    delay(600); // Allow the reach pose to complete.
    shoulderServo.write(60); // Move shoulder to the return position.
    elbowServo.write(120); // Move elbow to the return position.
    delay(600); // Allow the return pose to complete.
  } else { // No valid nearby object was detected.
    noTone(buzzerPin); // Keep the buzzer silent while searching.
    sweepPosition += 2 * sweepDirection; // Advance the base scan by two degrees.
    if (sweepPosition >= 180) { // Check the upper sweep limit.
      sweepPosition = 180; // Clamp the position to the servo range.
      sweepDirection = -1; // Reverse the scan direction.
    } else if (sweepPosition <= 0) { // Check the lower sweep limit.
      sweepPosition = 0; // Clamp the position to the servo range.
      sweepDirection = 1; // Reverse the scan direction.
    } // End sweep boundary handling.
    baseServo.write(sweepPosition); // Command the base search angle.
    delay(20); // Allow the base servo to move between scan steps.
  } // End detect/search choice.
} // End autonomous mode function.
void setup() { // Configure hardware once at startup.
  baseServo.attach(3); // Attach base servo to D3.
  shoulderServo.attach(5); // Attach shoulder servo to D5.
  elbowServo.attach(6); // Attach elbow servo to D6.
  pinMode(trigPin, OUTPUT); // Configure sensor trigger output.
  pinMode(echoPin, INPUT); // Configure sensor echo input.
  pinMode(buzzerPin, OUTPUT); // Configure buzzer output.
  pinMode(modePin, INPUT_PULLUP); // Use the Uno's internal pullup for the mode switch.
  Serial.begin(9600); // Start serial status output.
  baseServo.write(90); // Start the base near its middle position.
  shoulderServo.write(90); // Start the shoulder near its middle position.
  elbowServo.write(90); // Start the elbow near its middle position.
} // End setup.
void loop() { // Select the operating mode continuously.
  bool manualSelected = digitalRead(modePin) == LOW; // LOW means the switch connects D2 to GND for manual mode.
  if (manualSelected) { // Check whether manual mode is selected.
    manualMode(); // Let potentiometers control all three joints.
  } else { // The switch is open and the pullup reads HIGH.
    autonomousMode(); // Search for an object and perform the programmed action.
  } // End mode selection.
} // End loop.
