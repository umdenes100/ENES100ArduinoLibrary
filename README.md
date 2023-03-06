# Enes100ArduinoLibrary

An Arduino library for use in the ENES100 course with Vision System over WiFi

## Download and Installation <a name="download"></a>

To download this library, click on the blue **Download** button on the right. Make sure the file is unzipped and move it to the arduino libraries folder (Documents->Arduino->libraries). You must have Arduino IDE version 1.5.0 or above. The most current version of Arduino IDE can be downloaded from the [Arduino website](https://www.arduino.cc/en/Main/Software).

**If you have an older version of the library on your computer, you _must_ delete it before adding a newer version.** Failure to do this may cause file conflicts and it is not guaranteed that the library will work properly.

## Setup<a name="setup"></a>

Communication with the Vision System is done using ESP8266 WiFi-enabled microcontrollers. WiFi modules are available for checkout through a Teaching Fellow. The WiFi module has 4 pins:

| Pin | Description     | Connect to...       |
|-----|-----------------|---------------------|
| GND | Ground          | Common ground       |
| VCC | Voltage supply  | +5 V                |
| TX  | Serial transmit | Arduino digital pin |
| RX  | Serial receive  | Arduino digital pin |

When choosing the pins to use for communication, there are a few things to keep in mind. You might not want to use PWM pins for your communication so you can save those for things like controlling your motors. That choice simply depends on how many PWM pins you need for your other peripherals. In addition, some pins aren't configured to transmit and recieve. You can find out if a certain pin is suitable to transmit or recieve by looking at the datasheet for your arduino, or by uploading the example code and seeing if you are able to recieve location coordinates. 

## Variables<a name="var"></a>

The Aruco Marker has 3 values
   * x: x-coordinate of the Aruco Marker (from 0.0 to 4.0)
   * y: y-coordinate of the Aruco Marker (From 0.0 to 2.0)
   * theta: angle of the Aruco Marker (from -pi radians to pi radians)

These values are stored in the associated variables
   * Enes100.location.x
   * Enes100.location.y
   * Enes100.location.theta

When you call `Enes100.updateLocation()` the Aruco marker values will be updated.

In additon, the location of the mission site is also stored in the following variables
  * Enes100.missionSite.x
  * Enes100.missionSite.y
## Usage

To use the library, you have to direct the compiler to include it in your code. Go to **Sketch > Include Library > ENES100**, or add it manually by typing
`#include "Enes100.h"` at the very top of your file.

### Enes100.begin
Format:

```Enes100.begin(const char* teamName, byte teamType, int markerId, int wifiModuleRX, int wifiModuleTX)```
Initializes the ENES100 library and establishes communication with the Vision System.

The `txPin` and `rxPin` described below refer to the digital pins that will be connected to the __Tx__ and __Rx__ of the __wifi module__. 
* teamName: Name of the team that will show up in the Vision System
* teamType: Type of mission your team is running. 
    *  Valid Mission Types:  `CRASH_SITE`, `DATA`, `MATERIAL`, `FIRE`, `WATER`
* markerID: ID of your Aruco Marker
* txPin: Digital Pin that will be connected to the __Tx pin on the wifi module__.
* rxPin: Digital Pin that will be connected to the __Rx pin on the wifi module__.

Note: you can use any digital pin __except 0 or 1__ for the rxPin and txPin. This means that on an Arduino Uno you can use any pin from 2 to 13.
#### Example
```arduino
void setup() {
    //Connect digital pin 8 to the Rx pin of the wifi module.
    //Connect digital pin 9 to the Tx pin of the wifi module.
    Enes100.begin("It's lit", FIRE, 3, 8, 9);
    
    // Some other setup code...
}
```

Returns a `bool` to indicate if the Vision System received your request to start the mission. You can use this return value to make your OTV wait for affirmative communication before starting.
   * 0: Did not work
   * 1: Success
The coordinates of your mission site are transmitted automatically when you call `begin()`. You can access the coordinates using

### <span style="color:red">Enes100.updateLocation()<a name="update"></a>
Requests an update of your OTV's location.

Returns a `bool` indicating if the request for the locaiton succeded or not.
   * 0: Did not work
   * 1: Success

```arduino
Enes100.updateLocation(); 
```
### <span style="color:red">Enes100.ping()<a name="ping"></a>
Sends a ping to the Vision System. The Vision System will attempt to reply back.

Returns a `bool` indicating if the operation was successfull or not.
   * 0: Did not work
   * 1: Success
### <span style="color:red">Enes100.print(Type message)<a name="print"></a>
Sends a message to the vision system. Note that any 'print' or 'println' called after will begin their message on the same line.

Can accept
   * Strings
   * Integers
   * Doubles
```arduino
These two lines will output "Hello World!Hello World!"
Enes100.print("Hello World!")
Enes100.print("Hello World!")
```
### <span style="color:red">Enes100.println(Type message)<a name="println"></a>
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

### <span style="color:red">Enes100.mission(Int type, Int message)<a name="mission"></a>
Sends value for a mission objective.

* type: what type of mission call you are sending
* message: mission value associated with the mission type. 

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

## Machine Learning Functions

### <span style="color:red">int Enes100.MLGetPrediction()<a name="ml_pred"></a>
Sends current image from the ESPCAM to the Jetson to get processed by machine learning algorithm on the Jetson.
Returns the index of the category that the model predicts.

#### **NOTE**: 
You **MUST** have Python script running on Jetson in order for this function to return anything. See provided Jetson documentation for more information on writing that script.

Example:
If the Jetson contained the categories:
**Thumbs Up**, **Thumbs Down**, **Thumb Sideways** 
in an array in that order, calling `Enes100.MLGetPrediction()` would return `0` if **Thumbs Up** is predicted, `1` if **Thumbs Down** is predicted, and `2` if **Thumb Sideways** is predicted.

### <span style="color:red">Enes100.MLCaptureTrainingImage(String label)<a name="ml_cap"></a>
Sends current image from the ESPCAM to the Jetson for storage for use in training a model later. 
String 'label' is the category of the image you are sending.
Image will be stored on the Jetson in a folder sith the name of the label you provide, so **make sure you keep names consistent**.
For recommendations on best methods for collecting data, see provided Jetson documentation for more information.

## Example Code and Debugging<a name="ex"></a>

Example code for each type of mission is included with the library. To view examples, open Arduino IDE and go to **File > Examples > ENES100**. (You must restart the IDE after installing the library for the examples to show.)

## Product Demonstration Procedures<a name="prod"></a>

During the product demonstration, messages sent using `print()` and `println()` will not be shown on the Vision System console. The console will only print out the values that you send using `mission()`.
