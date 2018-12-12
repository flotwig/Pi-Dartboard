Pi Dartboard
======

![Our Dartboard](/doc-images/dartboard.jpg)

Team members: Kaitlin Burke, Kevin Hillinski, Sangwon Han, and Zach Bloomquist

### Table of Contents
1. [Project Idea](#Project-Idea)
2. [Instructions](#Instructions)
3. [Hardware Setup](#Hardware-Setup)

## Project Idea

This project is an electronic dartboard that uses the plastic casing of an existing electronic dartboard and adds a Raspberry Pi and a touchscreen.

The touchscreen will be mounted in the casing and the Pi and wiring will be hidden inside in the case as well.

To register when darts hit the dartboard, we will use GPIO pins as digital input and digital outputs, along with an algorithm, to detect when a dart has hit.

This setup will allow us to program any popular dart game as well as any custom game that be thought of.

We will also make a GUI for gameplay with buttons such as "Missed Dart" and "Next Player", as well as a home screen that allows you to pick the game mode and number of players.

## Instructions

1. Connect the Raspberry Pi and dartboard together as described in [Hardware Setup](#Hardware-Setup).
2. Boot up your Raspberry Pi and open a terminal
2. Clone [this Git repository](https://github.com/flotwig/Pi-Dartboard) to your Raspberry Pi to get set up with the code for this project:  
`git clone https://github.com/flotwig/Pi-Dartboard.git`
3. Run `./gui` to start the dartboard user interface.


If you modify the code, you can use `./build.sh` to rebuild the `gui` executable.

## Hardware Setup

* [Raspberry Pi 3 Model B](https://www.raspberrypi.org/products/raspberry-pi-3-model-b/)
* [Adafruit HDMI 7" 800x480 Display](https://www.adafruit.com/product/2407)
* [Arachnid Cricket Pro 670 Tournament-Quality Electronic Dartboard](https://www.walmart.com/ip/Arachnid-Cricket-Pro-670-Tournament-Quality-Electronic-Dartboard-with-15-5-Target-Area-and-Micro-Segment-Dividers-for-Higher-Scoring/26509524)
* [Sparkfun Mono Audio Amp](https://www.sparkfun.com/products/11044)
* [Sparkfun TRRS 3.5mm Jack Breakout](https://www.sparkfun.com/products/11570)
* RGB LED
* 3 x 330 ohm resistors (for LED)
* Short 3.5mm audio cable to connect Pi to amplifier

### Display & LED Hookup

Connect the Pi to the HDMI display's HDMI and USB (for touch input).

Connect the LED signal pins to GPIO 2, 3, and 4, and connect the last pin to ground.

### Dartboard Hookup

Here's what the dartboard looks like from the back with the circuit removed and with the circuit board added:

|Without Circuit Board|With Circuit Board|
|---------------------|------------------|
|![Dartboard back](/doc-images/dartboard-back.jpg)|![Dartboard circuit](/doc-images/dartboard-circuit.jpg)|

When a dart hits the dartboard from the front, the little plastic piece it hits will slide backwards:  
![Plastic bit sliding](/doc-images/sliding.gif)

And it will strike the dartboard's circuit board:  
![Plastic bit striking](/doc-images/bumping.gif)

The circuit board itself consists of three layers. The top and bottom layers are flexible layers with large solder pads at the intersection of traces. The middle layer is a solid translucent plastic sheet with holes where the top and bottom layers have intersections. Thus, when a dartboard strikes the dartboard, the traces of the top and bottom layers will connect through the middle layer for a moment at the location where the dart struck.

By carefully decoding what traces of the cables coming from the circuit board cover what parts of the dartboard, we can connect the dartboard to our Pi and use code to poll for hits.

Our dartboard uses a thin ribbon cable for connection. To adapt this to our breadboard, we opted to cut up the ribbon cable between the traces and attach alligator clips to each contact, since soldering the ribbon cable would destroy it:  
![Ribbon cable splice](/doc-images/ribbon-cable-splice.jpg)

In our setup, we used the following table to align the traces on the circuit board to our circuit layout:  
![Circuit layout](/doc-images/circuit-layout.png)

Then we can calculate the point values for each possible combination of pins:  
![Point values](/doc-images/point-values.png)

We connected the pins from the dartboard circuit to the corresponding GPIO pins on our Raspberry Pi:

|Pinout Table|Raspberry Pi 3 Pinout|
|------------|---------------------|
|![Pi Pinout](/doc-images/pi-pinout.png)|![Raspberry Pi 3 Pinout](/doc-images/rpi-3-pinout.jpg)|

### Speaker Hookup

We'll re-use the existing speaker that's in the dartboard. You'll need to solder on new wires to the speaker's + and - hookups. Then, connect the audio amplifier and TRRS breakout board as follows:

|Amplifier Pin|Connection|
|----|----|
|OUT +|Speaker +|
|OUT -|Speaker -|
|IN +|TRRS RING1 and TRRS TIP|
|IN -|Pi Ground|
|PWR -|Pi Ground|
|PWR +|Pi 5V|

|TRRS Breakout Pin|Connection|
|----|----|
|SLEEVE|Ground|
|RING2|Not Connected|
|RING1|Amp IN +|
|TIP|Amp IN +|

We connect the TRRS RING1 and TIP because together, they contain the left and right channels.

Now, connect the Pi's 3.5mm output to the TRRS breakout board's 3.5mm jack.

As a last step, ensure that the Pi's audio is set up to be routed over 3.5mm, not over HDMI. This varies by operating system and previous config so we will not cover it here.