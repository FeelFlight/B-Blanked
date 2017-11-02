#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "network.h"

uint64_t networkLastCallMillis = 0;

void networkSetup(void){
  networkLastCallMillis = millis();
}

uint64_t networkLoop(void){
  if( (millis() - networkLastCallMillis) > NETWORK_SCHEDULE){
    networkLastCallMillis = millis();
  }
  return (networkLastCallMillis + NETWORK_SCHEDULE);
}

void connectToWifi(void){

  WiFi.begin(WLANSSID, WLANPASSWD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}
