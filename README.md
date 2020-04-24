![My "shells"](https://i.imgur.com/4LNTElJ.jpg)

# Long Distance Arduino Lamp
This is my faithful NodeMCU ESP8266 recreation of the "Long Distance Friendship Lamp" product that extends on the project begun by Deepak Dhanavel Kumar Ravi Kumar. My version uses a physical button and implements easy wifi configuration via your smart phone with the ESP8266WiFi library, as well as other tweaks.

## What these do
Basically you have up to 5 units you hand out to your friends / family members. They configure it using the SSID that is initially broadcasted from the unit to use your home WiFi connection. From there it reaches a small MQTT messaging server to send a response and listen for events. When you hit your button, depending on what color you have configured for that unit, it will change all of the other units that color. Likewise, if they hit their button, it will change yours their color. The idea around this is for presence and feeling close to someone albeit they are far away. When you walk past your lamp, hitting your button signifies you're thinking of your family, and if they see their light change to your color, they will know they are by their lamp. "Thinking of us" or "Thinking of you" and "Miss you" are things my family have said while tapping their buttons. This act usually results in someone calling the other person to talk to them :)

## For this project you will need:

1. A MQTT Server (like https://www.cloudmqtt.com/, which I used, and is free up to 5 nodes or "lamps")

2. NodeMCU units: https://amzn.to/2G1vxPB

3. 10k ohm Resistor

4. Hookup wire

5. MicroUSB cables that support data (for programming the NodeMCU) and charging (however many lamps you are going to set up): https://amzn.to/2ToN3R1

6. Some basic USB power bricks (however many lamps you are going to set up): https://amzn.to/2Sl5icA

7. An enclosure for the unit after you fully assemble it.  I 3D Printed a shell design which can be found on thingiverse: https://www.thingiverse.com/thing:29876. I designed a custom base that fit the bottom of this design for the NodeMCU V3 unit and enclosed the entire thing nice and neatly.

![Design of custom NodeMCU V3 Base for the 3D Printed enclosure](https://i.imgur.com/BZfPDIP.png)

**NOTE** I do make and sell these. If you would like me to make these for you (with your own custom design of lamp / enclosure for the unit and RGB light), please reach out to me at webdevbrian@gmail.com.

Based off of the project here: https://www.uncommongoods.com/product/long-distance-friendship-lamp

Wifi is configured via the ESP8266WiFi library: https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi

## Notes:

- There is a total maximum of 5 combinations of lamp colors due to this being RGB LED based. I hopefully soon will implement a version of this to support Adafruit NEO Pixels and support things like color patterns for each lamp

- Each lamp will need to be configured individually as MQTT Users setup in your MQTT settings.  The MQTT Patterns were set up as read/write access to two Patterns named "online" and "colorChange", which you will need to setup on your MQTT server.

- Future enhacements include Piezo buzzer tone support, Adafruit Neo Pixel support and implementation of a 1'' OLED ICU display to show custom graphics and other things like the weather.
