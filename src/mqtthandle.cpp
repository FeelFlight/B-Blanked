#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "mqtthandle.h"
#include "firmware.h"
#include "battery.h"
#include "light.h"
#include "vibration.h"

WiFiClient   espClient;
PubSubClient mqtt_client(espClient);
uint64_t     mqttLastCallMillis = 0;
uint64_t     mqttPingMillis     = 0;
uint64_t     mqttBatteryMillis  = 0;
char         myID[50];
char         topic[100];
char         msg[100];

void callback(char* topicchars, byte* payloadbytes, unsigned int length) {
  String topic = String(topicchars);
  char payload[100];
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (uint8_t i = 0; i < length; i++) {
    payload[i] = payloadbytes[i];
  }

  payload[length] = '\0';
  Serial.println(payload);

  if (topic.indexOf("firmwareupdate") > 0){
    Serial.println("Firmware update now");
    firmwarecheckForNew();
  }

  if (topic.indexOf("vibrate") > 0){
    Serial.println("vibrate");
    uint16_t dval = atoi(payload);
    vibrationForMilliseconds(dval);
  }

  if (topic.indexOf("flash") > 0){
    Serial.println("flash");
    uint8_t li = topic.lastIndexOf('/');
    String  si = topic.substring(li+1);
    uint8_t i  = si.toInt();
    uint16_t dval = atoi(payload);
    flashLED(i, dval);
  }

  if (topic.indexOf("pulse") > 0){
    Serial.println("pulse");
    uint8_t li = topic.lastIndexOf('/');
    String  si = topic.substring(li+1);
    uint8_t i  = si.toInt();

    if (String(payload) == "ON"){
        pulseLED(i, true);
    }else{
        pulseLED(i, false);
    }

  }

  if (topic.indexOf("colour") > 0){
    Serial.println("colour");
    uint8_t li = topic.lastIndexOf('/');
    String  si = topic.substring(li+1);
    uint8_t i  = si.toInt();
    char subbuff[5];
    memcpy(subbuff, &payload[0], 3 );
    subbuff[3] = '\0';
    uint8_t r  = atoi(subbuff);
    memcpy(subbuff, &payload[4], 3 );
    subbuff[3] = '\0';
    uint8_t g  = atoi(subbuff);
    memcpy(subbuff, &payload[8], 3 );
    subbuff[3] = '\0';
    uint8_t b  = atoi(subbuff);
    light(i,r,g,b);
  }

}

void mqttConnect(void){
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (mqtt_client.connect(myID)) {
      Serial.println("connected");
      snprintf (topic, sizeof(topic), "%s/%s", myID, "startup");
      snprintf (msg, sizeof(msg), "%d", BUILD_VERSION);
      mqtt_client.publish(topic, msg);
      snprintf (topic, sizeof(topic), "%s/#", myID);
      mqtt_client.subscribe(topic);
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
  mqtt_client.setCallback(callback);
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

    // Ping
    if( (millis() - mqttBatteryMillis) > 30000){
      snprintf (topic, sizeof(topic), "%s/%s", myID, "battery");
      snprintf (msg, sizeof(msg), "%d", batteryGetLevel());
      mqtt_client.publish(topic, msg);
      mqttBatteryMillis = millis();
    }

    mqttLastCallMillis = millis();
  }
  return (mqttLastCallMillis + MQTT_SCHEDULE);
}
