# Enes100ArduinoLibrary

An Arduino library for use in the ENES100 course to allow Arduino boards to communicate with the ENES100 Vision System
via ESP8266 Wi-Fi modules.

## Download and Installation <a name="download"></a>

To download this library, click on the blue **Download** button on the right. Make sure the file is unzipped and move it
to the arduino libraries folder (Documents->Arduino->libraries). You must have Arduino IDE version 1.5.0 or above. The
most current version of Arduino IDE can be downloaded from
the [Arduino website](https://www.arduino.cc/en/Main/Software).

**If you have an older version of the library on your computer, you _must_ delete it before adding a newer version.**
Failure to do this may cause file conflicts, and it is not guaranteed that the library will work properly.

## Setup<a name="setup"></a>

Communication with the Vision System is done using ESP8266 WiFi-enabled microcontrollers. Wi-Fi modules are available
for
checkout through a Teaching Fellow. The Wi-Fi module has 4 pins:

| Pin | Description     | Connect to...       |
|-----|-----------------|---------------------|
| GND | Ground          | Common ground       |
| VCC | Voltage supply  | +5 V                |
| TX  | Serial transmit | Arduino digital pin |
| RX  | Serial receive  | Arduino digital pin |

Boards have limitations around what pins can be connected to Wi-Fi module TX.
Allowed Pins:

- Uno - every digital pin excluding 0,1,13
- Mega - 10, 11, 12, 13, 14, 15, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64), A11 (65), A12 (66), A13 (67), A14 (68),
  A15 (69)
  Students have had difficulty with the Mega. <small>Some students have a certain set of pins work while other students
  will find those same pins broken.</small>
- Leonardo - 8, 9, 10, 11, 14, 15, 16.
- Romeo V1 - Same as Uno
- Romeo V2 - Same as Leonardo

## Usage

To use the library, you have to direct the compiler to include it in your code. Go to **Sketch > Include Library >
ENES100**, or add it manually by typing
`#include "Enes100.h"` at the very top of your file.

### <span >Enes100.begin<a name="begin"></a>

Format:

```Enes100.begin(const char* teamName, byte teamType, int markerId, int wifiModuleTX, int wifiModuleRX)```

Initializes the ENES100 library and establishes communication with the Vision System.

The `wifiModuleTX` and `wifiModuleRX` described below refer to the digital pins that will be connected to the __Tx__ and
__Rx__ of the
__Wi-Fi module__.

* teamName: Name of the team that will show up in the Vision System
* teamType: Type of mission your team is running.
    * Valid Mission Types:  `CRASH_SITE`, `DATA`, `MATERIAL`, `FIRE`, `WATER`, `SEED`
* markerID: ID of your Aruco Marker
* wifiModuleTX: Digital Pin that will be connected to the __Tx pin on the Wi-Fi module__.
* wifiModuleRX: Digital Pin that will be connected to the __Rx pin on the Wi-Fi module__.

Boards have limitations around what pins can be connected to Wi-Fi module TX.
Allowed Pins:

- Uno - every digital pin excluding 0,1,13
- Mega - 10, 11, 12, 13, 14, 15, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64), A11 (65), A12 (66), A13 (67), A14 (68),
  A15 (69)
  Students have had difficulty with the Mega. <small>Some students have a certain set of pins work while other students
  will find those same pins broken.</small>
- Leonardo - 8, 9, 10, 11, 14, 15, 16.
- Romeo V1 - Same as Uno
- Romeo V2 - Same as Leonardo

#### **NOTE**: Want to call begin multiple times?

There are some of you who will wish to use begin in combination with the isConnected method to 'reconnect' if needed.
<b> Here is your warning.</b> The begin method allocates some resources with new/delete to allow calling the
SoftwareSerial constructor in the begin method instead of in the Enes100 object constructor (the constructor not being
student facing). You may call begin multiple times, and the resources should be freed and reallocated (untested, no
guarantees). You should consider the dangers of heap fragmentation.

My opinion and experience - the connection is rarely dropped - and if so usually because of power regulation issues.
Reconnect code is not needed. KISS.

#### Example

```arduino
void setup() {
    //Connect digital pin 8 to the Tx pin of the wifi module.
    //Connect digital pin 9 to the Rx pin of the wifi module.
    Enes100.begin("It's lit", FIRE, 3, 8, 9);
    
    // Some other setup code...
}
```

### <span >Enes100.getX() and similar<a name="update"></a>

The Aruco Marker has 4 values

* x: x-coordinate of the Aruco Marker (from 0.0 to 4.0)
* y: y-coordinate of the Aruco Marker (From 0.0 to 2.0)
* theta: angle of the Aruco Marker (from -pi radians to pi radians)
* visibility: whether the ArUco marker is visible (true or false)

These values can be queried by calling the following functions:

* `Enes100.getX()`
* `Enes100.getY()`
* `Enes100.getTheta()`
* `Enes100.isVisible()`

Enes100.get variants will make sure you get the latest data available to you about your OTV's location. The first time
getX is
called, X, Y, theta and visibility are queried and cached. Subsequent calls return from the cache, so there
is no performance gain to saving the function response to a variable.

### <span >Enes100.isConnected()<a name="isConnected"></a>

`Enes100.isConnected()`

Returns true if the ESP8266 is connected to the Vision System, false otherwise.
Note: Enes100.begin will not return until this function is true.

See my warning above about calling Enes100.begin multiple times - if you plan on using isConnecting in that fashion I
would discourage it.

### <span >Enes100.print(Type message)<a name="print"></a>

Sends a message to the vision system. Note that any 'print' or 'println' called after will begin their message on the
same line.

