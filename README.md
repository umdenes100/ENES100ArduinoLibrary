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

```arduino
#include "Enes100.h"
```

at the very top of your file.

### Enes100.begin(String Team\_Name,Type Mission\_Type,Int Aruco\_ID,Int RX,Int TX)
Just like the built-in `Serial` interface, you need to initialize the library by calling `begin()`.

* Team_Name: Name of the team that will show up in the Vision System
* Mission_Type: Type of mission your team is running. 
    *  Valid Mission Types:  `CRASH_SITE`, `DATA`, `MATERIAL`, `FIRE`, `WATER`
* Aruco_ID: ID of your Aruco Marker
* Rx: Digital Recieve Pin that will be connected to the Rx pin on the wifi module.
* Tx: Digital Transmit Pin that will be connected to the Tx pin on the wifi module.

```arduino
void setup() {
    Enes100.begin("It's lit", FIRE, 3, 8, 9);
    
    // Some other setup code...
}
```

The `begin()` method returns a `bool` to indicate if the Vision System received your request to start the mission. You can use this return value to make your OTV wait for affirmative communication before starting.

The coordinates of your mission site are transmitted automatically when you call `begin()`. You can access the coordinates using

```arduino
Enes100.missionSite.x; // x Coordinate
Enes100.missionSite.y; // y Coordinate
```

### Enes100.updateLocation()
Requests an update of your OTV's location.

Returns a `bool` indicating if it succeeded or not.
   * 0 : Did not work
   * 1 : Success

```arduino
while (!Enes100.updateLocation()); 
```

### Enes100.print()
Sends a message to the vision system. Note that any 'print' or 'println' called after will begin their message on the same line.

Can accept
   * Strings
   * Integers\
   * Doubles
```arduino
These two lines will output "Hello World!Hello World!"
Enes100.print("Hello World!")
Enes100.print("Hello World!")
```
### Enes100.println()
Sends a message to the vision system with a new line. Any messages sent after will be printed in a new line below the 'println'
```arduino
These two lines will output 
// Hello World!
// Hello World!
Enes100.println("Hello World!")
Enes100.println("Hello World!")
```
Can accept
   * Strings
   * Integers
   * Doubles

```arduino
Enes100.print(“Our x coordinate is: “);
Enes100.println(enes.location.x);
```
### Coordinates
The x and y coordinates are the distance in meters from the y axis and x axis to the center of your marker. Theta is measured in radians from -π to π, with zero being parallel to the x axis, increasing counter-clockwise.

### Enes100.mission(int type, int message)
As your OTV completes its objectives, it will need to alert the Vision System. When your OTV completes one of the objectives 
that requires transmission, call the `mission(int type, int message)` function with appropriate values. 

There are two arguments associated with this function `mission(arg1, arg2)`. 
- **arg1** is the argument specifying what type of mission call you are sending
- **arg2** is the argument specifying the mission value associated with the mission type. 

**NOTE:** Some of the values passed as arguments in the examples below are **C definitions**, defined in `Enes100.h`. All of 
the C definitions defined in `Enes100.h` correlate to an integer. To save you the trouble, you can call the uppercase definition 
like `LENGTH` for Crash Site teams or `SQUISHABILITY` for Material Identification teams. 

For the valid mission calls below, the value `x` will denote an integer value.

Valid calls for **CRASH_SITE**:

 * `Enes100.mission(LENGTH, x);` *x is in millimeters*
 * `Enes100.mission(HEIGHT, x);` *x is in millimeters*
 * `Enes100.mission(DIRECTION, POS_X);`
 * `Enes100.mission(DIRECTION, NEG_X);`
 * `Enes100.mission(DIRECTION, POS_Y);`
 * `Enes100.mission(DIRECTION, NEG_Y);`

Valid calls for **DATA**:

 * `Enes100.mission(CYCLE, x);` *x is the duty cycle percent (ex. 10, 30, 50, 70, 90)*
 * `Enes100.mission(MAGNETISM, MAGNETIC);`
 * `Enes100.mission(MAGNETISM, NOT_MAGNETIC);`

Valid calls for **MATERIAL**:

 * `Enes100.mission(WEIGHT, HEAVY);`
 * `Enes100.mission(WEIGHT, MEDIUM);`
 * `Enes100.mission(WEIGHT, LIGHT);`
 * `Enes100.mission(MATERIAL_TYPE, FOAM);`
 * `Enes100.mission(MATERIAL_TYPE, PLASTIC);`

Valid calls for **FIRE**:

 * `Enes100.mission(NUM_CANDLES, x);` *x is an integer (0, 1, 2, 3, 4, 5)*
 * `Enes100.mission(TOPOGRAPHY, TOP_A);`
 * `Enes100.mission(TOPOGRAPHY, TOP_B);`
 * `Enes100.mission(TOPOGRAPHY, TOP_C);`

Valid calls for **WATER**:

 * `Enes100.mission(DEPTH, x);` *x is in mm*
 * `Enes100.mission(WATER_TYPE, FRESH_UNPOLLUTED);`
 * `Enes100.mission(WATER_TYPE, FRESH_POLLUTED);`
 * `Enes100.mission(WATER_TYPE, SALT_UNPOLLUTED);`
 * `Enes100.mission(WATER_TYPE, SALT_POLLUTED);`


## Example Code and Debugging

Example code for each type of mission is included with the library. To view examples, open Arduino IDE and go to **File > Examples > ENES100**. (You must restart the IDE after installing the library for the examples to show.)

The library also includes a `ping()` function which sends an empty message to the Vision System and waits for an empty reply. You can use this function to troubleshoot communication problems.

## Function List

The following are functions of the `Enes100` class.

`bool ping()`   
Returns: `true` on success, `false` on failure.   
Confirms that communication with the Vision System is still active.

`bool begin(teamName, teamType, markerId, txPin, rxPin)`   
Returns: `true` on success, `false` on failure.   
Initializes the ENES100 library and establishes communication with the Vision System.

`bool updateLocation()`   
Returns: `true` on success, `false` on failure.   
Updates the OTV's location information.

`void mission(type, value)`   
Sends value for a mission objective.

`void print(message)`  
Sends a message to be displayed on the Vision System.

`void println(message)`  
Sends a message to be displayed on the Vision System followed by a new line character.

## Product Demonstration Procedures

During the product demonstration, messages sent using `print()` and `println()` will not be shown on the Vision System console. The console will only print out the values that you send using `mission()`.
