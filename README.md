# AEP Robotic Arm Capstone — TinkerCAD

Manual and autonomous three-joint arm simulation for the Aurora Educators Program Robotic Arms track.

## TinkerCAD circuit

https://www.tinkercad.com/things/gu5aTrdx2sm/editel

Parts: Arduino Uno, three servos, three potentiometers, four-pin HC-SR04, buzzer, slide switch, and jumper wires.

## Pin map

| Component | Connection |
|---|---|
| Base / shoulder / elbow servo signal | D3 / D5 / D6 |
| Potentiometer wipers | A0 / A1 / A2 |
| HC-SR04 TRIG / ECHO | D7 / D8 |
| HC-SR04 VCC / GND | 5V / GND |
| Buzzer + / − | D4 / GND |
| Slide switch common / one outer terminal | D2 / GND; other outer terminal unused |

The slide switch uses `INPUT_PULLUP`: LOW selects manual mode, HIGH selects autonomous mode. In manual mode, each potentiometer maps to one servo angle. In autonomous mode, the base sweeps and the sketch uses a valid sensor distance below 20 cm to trigger a short buzzer chirp and a programmed shoulder/elbow reach-and-return pose. A sensor timeout is reported and does not trigger that pose.

## Implementation notes

The sketch in this repository is the TinkerCAD-targeted version using the built-in Servo library and a four-pin ultrasonic sensor. `diagram.json` and `libraries.txt` remain from an older Wokwi draft; they are not the required TinkerCAD circuit or evidence.

## Evidence status

The TinkerCAD draft currently has one four-pin HC-SR04 after removal of an obsolete sensor and a duplicate. The latest browser simulation still reported “No echo; autonomous search continues.” Sensor behavior and both operating modes therefore remain unverified. The committed images and schematic are earlier snapshots, not completion evidence. Replace them after confirming the circuit in TinkerCAD.

## Learning reflection

`WHAT_I_LEARNED.md` is a draft reflection. Edit it to include only learning experiences that accurately describe the student's own classes and work.
