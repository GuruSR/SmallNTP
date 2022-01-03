#ifndef SMALL_NTP_H
#define SMALL_NTP_H

#include <Timelib.h>
#include <WiFiUdp.h>
#include <WiFiGeneric.h>

#define NTP_PACKET_SIZE 48

class SmallNTP {
    public:
        time_t Results;
        tmElements_t tmResults;
    public:
        SmallNTP();
        void Begin(String NTPServer);
        bool Query();
        void End();
    private:
        WiFiUDP ntpClient;
        WiFiGenericClass WiFiG;
        byte ntpPacketBuffer[NTP_PACKET_SIZE];
};
#endif
