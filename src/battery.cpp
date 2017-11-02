#include <Arduino.h>
#include "pin.h"
#include "battery.h"

uint64_t batteryLastCallMillis    = 0;

void batterySetup(void){
    batteryLastCallMillis = millis();
}

uint64_t batteryLoop(void){
    if( (millis() - batteryLastCallMillis) > BATTERY_SCHEDULE){
        batteryLastCallMillis = millis();
    }
    return (batteryLastCallMillis + BATTERY_SCHEDULE);
}

uint16_t batteryGetLevel(void){
    return 2134;
}
