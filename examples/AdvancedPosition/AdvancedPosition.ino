// ================================================================
// UNO R4 WIFI ONLY
// This example only works on the Arduino UNO R4 WiFi because it
// uses the board's onboard 12x8 LED matrix.
// ================================================================

#if !defined(ARDUINO_UNOR4_WIFI)
#error "AdvancedPosition only works on the Arduino UNO R4 WiFi."
#endif

#include <Enes100.h>
#include <Arduino_LED_Matrix.h>

// ENES100 arena dimensions in meters.
const float ARENA_WIDTH  = 4.0f;  // X: 0.0 m to 4.0 m
const float ARENA_HEIGHT = 2.0f;  // Y: 0.0 m to 2.0 m

// UNO R4 WiFi onboard LED matrix dimensions.
const int MATRIX_COLS = 12;
const int MATRIX_ROWS = 8;

const char TEAM_NAME[] = "Your Team Name";
const int ARUCO_ID = 205;
const int ROOM_NUMBER = 1116;

ArduinoLEDMatrix matrix;
uint8_t frame[MATRIX_ROWS][MATRIX_COLS] = {0};

int lastCol = -1;
int lastRow = -1;
bool markerWasVisible = false;

// Convert an arena X coordinate (0..4 m) to the nearest LED column (0..11).
int xToColumn(float x) {
  x = constrain(x, 0.0f, ARENA_WIDTH);
  return (int)roundf((x / ARENA_WIDTH) * (MATRIX_COLS - 1));
}

// Convert an arena Y coordinate (0..2 m) to the nearest LED row (0..7).
// Matrix row 0 is the TOP, so the Y axis is inverted here:
//   y = 0 m -> bottom row
//   y = 2 m -> top row
int yToRow(float y) {
  y = constrain(y, 0.0f, ARENA_HEIGHT);
  int fromBottom = (int)roundf((y / ARENA_HEIGHT) * (MATRIX_ROWS - 1));
  return (MATRIX_ROWS - 1) - fromBottom;
}

void clearPositionDisplay() {
  memset(frame, 0, sizeof(frame));
  matrix.renderBitmap(frame, MATRIX_ROWS, MATRIX_COLS);
  lastCol = -1;
  lastRow = -1;
}

void showPosition(float x, float y) {
  int col = xToColumn(x);
  int row = yToRow(y);

  // Do not redraw if the marker still maps to the same LED.
  if (col == lastCol && row == lastRow) {
    return;
  }

  memset(frame, 0, sizeof(frame));
  frame[row][col] = 1;
  matrix.renderBitmap(frame, MATRIX_ROWS, MATRIX_COLS);

  lastCol = col;
  lastRow = row;
}

void setup() {
  Serial.begin(115200);

  matrix.begin();
  clearPositionDisplay();

  // UNO R4 WiFi uses its onboard WiFi, so no RX/TX pins are needed.
  Enes100.begin(TEAM_NAME, MATERIAL, ARUCO_ID, ROOM_NUMBER);
}

void loop() {
  if (Enes100.isConnected() && Enes100.isVisible()) {
    float x = Enes100.getX();
    float y = Enes100.getY();

    showPosition(x, y);
    markerWasVisible = true;

    // Optional serial output so you can compare the physical position
    // with the LED selected on the matrix.
    Serial.print("x = ");
    Serial.print(x, 3);
    Serial.print(" m, y = ");
    Serial.print(y, 3);
    Serial.print(" m -> LED col ");
    Serial.print(xToColumn(x));
    Serial.print(", row ");
    Serial.println(yToRow(y));
  }
  else if (markerWasVisible) {
    // Turn all LEDs off if the vision system loses the marker.
    clearPositionDisplay();
    markerWasVisible = false;
  }

  delay(50);
}
