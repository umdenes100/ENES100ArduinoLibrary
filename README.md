# Enes100ArduinoLibrary

Arduino library for use in the ENES100 course with Vision System v5.1 over WiFi

## Download and Installation

To download this library, click on the blue **Download** button on the right. Make sure the file is unzipped and move it to the arduino libraries folder (Documents->Arduino->libraries). You must have Arduino IDE version 1.5.0 or above. The most current version of Arduino IDE can be downloaded from the [Arduino website](https://www.arduino.cc/en/Main/Software).

**If you have an older version of the library on your computer, you _must_ delete it before adding a newer version.** Failure to do this may cause file conflicts and it is not guaranteed that the library will work properly.

## Setup

Communication with the Vision System is done using ESP8266 WiFi-enabled microcontrollers. WiFi modules are available for checkout through a Teaching Fellow. The WiFi module has 4 pins:

| Pin | Description     | Connect to...           |
| --- | ---             | ---                     |
| GND | Ground          | Common ground           |
| VCC | Voltage supply  | +5 V                    |
| TX  | Serial transmit | Arduino serial receive  |
| RX  | Serial receive  | Arduino serial transmit |

When choosing the pins to use for communication, there are a few things to keep in mind. Firstly, you will want to avoid pins 0 and 1 for communication (or for anything else). The RX and TX labels on those pins refer to the Arduino’s serial transmit and receive with the computer. If you block those pins, you will not be able to upload code to your Arduino. Secondly, you might not want to use PWM pins for your communication so you can save those for things like controlling your motors. That choice simply depends on how many PWM pins you need for your other peripherals. Thirdly, some pins aren't configured to transmit and recieve. You can find out if a certain pin is suitable to transmit or recieve by looking at the datasheet for your arduino, or by uploading the example code and seeing if you are able to recieve location coordinates. 

## Usage

To use the library, you have to direct the compiler to include it in your code. Go to **Sketch > Include Library > ENES100**, or add it manually by typing

```
#include "Enes100.h"
```

at the very top of your file.

Just like the built-in `Serial` interface, you need to initialize the library by calling `begin()`.

```
void setup() {
    Enes100.begin("It's lit", FIRE, 3, 8, 9);
    
    // Some other setup code...
}
```

The first parameter is the name of your team, which will appear on the Vision System. `FIRE` refers to the mission type of your team. The full list of valid mission names is:

 * `CRASH_SITE`
 * `DATA`
 * `DEBRIS`
 * `FIRE`
 * `WATER`

(As with all `code samples` in this document, these are case sensitive.) The third argument is the ID number of the aruco marker that your team will be using. The fourth argument is your serial transmit pin. The fifth argument is your serial receive pin.

The `begin()` method returns a `bool` to indicate if the Vision System received your request to start the mission. You can use this return value to make your OSV wait for affirmative communication before starting.

The coordinates of your mission site are transmitted automatically when you call `begin()`. You can access the coordinates using

```
Enes100.destination.x; // x Coordinate
Enes100.destination.y; // y Coordinate
```

To request an update of your OSV’s location, call the `updateLocation()` method. This method returns a `bool` indicating if it succeeded or not.

```
if (Enes100.updateLocation()) {
    Enes100.location.x; // x Coordinate
    Enes100.location.y; // y Coordinate
    Enes100.location.theta; // Theta
}
```

The x and y coordinates are the distance in meters from the y axis and x axis to the center of your marker. Theta is measured in radians from -π to π, with zero being parallel to the x axis, increasing counter-clockwise.

At points in your mission, you may want to send information to be displayed on the Vision System. Writing information to the Vision System is similar to writing information to the Serial console. The `print()` function will write a message to the console. The `println()` function will write a message to the console followed by a new line character. These functions can accept strings, integers, and doubles as arguments.

```
Enes100.print(“Our x coordinate is: “);
Enes100.println(enes.location.x);
```

As your OSV completes its objectives, it will need to alert the Vision System. When your OSV completes one of the objectives that requires transmission, call the `mission()` method with the value that you've calculated.

```
Enes100.mission(2.34);
Enes100.mission(COPPER);
```

For the Water mission, valid water types for the base objective are

 * `FRESH_UNPOLLUTED`
 * `FRESH_POLLUTED`
 * `SALT_UNPOLLUTED`
 * `SALT_POLLUTED`

For the Debris mission, valid material types are

 * `PLASTIC`
 * `COPPER`

### The Coordinate Object

The ENES100 library also includes a `Coordinate` class that holds an x, y, and theta. Students may use Coordinates in their code as desired. The `destination` and `location` members of `Enes100` are `Coordinate` objects. The x, y, and theta are accessed in the same way as in `destination` and `location`, i.e. `coordinate.x`, `coordinate.y`, and `coordinate.theta`. For convenience, we have included 3 constructors for the Coordinate object.

```
// Represents the point (0, 0, 0)
Coordinate coordinate1();
// Represents the point (1.2, 0.7, 0)
Coordinate coordinate2(1.2, 0.7);
// Represents the point (1.2, 0.7, 1.1)
Coordinate coordinate3(1.2, 0.7, 1.1);
```

## Example Code and Debugging

Example code for each type of mission is included with the library. To view examples, open Arduino IDE and go to **File > Examples > ENES100**. (You must restart the IDE after installing the library for the examples to show.)

The library also includes a `ping()` function which sends an empty message to the Vision System and waits for an empty reply. You can use this function to troubleshoot communication problems.

## Function List

The following are functions of the `Enes100` class.

`bool ping()`   
Returns: `true` on success, `false` on failure.   
Confirms that communication with the Vision System is still active.

`bool begin(teamName, teamType, markerId, rxPin, txPin)`   
Returns: `true` on success, `false` on failure.   
Initializes the ENES100 library and establishes communication with the Vision System.

`bool updateLocation()`   
Returns: `true` on success, `false` on failure.   
Updates the OSV's location information.

`void mission(value)`   
Sends value for a mission objective.

`void print(message)`  
Sends a message to be displayed on the Vision System.

`void println(message)`  
Sends a message to be displayed on the Vision System followed by a new line character.

## Competition Procedures

During the competition, messages sent using `print()` and `println()` will not be shown on the Vision System console. The console will only print out the values that you send using `mission()`.
