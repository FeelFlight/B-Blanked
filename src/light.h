#ifndef LIGHT_H
#define LIGHT_H

#define LIGHT_SCHEDULE 1000000

void     lightSetup(void                              );
uint64_t lightLoop (void                              );
void     pulseLED  (uint8_t, boolean                  );
void     light     (uint8_t, uint8_t, uint8_t, uint8_t);
void     flashLED  (uint8_t, uint8_t                  );

#endif
