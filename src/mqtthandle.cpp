#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "mqtthandle.h"
#include "firmware.h"

WiFiClient   espClient;
PubSubClient mqtt_client(espClient);
uint64_t     mqttLastCallMillis = 0;
uint64_t     mqttPingMillis     = 0;
char         myID[50];
char         topic[100];
char         msg[100];

void mqttConnect(void){
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (mqtt_client.connect(myID)) {
      Serial.println("connected");
      snprintf (topic, sizeof(topic), "%s/%s", myID, "startup");
      snprintf (msg, sizeof(msg), "%d", BUILD_VERSION);
      mqtt_client.publish(topic, msg);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqttSetup(void){
  snprintf (myID, sizeof(myID), "/blanket/%08X", ESP.getChipId());
  mqttLastCallMillis = millis();
  mqtt_client.setServer("172.26.2.68", 1883);
  mqttConnect();
  mqtt_client.loop();
}

uint64_t mqttLoop(void){

  if( (millis() - mqttLastCallMillis) > MQTT_SCHEDULE){

    mqttConnect();
    mqtt_client.loop();

    // Ping
    if( (millis() - mqttPingMillis) > 5000){
      snprintf (topic, sizeof(topic), "%s/%s", myID, "ping");
      snprintf (msg, sizeof(msg), "%d", millis());
      mqtt_client.publish(topic, msg);
      mqttPingMillis = millis();
    }


    mqttLastCallMillis = millis();
  }
  return (mqttLastCallMillis + MQTT_SCHEDULE);
}
