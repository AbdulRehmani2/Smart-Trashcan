#include <WiFi.h>
#include <PubSubClient.h>
#include "ThingSpeak.h"
#include <HardwareSerial.h>

#define SSID "YOUR WIFI NAME"
#define PASSWORD "YOUR WIFI PASSWORD"

#define channelID "CHANNEL ID ON THINGSPEAK"
#define APIKey "YOUR API KEY"

const char* mqttServer = "broker.mqtt.cool";
const char *ntpServerName = "pool.ntp.org";
const int mqttPort = 1883;

HardwareSerial SerialPort(2);

WiFiClient espClient;
PubSubClient client(espClient);

WiFiClient thingClient;

void setup() {
  Serial.begin(115200);
  SerialPort.begin(9600, SERIAL_8N1, 17, 16);
  connectToWiFi();
  connectToMQTT();
  ThingSpeak.begin(thingClient);
}

void connectToWiFi() {
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void connectToMQTT() {
  client.setServer(mqttServer, mqttPort);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("2022-CS-57/esp-controller")) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("Failed");
      delay(2000);
    }
  }
  client.subscribe("2022-CS-57/controller");
  client.setCallback(callback);
}

void loop(){
  client.loop();
  if (SerialPort.available() > 0) {
    String receivedValue = SerialPort.readStringUntil('\n');
    receivedValue.trim();
    char receivedChar = receivedValue.charAt(0);
    if(receivedChar == '0'){
      client.publish("2022-CS-57/Log", "Closed");
      ThingSpeak.writeField(channelID, 1, 0, APIKey);
    }
    else if (receivedChar == '1'){
      client.publish("2022-CS-57/Log", "Opened");
      ThingSpeak.writeField(channelID, 1, 1, APIKey);
    }
  }
  delay(1000);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String topicStr = topic;
  String payloadStr(reinterpret_cast<char*>(payload), length);
  Serial.println(topicStr);
  Serial.println(topicStr == "2022-CS-57/controller");
  Serial.println(payloadStr.charAt(0));
  payloadStr.trim();
  SerialPort.print(payloadStr.charAt(0));
}
