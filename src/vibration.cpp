#include <Arduino.h>
#include <Ticker.h>
#include "pin.h"
#include "vibration.h"

Ticker  tickerVibration;

void vibrationOff(void){
    digitalWrite(PIN_VIBRATION, LOW);
}

void vibrationForMilliseconds(uint16_t time){
    digitalWrite(PIN_VIBRATION, HIGH);
    tickerVibration.once_ms(time, vibrationOff);
}
