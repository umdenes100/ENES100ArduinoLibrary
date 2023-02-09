# Initializing the Wifi Module and Updating the Location
<pre>
<font color="#5e6d03">#include</font> <font color="#005c5f">&#34;Enes100.h&#34;</font>
<font color="#00979c">void</font> <font color="#5e6d03">setup</font><font color="#000000">(</font><font color="#000000">)</font> <font color="#000000">{</font>
 &nbsp;&nbsp;&nbsp;<font color="#d35400">delay</font><font color="#000000">(</font><font color="#000000">2000</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;&nbsp;&nbsp;<b><font color="#d35400">Enes100</font></b><font color="#434f54">.</font><font color="#d35400">begin</font><font color="#000000">(</font><font color="#005c5f">&#34;It&#39;s lit&#34;</font><font color="#434f54">,</font> <font color="#000000">FIRE</font><font color="#434f54">,</font> <font color="#000000">3</font><font color="#434f54">,</font> <font color="#000000">8</font><font color="#434f54">,</font> <font color="#000000">9</font><font color="#000000">)</font><font color="#000000">;</font> &nbsp;<font color="#434f54">&#47;&#47; Aruco ID of 3. Rx Pin of 8. Tx Pin of 9.</font>
 &nbsp;&nbsp;&nbsp;<font color="#d35400">delay</font><font color="#000000">(</font><font color="#000000">2000</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;&nbsp;&nbsp;<b><font color="#d35400">Enes100</font></b><font color="#434f54">.</font><font color="#d35400">println</font><font color="#000000">(</font><font color="#005c5f">&#34;Connected!&#34;</font><font color="#000000">)</font><font color="#000000">;</font>
<font color="#000000">}</font>
<font color="#00979c">void</font> <font color="#5e6d03">loop</font><font color="#000000">(</font><font color="#000000">)</font><font color="#000000">{</font>
 &nbsp;&nbsp;&nbsp;<font color="#5e6d03">while</font> <font color="#000000">(</font><font color="#434f54">!</font><b><font color="#d35400">Enes100</font></b><font color="#434f54">.</font><font color="#d35400">updateLocation</font><font color="#000000">(</font><font color="#000000">)</font><font color="#000000">)</font><font color="#000000">;</font> <font color="#434f54">&#47;&#47;will keep running until succesfully update location</font>
 &nbsp;&nbsp;&nbsp;<b><font color="#d35400">Enes100</font></b><font color="#434f54">.</font><font color="#d35400">print</font><font color="#000000">(</font><font color="#005c5f">&#34;X coord:&#34;</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;&nbsp;&nbsp;<b><font color="#d35400">Enes100</font></b><font color="#434f54">.</font><font color="#d35400">println</font><font color="#000000">(</font><b><font color="#d35400">Enes100</font></b><font color="#434f54">.</font><font color="#000000">location</font><font color="#434f54">.</font><font color="#000000">x</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;&nbsp;&nbsp;<b><font color="#d35400">Enes100</font></b><font color="#434f54">.</font><font color="#d35400">print</font><font color="#000000">(</font><font color="#005c5f">&#34;Y coord:&#34;</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;&nbsp;&nbsp;<b><font color="#d35400">Enes100</font></b><font color="#434f54">.</font><font color="#d35400">println</font><font color="#000000">(</font><b><font color="#d35400">Enes100</font></b><font color="#434f54">.</font><font color="#000000">location</font><font color="#434f54">.</font><font color="#000000">y</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;&nbsp;&nbsp;<b><font color="#d35400">Enes100</font></b><font color="#434f54">.</font><font color="#d35400">print</font><font color="#000000">(</font><font color="#005c5f">&#34;Theta coord:&#34;</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;&nbsp;&nbsp;<b><font color="#d35400">Enes100</font></b><font color="#434f54">.</font><font color="#d35400">println</font><font color="#000000">(</font><b><font color="#d35400">Enes100</font></b><font color="#434f54">.</font><font color="#000000">location</font><font color="#434f54">.</font><font color="#000000">theta</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;&nbsp;&nbsp;<font color="#d35400">delay</font><font color="#000000">(</font><font color="#000000">1000</font><font color="#000000">)</font><font color="#000000">;</font> <font color="#434f54">&#47;&#47;wait 1 second in order to not overload the vision system</font>
<font color="#000000">}</font>

</pre>


# Setting up an Ultrasonic Sensor

```cpp
// defines pins numbers
const int trigPin = 9;
const int echoPin = 10;
// defines variables
long duration;
int distance;
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}
void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
}
```

## Note: The below code is pseudocode. Its purpose is to help you understand how to do certain actions. There are multiple ways to do this.
# Drive forward until you reach a certain x and y coordinate.
```cpp
//Setting a target of x = 2.5, y = 1.5
while (!Enes100.updateLocation()); //will keep running until succesfully update location
while(Enes100.location.x < 2.5) {  //move to x = 2.5
    Enes100.updateLocation();
    moveForward();
}
stop();
if(Enes100.locaiton.y > 1.5) { //if we are above the target, move down, otherwise move up.
   Enes100.updateLocation();
   moveDown();
} else {
    Enes100.updateLocaiton();
    moveUp();
}

```
# Rotate to a certain theta coordinate
This is when things start to get fun.
```cpp
// This function will make the OTV turn to a certain location.
void setAngle(target) {
    print("Targeting angle: "); println(target);
    updateLocation();
    // The following line runs our targeting code WHILE the DIFFERENCE (subtraction is taking he difference) is between -thresh and thresh. 
    // We take the absolute value of the difference in order to compare it to a single threshold.
    while (abs(target - currentTheta) > threshold) {
        turnValue = someConstant * (target - currentTheta);
        turnValue = constrain(turnValue, -MAX_TURN_SPEED, MAX_TURN_SPEED); //Constrain your turn speed.
        setMotors(turnValue, -turnValue); // You will need to implement this yourself. 
    }
    print("Got to angle!");
    setMotors(0, 0);
}
```

