# Bench test: UNO R4 WiFi + ML camera (camera-module mode)

There are no automated hardware tests for this path. Run this checklist on the bench before handing the library to students.

## Setup

- UNO R4 WiFi, ML camera on the ENES100 PCB with current (unmodified) camera firmware.
- Wiring: camera TX -> R4 D0 (RX), camera RX -> R4 D1 (TX), GD -> GND, 5V -> 5V.
- Camera switch in the vision position.
- Vision system running for the room, a model uploaded for the team name used in the sketch, and an ArUco marker with the sketch's ID in view.
- Sketch: `examples/Machine_Learning_R4`, with team name, marker ID and room edited. Board: Arduino UNO R4 WiFi.

## Checks

| # | Step | Expected |
|---|------|----------|
| 1 | Upload with the camera connected | Upload succeeds (D0/D1 are not the USB port on the R4) |
| 2 | Power up and watch the vision system | Team appears as connected within about 10 s; "Connected..." shows in the team console |
| 3 | Let the sketch loop | "Requesting prediction..." then a prediction every ~5 s; the captured image shows in the team's ML panel; the value matches what the same camera and model give on a classic Uno |
| 4 | Note "The prediction took" times | Well under 5000 ms; a `-1` after ~5000 ms means no reply reached the R4 |
| 5 | Add `Enes100.println(Enes100.getX())`, `getY()`, `getTheta()`, `isVisible()` to `loop()` | Values match the vision system display; covering the marker gives `isVisible()` false and -1 values |
| 6 | Add an `Enes100.mission(...)` call for your mission type | The formatted mission message appears in the team console |
| 7 | Unplug the camera's 5V for a few seconds, then reconnect | Predictions return `-1` while unplugged; the team reconnects and predictions resume without resetting the R4 |
| 8 | Press reset on the R4 only | Team stays or returns to connected, predictions resume |
| 9 | Flip the camera switch to training and back to vision | Calls fail while in training; they recover after switching back |
| 10 | Request a model index that does not exist | `MLGetPrediction()` returns `-1` (after the 5 s timeout) and the sketch keeps running |
| 11 | Leave it running for 15 minutes | No lockups, no growing delays |

## Regression checks

- Classic Uno + camera on D3/D2 with `examples/Machine_Learning`: behaves as before.
- UNO R4 WiFi with onboard WiFi (`Enes100.begin("Team", SEED, 381, 1116)`, no camera) and `examples/SimpleLocation`: connects and reports location as before; `MLGetPrediction()` returns `-1`.

## If it does not work

- Nothing at all, `Enes100.isConnected()` never true: swap the D0/D1 wires, check the switch position, check 5V/GND.
- Add `Enes100.debugDump(Serial);` after `Serial.begin(9600)`: `state() = 255` means the camera is not answering on the serial link; `0` means it answers but has not accepted the team information yet; `1` means the link is up, so look at WiFi and the vision system side.
- Connected but every prediction is `-1`: check the team name matches the uploaded model's team name exactly and that the model index exists.
