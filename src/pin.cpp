#include <Arduino.h>
#include <pin.h>

uint64_t pinLastCallMillis = 0;

void pinSetup(void){
    pinLastCallMillis = millis();

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

uint64_t pinLoop(void){
  if( (millis() - pinLastCallMillis) > PIN_SCHEDULE){
    pinLastCallMillis = millis();
  }
  return (pinLastCallMillis + PIN_SCHEDULE);
}