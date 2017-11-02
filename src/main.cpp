#include <Arduino.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <PubSubClient.h>
#include <EEPROM.h>

#define BUILD_VERSION        REPLACE_WITH_CURRENT_VERSION
#define ULR_FIRMWARE_BIN     "http://s3.amazonaws.com/feelflight/firmware/blanket.bin"
#define URL_FIRMWARE_VERSION "http://s3.amazonaws.com/feelflight/firmware/blanket.version"
#define MQTT_SERVER          "172.26.2.68"

#define PIN_LED_RED     0
#define PIN_LED_BLUE    2

#define PIN_SDA         4
#define PIN_SCL         5

#define PIN_MISO       12
#define PIN_MOSI       13
#define PIN_SCK        14
#define PIN_15         15
#define PIN_16         16

#define PIN_VIBRATION PIN_15
#define PIN_LED_R     PIN_MISO
#define PIN_LED_G     PIN_MOSI
#define PIN_LED_B     PIN_SCK

Ticker  tickerLED;
Ticker  tickerVibration;
uint8_t pulseLED_intensity    = 0;
boolean pulseLED_direction_up = true;

WiFiClient   espClient;
PubSubClient mqtt_client(espClient);

int8_t myID = 0; // > 0 Real ID, 0 not set, < 0 requested

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

void vibrationForMilliseconds(uint16_t time){
    digitalWrite(PIN_VIBRATION, HIGH);
    tickerVibration.once_ms(time, vibrationOff);
}

void vibrationOff(void){
    digitalWrite(PIN_VIBRATION, LOW);
}

void light(uint8_t r, uint8_t g, uint8_t b){
    analogWrite(PIN_LED_R ,r);
    analogWrite(PIN_LED_G ,g);
    analogWrite(PIN_LED_B ,b);
}

void pulseStep(void){

    if(pulseLED_direction_up){
        pulseLED_intensity++;
    }else{
        pulseLED_intensity--;
    }

    if(pulseLED_intensity == 100){
        pulseLED_direction_up = false;
    }

    if(pulseLED_intensity ==   0){
        pulseLED_direction_up = true;
    }

    light(255.0 * (pulseLED_intensity / 100.0), 162.0 * (pulseLED_intensity / 100.0), 0);

}

void pulseLED(boolean state){
    if (state){
        pulseLED_intensity    = 0;
        pulseLED_direction_up = true;
        tickerLED.attach_ms(50, pulseStep);
    }else{
        tickerLED.detach();
        light(0, 0, 0);
    }
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
    getMyID(); // To trigger request

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
  mqtt_client.publish("blanket/1/ping", millis());
  mqtt_client.loop();
  delay(1000);

}
