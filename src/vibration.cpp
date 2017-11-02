#include <Arduino.h>
#include <Ticker.h>
#include "pin.h"
#include "vibration.h"

Ticker   tickerVibration;
uint64_t vibrationLastCallMillis = 0;

void vibrationSetup(void){
    vibrationLastCallMillis = millis();
}

uint64_t vibrationLoop(void){
  if( (millis() - vibrationLastCallMillis) > VIBRATION_SCHEDULE){
    vibrationLastCallMillis = millis();
  }
  return (vibrationLastCallMillis + VIBRATION_SCHEDULE);
}

void vibrationOff(void){
    digitalWrite(PIN_VIBRATION, LOW);
}

void vibrationForMilliseconds(uint16_t time){
    digitalWrite(PIN_VIBRATION, HIGH);
    tickerVibration.once_ms(time, vibrationOff);
}
