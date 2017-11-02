#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include "pin.h"
#include "light.h"
#include "main.h"
#include "vibration.h"

WiFiClient   espClient;
PubSubClient mqtt_client(espClient);

void checkForNewFirmware(void){

    HTTPClient http;
    http.begin(URL_FIRMWARE_VERSION);
    int httpCode = http.GET();

    if(httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        int newVersion = payload.toInt();

        if (BUILD_VERSION < newVersion){
            Serial.println("I need to update");
            t_httpUpdate_return ret = ESPhttpUpdate.update(ULR_FIRMWARE_BIN);

            if (ret == HTTP_UPDATE_FAILED){
                Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            }
        }
    }
}

void connectToWifi(void){

  //WiFi.begin("WLANSSID", "WLANPASSWD");
  WiFi.begin("feelflight", "feelflight");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void pinSetup(void){
    pinMode(PIN_LED_RED,   OUTPUT);
    pinMode(PIN_LED_BLUE,  OUTPUT);
    pinMode(PIN_VIBRATION, OUTPUT);
    pinMode(PIN_LED_R,     OUTPUT);
    pinMode(PIN_LED_G,     OUTPUT);
    pinMode(PIN_LED_B,     OUTPUT);

    digitalWrite(PIN_LED_RED   ,HIGH);
    digitalWrite(PIN_LED_BLUE  ,HIGH);
    digitalWrite(PIN_VIBRATION ,LOW);
    analogWrite(PIN_LED_R      ,0);
    analogWrite(PIN_LED_G      ,0);
    analogWrite(PIN_LED_B      ,0);
}

void setup_mqtt() {
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqtt_client.connect("bike")) {
      Serial.println("connected");
      mqtt_client.publish("blanket/system", "startup");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup(){
    Serial.begin(115200);
    Serial.print("My version:");
    Serial.println(BUILD_VERSION);
    Serial.println(ESP.getChipId());

    randomSeed(analogRead(0));
    pinSetup();
    vibrationForMilliseconds(500);
    pulseLED(true);
    connectToWifi();
    mqtt_client.setServer("172.26.2.68", 1883);
    setup_mqtt();
}

void eraseEEPROM(void){

  EEPROM.begin(512);
  for (int i = 0; i < 512; i++){
    EEPROM.write(i, 0);
  }
  EEPROM.end();

}

void loop(){

  //eraseEEPROM();
  Serial.println("Loop");
  setup_mqtt();
  mqtt_client.publish("blanket/1/ping", "1");
  mqtt_client.loop();
  delay(1000);

}
