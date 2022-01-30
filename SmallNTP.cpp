#include "SmallNTP.h"

/* SmallNTP by GuruSR (https://www.github.com/GuruSR/SmallNTP)
 * Original version corrected from Arduino, which is an industry standard copy of the normal NTP function
 * which has been around for decades.
 * 
 * Version 1.0, January  2, 2022
 * Version 1.1, January 11, 2022 - Fix internal data.
 * Version 1.2, January 29, 2022 - Added Make & Break Time functions to MATCH TimeLib & time.h by reducing Month and Wday.
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

SmallNTP::SmallNTP() {}

void SmallNTP::Begin(String NTPServer){
    IPAddress NTPAddress;
    WiFiG.hostByName(NTPServer.c_str(), NTPAddress);
    memset(ntpPacketBuffer, 0, NTP_PACKET_SIZE);
    ntpPacketBuffer[0] = 0b11100011;   // LI, Version, Mode
    ntpPacketBuffer[1] = 0;     // Stratum, or type of clock
    ntpPacketBuffer[2] = 6;     // Polling Interval
    ntpPacketBuffer[3] = 0xEC;  // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    ntpPacketBuffer[12]  = 49;
    ntpPacketBuffer[13]  = 0x4E;
    ntpPacketBuffer[14]  = 49;
    ntpPacketBuffer[15]  = 52;
    ntpClient.beginPacket(NTPAddress, 123); //NTP requests are to port 123
    ntpClient.write(ntpPacketBuffer, NTP_PACKET_SIZE);
    ntpClient.endPacket();
}

bool SmallNTP::Query(){
  if(ntpClient.parsePacket() >= NTP_PACKET_SIZE){
    ntpClient.read(ntpPacketBuffer, NTP_PACKET_SIZE);
    ntpClient.flush();
    SmallNTP::Results1900 = ((uint32_t)ntpPacketBuffer[40] << 24 | (uint32_t)ntpPacketBuffer[41] << 16 | (uint32_t)ntpPacketBuffer[42] << 8 | ntpPacketBuffer[43]);
	SmallNTP::Results = SmallNTP::Results1900 - 2208988800UL;	// - 1970 seconds.
    SmallNTP::BreakTime(SmallNTP::Results,SmallNTP::tmResults);
    SmallNTP::BreakTime(SmallNTP::Results1900,SmallNTP::tmResults1900);
    return true;
  }
  return false;
}

void SmallNTP::End(){ ntpClient.stop(); }

time_t SmallNTP::MakeTime(tmElements_t TM){
	TM.Month++;
	TM.Wday++;
	return makeTime(TM);
}

void SmallNTP::BreakTime(time_t &T, tmElements_t &TM){
	breakTime(T,TM);
	TM.Month--;
	TM.Wday--;
}
