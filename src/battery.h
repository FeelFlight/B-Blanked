#ifndef BATTERY_H
#define BATTERY_H

#define BATTERY_SCHEDULE 1000000

void     batterySetup   (void);
uint64_t batteryLoop    (void);
uint16_t batteryGetLevel(void);

#endif
