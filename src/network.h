#ifndef NETWORK_H
#define NETWORK_H

#define WLANSSID   "feelflight"  //MYWLANSSID
#define WLANPASSWD "feelflight"  //MYWLANPASSWD

#define NETWORK_SCHEDULE 1000000

void     networkSetup (void);
uint64_t networkLoop  (void);
void     connectToWifi(void);

#endif
