#ifndef MQTTHANDLE_H
#define MQTTHANDLE_H

#define MQTT_SERVER   MYMQTTSERVER
#define MQTT_SCHEDULE 100

void     mqttSetup (void);
uint64_t mqttLoop  (void);

#endif
