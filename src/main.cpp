#include <Arduino.h>
#include "pin.h"
#include "light.h"
#include "main.h"
#include "vibration.h"
#include "network.h"
#include "firmware.h"
#include "mqtthandle.h"
#include "battery.h"

uint64_t sleeptime = 0;

void setup(){
    Serial.begin(115200);
    Serial.println("====================================");
    Serial.print("My version:");
    Serial.println(BUILD_VERSION);
    Serial.print("My id:");
    Serial.println(ESP.getChipId());

    pinSetup();
    networkSetup();
    firmwareSetup();
    lightSetup();
    mqttSetup();
    vibrationSetup();
    batterySetup();
}

void loop(){
  Serial.println("Loop");
  sleeptime = 1000;
  yield();
  sleeptime = min(sleeptime, firmwareLoop()  );
  yield();
  sleeptime = min(sleeptime, lightLoop()     );
  yield();
  sleeptime = min(sleeptime, mqttLoop()      );
  yield();
  sleeptime = min(sleeptime, networkLoop()   );
  yield();
  sleeptime = min(sleeptime, pinLoop()       );
  yield();
  sleeptime = min(sleeptime, vibrationLoop() );
  yield();
  sleeptime = min(sleeptime, batteryLoop()   );
  yield();
  delay(sleeptime);
}
