# ENES100 Arduino Library v6.2.4

One ENES100 library for the **Arduino Uno** and **Arduino UNO R4 WiFi**.

## Board behavior

- **Arduino Uno:** uses the external ENES100 WiFi module over `SoftwareSerial`. The WiFi-module firmware must be compatible with the current ENES100 serial protocol.
- **Arduino UNO R4 WiFi:** uses the board's onboard WiFi and only the R4 registration data.
- Other boards intentionally fail at compile time.

## `begin()`

Arduino Uno with external WiFi module:

```cpp
Enes100.begin("Team Name", MATERIAL, 205, 1116, 3, 2);
```

UNO R4 WiFi:

```cpp
Enes100.begin("Team Name", MATERIAL, 205, 1116);
```

The examples include a commented RX/TX form for classic Uno users. `AdvancedPosition` is explicitly UNO R4 WiFi-only because it uses the onboard 12x8 LED matrix.

## Debug states (classic Uno + WiFi module)

- `1`: connected
- `0`: module is responding but WiFi/WebSocket connection is still in progress
- `255`: no valid state response was received; this can occur briefly while the module boots

`lastDisconnectReason` is historical. If `wifiConnected = true`, a previous disconnect reason does not describe the current connection.


## Example initialization

The examples intentionally avoid preprocessor `#if` blocks. The simple form is used directly:

```cpp
Enes100.begin("Team Name", MATERIAL, 205, 1116);
```

On an Arduino Uno, this uses the standard external WiFi-module pins (module TX = 3, module RX = 2). If different pins are needed, the examples show the explicit form immediately below as a comment:

```cpp
// If using an Arduino Uno with an external WiFi module, you can use this instead
// to explicitly assign the WiFi module TX and RX pins:
// Enes100.begin("Team Name", MATERIAL, 205, 1116, 3, 2);
```

The four-argument form remains the normal form for the Arduino UNO R4 WiFi.
