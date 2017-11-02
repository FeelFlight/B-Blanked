#include <Arduino.h>
#include <Ticker.h>
#include "pin.h"
#include "light.h"
#include "main.h"

Ticker  tickerLED;
uint8_t pulseLED_intensity    = 0;
boolean pulseLED_direction_up = true;

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
