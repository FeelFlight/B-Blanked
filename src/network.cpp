#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "network.h"

uint64_t networkLastCallMillis = 0;

void networkconnectToWifi(void){

  Serial.print("WLAN:");
  Serial.println(WLANSSID);
  Serial.print("PASS:");
  Serial.println(WLANPASSWD);
  Serial.print("Connecting WLAN:");
  WiFi.begin(WLANSSID, WLANPASSWD);

  uint8_t counter = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    if(counter++ > 100){
        ESP.reset();
    }

  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void networkSetup(void){
  networkconnectToWifi();
  networkLastCallMillis = millis();
}

uint64_t networkLoop(void){
  if( (millis() - networkLastCallMillis) > NETWORK_SCHEDULE){
    networkLastCallMillis = millis();
  }
  return (networkLastCallMillis + NETWORK_SCHEDULE);
}

