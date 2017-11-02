#ifndef LIGHT_H
#define LIGHT_H

#define LIGHT_SCHEDULE 1000000

enum Colour { red, green, blue };

void     lightSetup(void           );
uint64_t lightLoop (void           );
void     pulseLED  (boolean        );
void     setColour (Colour, uint8_t);

#endif
