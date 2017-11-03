#include <Arduino.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266HTTPClient.h>
#include "firmware.h"
#include "main.h"

uint64_t firmwareLastCallMillis = 0;

void firmwareSetup(void){
    firmwareLastCallMillis = millis();
}

uint64_t firmwareLoop(void){
    if( (millis() - firmwareLastCallMillis) > FIRMWARE_SCHEDULE){
        firmwareLastCallMillis = millis();
    }
    return (firmwareLastCallMillis + FIRMWARE_SCHEDULE);
}

void firmwarecheckForNew(void){

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
        }else{
            Serial.println("No need to update firmware.");
        }
    }
}