The function can accept most types. It uses the arduino SoftwareSerial print under the hood, so check out documentation
for that for specifics on types.

```arduino
These two lines will output "Hello World!Hello World!"
Enes100.print("Hello World!")
Enes100.print("Hello World!")
```

### <span >Enes100.println(Type message)<a name="println"></a>

Sends a message to the vision system with a new line. Any messages sent after will be printed in a new line below the '
println'

```arduino
//These two lines will output 
//   "Hello World!
//    Hello World!"
Enes100.println("Hello World!")
Enes100.println("Hello World!")
```

The function can accept most types. It uses the arduino SoftwareSerial print under the hood, so check out documentation
for that for specifics on types.

### <span >Enes100.mission(int type, int message)<a name="mission"></a>

Sends value for a mission objective.

* type: what type of mission call you are sending
* message: mission value associated with the mission type.

**NOTE:** Some of the values passed as arguments in the examples below are **C++ preprocessor definitions** (#define),
defined in `Enes100.h`.
All the C definitions defined in `Enes100.h` correlate to an integer. To save you the trouble, you can call the
uppercase definition
like `LENGTH` for Crash Site teams or `MATERIAL_TYPE` for Material Identification teams.

For the valid mission calls below, the value `i` will denote an integer value.

Valid calls for **CRASH_SITE**:

* `Enes100.mission(LENGTH, i);` *i is in millimeters*
* `Enes100.mission(HEIGHT, i);` *i is in millimeters*
* `Enes100.mission(DIRECTION, NORMAL_X);` *the normal of the exposed panels points in the positive and negative x
  direction*
* `Enes100.mission(DIRECTION, NORMAL_Y);` *the normal of the exposed panels points in the positive and negative y
  direction*

Valid calls for **DATA**:

* `Enes100.mission(CYCLE, i);` *i is the duty cycle percent (ex. 10, 30, 50, 70, 90)*
* `Enes100.mission(MAGNETISM, MAGNETIC);`
* `Enes100.mission(MAGNETISM, NOT_MAGNETIC);`

Valid calls for **MATERIAL**:

* `Enes100.mission(WEIGHT, HEAVY);`
* `Enes100.mission(WEIGHT, MEDIUM);`
* `Enes100.mission(WEIGHT, LIGHT);`
* `Enes100.mission(MATERIAL_TYPE, FOAM);`
* `Enes100.mission(MATERIAL_TYPE, PLASTIC);`

Valid calls for **FIRE**:

* `Enes100.mission(NUM_CANDLES, i);` *i is an integer (0, 1, 2, 3, 4, 5)*
* `Enes100.mission(TOPOGRAPHY, TOP_A);`
* `Enes100.mission(TOPOGRAPHY, TOP_B);`
* `Enes100.mission(TOPOGRAPHY, TOP_C);`

Valid calls for **WATER**:

* `Enes100.mission(DEPTH, i);` *i is in mm*
* `Enes100.mission(WATER_TYPE, FRESH_UNPOLLUTED);`
* `Enes100.mission(WATER_TYPE, FRESH_POLLUTED);`
* `Enes100.mission(WATER_TYPE, SALT_UNPOLLUTED);`
* `Enes100.mission(WATER_TYPE, SALT_POLLUTED);`

Valid calls for **SEED**:

* `Enes100.mission(LOCATION, plot);` *where plot is a single character A, B, C, or D*
  

## Machine Learning Functions

### About

The ESPCAM will be mounted to your OTV and act as your wifi module, with the added capabilities of camera vision. Note, this is not the overhead vision system above the arena - this is a development board provided that has a camera on it that will put on your otv.    

### <span>int Enes100.MLGetPrediction(int modelIndex)<a name="ml_pred"></a>

Sends current image from the ESPCAM to the Vision System to get processed by your team's machine learning model with the provided index. Models must have been uploaded to the [ENES100 Model Uploader](https://enes100.umd.edu/uploadmodel) with the index requested beforehand to use this function. The function uses your team name (from the Enes100.begin() statement) to find your model. As such, **make sure your team name matches the model name exactly**.

Example:
If your ML model contained the categories: **Thumbs Up**, **Thumbs Down**, **Thumb Sideways** in an array in that order, 
calling `Enes100.MLGetPrediction()` would return `0` if **Thumbs Up** is predicted, `1` if **Thumbs Down** is predicted, and `2` if **Thumb Sideways** is predicted.   

## Example Code and Debugging<a name="ex"></a>

Example code for each type of mission is included with the library. To view examples, open Arduino IDE and go to *
*File > Examples > ENES100**. (You must restart the IDE after installing the library for the examples to show.)

## Product Demonstration Procedures<a name="prod"></a>

During the product demonstration, messages sent using `print()` and `println()` will not be shown on the Vision System
console. The console will only print out the values that you send using `mission()`.

## For Simulator 

The function calls to get the Aurco values are a little bit different than what they will be when you code for your otv 
but they are functionally the same. 

```arduino
void setup() {
    // Your begin statement with arbitrary values
    Enes100.begin("It's lit", FIRE, 3, 8, 9);  
}
```
Once this is called you will use the function

`Enes100.updateLocation()`

Calling this will allow you to access the values of aurco marker by writing
the following

* `Enes100.location.x`
* `Enes100.location.y`
* `Enes100.location.theta`

```arduino
void loop() {
    // This code will continuously print out the x cordinate 
    Enes100.updateLocation();
    Enes100.println(Enes100.location.x);
		setBothMotors(25);
      
}
```
#### **Common Issues**:

If you get an error **AxiosError: Network Error** remove the 's' from the 
https link in url and reload the simulator 
