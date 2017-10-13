#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#define BUILD_VERSION REPLACE_WITH_CURRENT_VERSION
ESP8266WiFiMulti WiFiMulti;

void updateFirmware(void){

  t_httpUpdate_return ret = ESPhttpUpdate.update("http://s3.amazonaws.com/feelflight/firmware/blanked.bin");

  switch(ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      delay(1000);
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("HTTP_UPDATE_NO_UPDATES");
      break;

    case HTTP_UPDATE_OK:
      Serial.println("HTTP_UPDATE_OK");
      delay(1000);
      Serial.println("Resetting");
      ESP.reset();
      delay(1000);
    break;
  }

}

boolean checkForNewFirmware(void){

  HTTPClient http;

  Serial.print("[HTTP] begin...\n");
  http.begin("http://s3.amazonaws.com/feelflight/firmware/blanked.version");

  Serial.print("[HTTP] GET...\n");
  int httpCode = http.GET();

  if(httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      int newVersion = payload.toInt();
      Serial.println(newVersion);

      if (BUILD_VERSION < newVersion){
        Serial.println("I need to update");
        updateFirmware();
      }else{
        Serial.println("No need to update");
      }

    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  return true;
}

void setup(){
    Serial.begin(115200);

    for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }

    Serial.println("Version:" + BUILD_VERSION);

    WiFiMulti.addAP("WLANSSID", "WLANPASSWD");
}

void loop(){

    if((WiFiMulti.run() == WL_CONNECTED)) {
      Serial.println("Connected");
      checkForNewFirmware();
      delay(10000);
    }else{
      Serial.println("Waiting for connection");
      delay(1000);
    }
}
