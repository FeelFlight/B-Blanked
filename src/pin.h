#ifndef PIN_H
#define PIN_H

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

#define PIN_SCHEDULE 1000000


void     pinSetup(void);
uint64_t pinLoop (void);

#endif
