# <span >Initializing the Wifi Module and Updating the Location<a name="wifi_mod"></a>

See the example "SimpleTest.ino" by opening the Arduino editor with the library installed. Then click File->Examples->
Enes100->SimpleTest

# <span >Setting up an Ultrasonic Sensor<a name="ultrasonic"></a>

<pre>
<font color="#434f54">&#47;&#47; defines pins numbers</font>
<font color="#00979c">const</font> <font color="#00979c">int</font> <font color="#000000">trigPin</font> <font color="#434f54">=</font> <font color="#000000">9</font><font color="#000000">;</font>
<font color="#00979c">const</font> <font color="#00979c">int</font> <font color="#000000">echoPin</font> <font color="#434f54">=</font> <font color="#000000">10</font><font color="#000000">;</font>
<font color="#434f54">&#47;&#47; defines variables</font>
<font color="#00979c">long</font> <font color="#000000">duration</font><font color="#000000">;</font>
<font color="#00979c">int</font> <font color="#000000">distance</font><font color="#000000">;</font>
<font color="#00979c">void</font> <font color="#5e6d03">setup</font><font color="#000000">(</font><font color="#000000">)</font> <font color="#000000">{</font>
 &nbsp;<font color="#d35400">pinMode</font><font color="#000000">(</font><font color="#000000">trigPin</font><font color="#434f54">,</font> <font color="#00979c">OUTPUT</font><font color="#000000">)</font><font color="#000000">;</font> <font color="#434f54">&#47;&#47; Sets the trigPin as an Output</font>
 &nbsp;<font color="#d35400">pinMode</font><font color="#000000">(</font><font color="#000000">echoPin</font><font color="#434f54">,</font> <font color="#00979c">INPUT</font><font color="#000000">)</font><font color="#000000">;</font> <font color="#434f54">&#47;&#47; Sets the echoPin as an Input</font>
 &nbsp;<b><font color="#d35400">Serial</font></b><font color="#434f54">.</font><font color="#d35400">begin</font><font color="#000000">(</font><font color="#000000">9600</font><font color="#000000">)</font><font color="#000000">;</font> <font color="#434f54">&#47;&#47; Starts the serial communication</font>
<font color="#000000">}</font>
<font color="#00979c">void</font> <font color="#5e6d03">loop</font><font color="#000000">(</font><font color="#000000">)</font> <font color="#000000">{</font>
 &nbsp;<font color="#434f54">&#47;&#47; Clears the trigPin</font>
 &nbsp;<font color="#d35400">digitalWrite</font><font color="#000000">(</font><font color="#000000">trigPin</font><font color="#434f54">,</font> <font color="#00979c">LOW</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;<font color="#d35400">delayMicroseconds</font><font color="#000000">(</font><font color="#000000">2</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;<font color="#434f54">&#47;&#47; Sets the trigPin on HIGH state for 10 micro seconds</font>
 &nbsp;<font color="#d35400">digitalWrite</font><font color="#000000">(</font><font color="#000000">trigPin</font><font color="#434f54">,</font> <font color="#00979c">HIGH</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;<font color="#d35400">delayMicroseconds</font><font color="#000000">(</font><font color="#000000">10</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;<font color="#d35400">digitalWrite</font><font color="#000000">(</font><font color="#000000">trigPin</font><font color="#434f54">,</font> <font color="#00979c">LOW</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;<font color="#434f54">&#47;&#47; Reads the echoPin, returns the sound wave travel time in microseconds</font>
 &nbsp;<font color="#000000">duration</font> <font color="#434f54">=</font> <font color="#d35400">pulseIn</font><font color="#000000">(</font><font color="#000000">echoPin</font><font color="#434f54">,</font> <font color="#00979c">HIGH</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;<font color="#434f54">&#47;&#47; Calculating the distance</font>
 &nbsp;<font color="#000000">distance</font> <font color="#434f54">=</font> <font color="#000000">duration</font> <font color="#434f54">*</font> <font color="#000000">0.034</font> <font color="#434f54">&#47;</font> <font color="#000000">2</font><font color="#000000">;</font>
 &nbsp;<font color="#434f54">&#47;&#47; Prints the distance on the Serial Monitor</font>
 &nbsp;<b><font color="#d35400">Serial</font></b><font color="#434f54">.</font><font color="#d35400">print</font><font color="#000000">(</font><font color="#005c5f">&#34;Distance: &#34;</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;<b><font color="#d35400">Serial</font></b><font color="#434f54">.</font><font color="#d35400">println</font><font color="#000000">(</font><font color="#000000">distance</font><font color="#000000">)</font><font color="#000000">;</font>
<font color="#000000">}</font>

</pre>

## Note: The below code is pseudocode. Its purpose is to help you understand how to do certain actions. There are multiple ways to do this.

# <span >Drive forward until you reach a certain x and y coordinate.<a name="drivef"></a>
This code uses the old style of updateLocation and Enes100.location.x
<pre>
<font color="#434f54">&#47;&#47;Setting a target of x = 2.5, y = 1.5</font>
<font color="#5e6d03">while</font> <font color="#000000">(</font><font color="#434f54">!</font><b><font color="#d35400">Enes100</font></b><font color="#434f54">.</font><font color="#d35400">updateLocation</font><font color="#000000">(</font><font color="#000000">)</font><font color="#000000">)</font><font color="#000000">;</font> <font color="#434f54">&#47;&#47;will keep running until succesfully update location</font>
<font color="#5e6d03">while</font><font color="#000000">(</font><b><font color="#d35400">Enes100</font></b><font color="#434f54">.</font><font color="#000000">location</font><font color="#434f54">.</font><font color="#000000">x</font> <font color="#434f54">&lt;</font> <font color="#000000">2.5</font><font color="#000000">)</font> <font color="#000000">{</font> &nbsp;<font color="#434f54">&#47;&#47;move to x = 2.5</font>
 &nbsp;&nbsp;&nbsp;<b><font color="#d35400">Enes100</font></b><font color="#434f54">.</font><font color="#d35400">updateLocation</font><font color="#000000">(</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;&nbsp;&nbsp;<font color="#000000">moveForward</font><font color="#000000">(</font><font color="#000000">)</font><font color="#000000">;</font>
<font color="#000000">}</font>
<font color="#d35400">stop</font><font color="#000000">(</font><font color="#000000">)</font><font color="#000000">;</font>
<font color="#5e6d03">if</font><font color="#000000">(</font><b><font color="#d35400">Enes100</font></b><font color="#434f54">.</font><font color="#000000">locaiton</font><font color="#434f54">.</font><font color="#000000">y</font> <font color="#434f54">&gt;</font> <font color="#000000">1.5</font><font color="#000000">)</font> <font color="#000000">{</font> <font color="#434f54">&#47;&#47;if we are above the target, move down, otherwise move up.</font>
 &nbsp;&nbsp;<b><font color="#d35400">Enes100</font></b><font color="#434f54">.</font><font color="#d35400">updateLocation</font><font color="#000000">(</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;&nbsp;<font color="#000000">moveDown</font><font color="#000000">(</font><font color="#000000">)</font><font color="#000000">;</font>
<font color="#000000">}</font> <font color="#5e6d03">else</font> <font color="#000000">{</font>
 &nbsp;&nbsp;&nbsp;<b><font color="#d35400">Enes100</font></b><font color="#434f54">.</font><font color="#000000">updateLocaiton</font><font color="#000000">(</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;&nbsp;&nbsp;<font color="#000000">moveUp</font><font color="#000000">(</font><font color="#000000">)</font><font color="#000000">;</font>
<font color="#000000">}</font>

</pre>

# <span >Rotate to a certain theta coordinate<a name="rotate"></a>

<pre>
<font color="#434f54">&#47;&#47; This function will make the OTV turn to a certain location.</font>
<font color="#00979c">void</font> <font color="#000000">setAngle</font><font color="#000000">(</font><font color="#000000">target</font><font color="#000000">)</font> <font color="#000000">{</font>
 &nbsp;&nbsp;&nbsp;<font color="#d35400">print</font><font color="#000000">(</font><font color="#005c5f">&#34;Targeting angle: &#34;</font><font color="#000000">)</font><font color="#000000">;</font> <font color="#d35400">println</font><font color="#000000">(</font><font color="#000000">target</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;&nbsp;&nbsp;<font color="#434f54">&#47;&#47; The following line runs our targeting code WHILE the DIFFERENCE (subtraction is taking the difference) is between -thresh and thresh. </font>
 &nbsp;&nbsp;&nbsp;<font color="#434f54">&#47;&#47; We take the absolute value of the difference in order to compare it to a single threshold.</font>
 &nbsp;&nbsp;&nbsp;<font color="#5e6d03">while</font> <font color="#000000">(</font><font color="#d35400">abs</font><font color="#000000">(</font><font color="#000000">target</font> <font color="#434f54">-</font> <font color="#000000">currentTheta</font><font color="#000000">)</font> <font color="#434f54">&gt;</font> <font color="#000000">threshold</font><font color="#000000">)</font> <font color="#000000">{</font>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color="#000000">turnValue</font> <font color="#434f54">=</font> <font color="#000000">someConstant</font> <font color="#434f54">*</font> <font color="#000000">(</font><font color="#000000">target</font> <font color="#434f54">-</font> <font color="#000000">currentTheta</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color="#000000">turnValue</font> <font color="#434f54">=</font> <font color="#d35400">constrain</font><font color="#000000">(</font><font color="#000000">turnValue</font><font color="#434f54">,</font> <font color="#434f54">-</font><font color="#000000">MAX_TURN_SPEED</font><font color="#434f54">,</font> <font color="#000000">MAX_TURN_SPEED</font><font color="#000000">)</font><font color="#000000">;</font> <font color="#434f54">&#47;&#47;Constrain your turn speed.</font>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color="#000000">setMotors</font><font color="#000000">(</font><font color="#000000">turnValue</font><font color="#434f54">,</font> <font color="#434f54">-</font><font color="#000000">turnValue</font><font color="#000000">)</font><font color="#000000">;</font> <font color="#434f54">&#47;&#47; You will need to implement this yourself. </font>
 &nbsp;&nbsp;&nbsp;<font color="#000000">}</font>
 &nbsp;&nbsp;&nbsp;<font color="#d35400">print</font><font color="#000000">(</font><font color="#005c5f">&#34;Got to angle!&#34;</font><font color="#000000">)</font><font color="#000000">;</font>
 &nbsp;&nbsp;&nbsp;<font color="#000000">setMotors</font><font color="#000000">(</font><font color="#000000">0</font><font color="#434f54">,</font> <font color="#000000">0</font><font color="#000000">)</font><font color="#000000">;</font>
<font color="#000000">}</font>

</pre>
