/** The MIT License (MIT)
 *
Copyright (c) 2019 by Brian Kinney - Revolution Development, LLC www.wearerev.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER{
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Check us out: www.wearerev.com
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
Ticker ticker;
WiFiClient espClient;
PubSubClient client(espClient);

// MQTT Server setup
const char* mqtt_server = "xxx"; // Enter your MQTT Server
const int port = 12345; // Enter your MQTT Server port here

// RGB pin setup
const int red = 5; // GPIO pin for the RED LED
const int green = 4; // GPIO pin for the GREEN LED
const int blue = 12; // GPIO pin for the BLUE LED

// Main LED for status, touch input pin and lamp definitons
const int led = 4; // The GPIO pin for the status LED (i.e. when connecting to wifi this blinks). This can be set to one of the RGB pins defined above, or an onboard LED GPIO number.
const int touch = 14; // The GPIO pin for the touch switch.
const char* lampColor = "010"; // R G B "010" means RED and BLUE are ON, Green is OFF
const char* lampName = "LAMPNAMEHERE"; // MQTT Server user (a.k.a lamp name) here
const char* lampPassword = "LAMPPASSWORDHERE" // MQTT Server user password (a.k.a lamp password) here. Leave as empty string if there isn't any password.

int currentColor[3] = {0, 1, 0}; // Set current lamp color R G B - RED and BLUE are ON
int newColor[3] = {0, 1, 0}; // Set new lamp color - RED and BLUE are ON

long sampleTime = 0; // the last time the output pin was sampled
int debounce_count = 100; // number of millis/samples to consider before declaring a debounced input
int counter = 0;
int reading; // the current value read from the input pin
int current_state = LOW; // the debounced input value

unsigned long turnOffMillis = 0;
int flag = 0;
int duty = 0;

// LED Fade settings
const int steps = 64;
const int fadeSpeed = 50;
const int lookup[64] = {
  0, 0, 0, 1, 1, 1, 1, 2, 2, 3, 3,
  4, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
  14, 15, 16, 18, 19, 21, 22, 24, 25, 27, 28,
  30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50,
  52, 55, 57, 60, 62, 65, 67, 70, 72, 75, 78,
  81, 84, 87, 90, 93, 96, 99, 99, 99
};

void setColor(int[3]);

void tick() {
  //toggle state
  int state = digitalRead(led);  // get the current state of GPIO1 pin
  digitalWrite(led, !state);     // set pin to the opposite state
}

// Callback status output to Serial frm MQTT server pub/sub messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] -");
  Serial.print(length);
  Serial.print("- ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    newColor[i] = ((int)payload[i] - 48);
  }

  Serial.println();
  Serial.println(newColor[1]);
  setColor(newColor);
  turnOffMillis = millis();
  flag = 1;
}

// MQTT Server reconnect when not connected
void reconnect() {
  while(!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
    if(client.connect(lampName, lampName, lampPassword)) {
      Serial.println("Connected to the MQTT Server");
      client.publish("online", lampName);
      client.subscribe("colorChange");
      digitalWrite(led, LOW);
    } else {
      digitalWrite(led, HIGH);
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("Trying again in 5 seconds...");

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entering Wifi configuration mode...");
  Serial.println(WiFi.softAPIP());
  ticker.detach();
  ticker.attach(0.3, tick);
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setup() {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(touch, INPUT);
  digitalWrite(led, 1);

  // start ticker with 0.5 because we start in AP mode and try to connect
  ticker.attach(0.7, tick);

  Serial.begin(9600);

  //WiFiManager
  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setConnectTimeout(20);

  //Uncomment to reset any existing saved wifi settings
  //wifiManager.resetSettings();

  wifiManager.autoConnect("Long Distance Lamp");

  //if you get here you have connected to the WiFi
  Serial.println("Connected to wifi.");

  ticker.detach();
  client.setServer(mqtt_server, port);
  client.setCallback(callback);
  digitalWrite(led, LOW);

  setColor(currentColor);
  flag = 1;
  turnOffMillis = millis();
}

void loop(){
  if(flag == 1 && (millis() - turnOffMillis) > 3600000) {
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    digitalWrite(blue, LOW);
    flag = 0;
  }

  if(millis() != sampleTime) {
    reading = digitalRead(touch);

    if (reading == current_state && counter > 0) {
      counter--;
    }

    if(reading != current_state) {
      counter++;
    }

    // If the Input has shown the same value for long enough let's switch it
    if(counter >= debounce_count) {
      counter = 0;
      current_state = reading;

      Serial.println(current_state);

      if (current_state == 1) {
        client.publish("colorChange", lampColor);
      }
    }

    sampleTime = millis();
  }

  if(!client.connected()) {
    reconnect();
  }

  client.loop();
}

void setColor(int newColor[3]) {
  for(int i = (steps - 1); i >= 0; i--) {
    duty = lookup[i];

    for(int j = 0; j < fadeSpeed; j++) {
      // one pulse of PWM
      digitalWrite(red, (int)currentColor[0]);
      digitalWrite(green, (int)currentColor[1]);
      digitalWrite(blue, (int)currentColor[2]);
      delayMicroseconds(duty);
      digitalWrite(red, LOW);
      digitalWrite(green, LOW);
      digitalWrite(blue, LOW);
      delayMicroseconds(100 - duty);
    }
  }

  for(int i = 1; i < steps; i++) {
    duty = lookup[i];

    for(int j = 0; j < fadeSpeed; j++) {
      digitalWrite(red, (int)newColor[0]);
      digitalWrite(green, (int)newColor[1]);
      digitalWrite(blue, (int)newColor[2]);
      delayMicroseconds(duty);
      digitalWrite(red, LOW);
      digitalWrite(green, LOW);
      digitalWrite(blue, LOW);
      delayMicroseconds(100 - duty);
    }
  }

  digitalWrite(red, (int)newColor[0]);
  digitalWrite(green, (int)newColor[1]);
  digitalWrite(blue, (int)newColor[2]);
  currentColor[0] = newColor[0];
  currentColor[1] = newColor[1];
  currentColor[2] = newColor[2];
}
