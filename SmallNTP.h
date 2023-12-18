#ifndef SMALL_NTP_H
#define SMALL_NTP_H

/* SmallNTP by GuruSR (https://www.github.com/GuruSR/SmallNTP)
 * Original version corrected from Arduino, which is an industry standard copy of the normal NTP function
 * which has been around for decades.
 * 
 * Version 1.0, January  2, 2022
 * Version 1.1, January 11, 2022 - Fix internal data.
 * Version 1.2, January 29, 2022 - Added Make & Break Time functions to MATCH TimeLib & time.h by reducing Month and Wday.
 * Version 1.3, December 17, 2023 - Added 2 seconds onto time collection to account for communication.
 *
 * This library offers async NTP, which returns the seconds from Epoch (1970 and beyond) in 2 variations
 * for use with Watchy watch face code.
 *
 * This library is adapted to work with the Arduino ESP32 and any other project that has similar libraries.
 *
 * MIT License
 *
 * Copyright (c) 2022 GuruSR
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <WiFiGeneric.h>

#define NTP_PACKET_SIZE 48

class SmallNTP {
    public:
        time_t Results;
		time_t Results1900;
        tmElements_t tmResults;
        tmElements_t tmResults1900;
    public:
        SmallNTP();
        void Begin(String NTPServer);
        bool Query();
        void End();
        time_t MakeTime(tmElements_t TM);
        void BreakTime(time_t &T, tmElements_t &TM);
    private:
        WiFiUDP ntpClient;
        WiFiGenericClass WiFiG;
        byte ntpPacketBuffer[NTP_PACKET_SIZE];
};
#endif
