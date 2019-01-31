![My "shells"](https://i.imgur.com/4LNTElJ.jpg)

# Long Distance Arduino Lamp
This is my faithful NodeMCU ESP8266 recreation of the "Long Distance Friendship Lamp" product that extends on the project begun by Deepak Dhanavel Kumar Ravi Kumar. My version uses a physical button and implements easy wifi configuration via your smart phone with the ESP8266WiFi library, as well as other tweaks.

## For this project you will need:

1. A MQTT Server (like https://www.cloudmqtt.com/, which I used, and is free up to 5 nodes or "lamps")

2. NodeMCU units: https://amzn.to/2G1vxPB

3. 10k ohm Resistor

4. Hookup wire

5. MicroUSB cables that support data (for programming the NodeMCU) and charging (however many lamps you are going to set up): https://amzn.to/2ToN3R1

6. Some basic USB power bricks (however many lamps you are going to set up): https://amzn.to/2Sl5icA

7. An enclosure for the unit after you fully assemble it.  I 3D Printed a shell design which can be found on thingiverse (). I designed a custom base that fit the bottom of that design for the NodeMCU V3 unit and enclosed the entire thing nice and neatly.

![Design of custom NodeMCU V3 Base for the 3D Printed enclosure](https://i.imgur.com/BZfPDIP.png)

**NOTE** I do make these for people. If you would like me to make these for you (with your own custom design of lamp / enclosure for the unit and RGB light), please reach out to me at brian@wearerev.com.

Based off of the project here: https://www.uncommongoods.com/product/long-distance-friendship-lamp

Wifi is configured via the ESP8266WiFi library: https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
