#ifndef VIBRATION_H
#define VIBRATION_H

#define VIBRATION_SCHEDULE 1000000

void     vibrationSetup          (void    );
uint64_t vibrationLoop           (void    );
void     vibrationForMilliseconds(uint16_t);

#endif
