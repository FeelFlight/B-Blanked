#ifndef NETWORK_H
#define NETWORK_H

#define WLANSSID   MYWLANSSID
#define WLANPASSWD MYWLANPASSWD

#define NETWORK_SCHEDULE 1000000

void     networkSetup        (void);
uint64_t networkLoop         (void);
void     networkconnectToWifi(void);

#endif
