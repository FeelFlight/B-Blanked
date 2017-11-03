#ifndef FIRMWARE_H
#define FIRMWARE_H

#define BUILD_VERSION        33 //REPLACE_WITH_CURRENT_VERSION
#define ULR_FIRMWARE_BIN     "http://s3.amazonaws.com/feelflight/firmware/blanket.bin"
#define URL_FIRMWARE_VERSION "http://s3.amazonaws.com/feelflight/firmware/blanket.version"

#define FIRMWARE_SCHEDULE 1000000

void     firmwareSetup      (void);
uint64_t firmwareLoop       (void);
void     firmwarecheckForNew(void);

#endif
