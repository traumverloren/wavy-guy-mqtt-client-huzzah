#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include "arduino_secrets.h" 

// constants won't change
const int RELAY_PIN = 13;  // the Arduino pin, which connects to the SIGNAL pin of relay

const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;

WiFiClient net;
MQTTClient client;

void connect();  // <- predefine connect() for setup()

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // You need to set the IP address directly.
  client.begin(IP_ADDRESS, PORT, net);
  client.onMessage(messageReceived);

  connect();

  // initialize digital pin as an output.
  pinMode(RELAY_PIN, OUTPUT);
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("wavy-guy-huzzah", "", "")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
  client.subscribe("wavy-gravy");
}

void messageReceived(String &topic, String &payload) {
  const char* delimiter = ",";
  String incomingMode = payload.substring(0,payload.indexOf(delimiter));
  Serial.println("topic: " + topic);
  Serial.println("payload: " + incomingMode);
}

void loop() {
  client.loop();
  //  delay(10);  // <- fixes some issues with WiFi stability

  // Test out the relay
  digitalWrite(RELAY_PIN, HIGH);
  delay(2000);
  digitalWrite(RELAY_PIN, LOW);
  delay(2000);

  if (!client.connected()) {
    connect();
  }
}
