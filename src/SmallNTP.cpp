#include "SmallNTP.h"

/* SmallNTP by GuruSR (https://www.github.com/GuruSR/SmallNTP)
 * Original version corrected from Arduino, which is an industry standard copy
 * of the normal NTP function which has been around for decades.
 *
 * Version 1.0, January   2, 2022
 * Version 1.1, January  11, 2022 - Fix internal data.
 * Version 1.2, January  29, 2022 - Added Make & Break Time functions to MATCH
 *                                  TimeLib & time.h by reducing
 *                                  Month and Wday.
 * Version 1.3, December 17, 2023 - Added 2 seconds onto time collection to
 *                                  account for communication.
 * Version 1.4, July     22, 2024 - Removed the unneeded results.
 * Version 1.5, May       3, 2025 - Added NTP V4 correction.  Drift math fix.
 *
 * This library offers async NTP, which returns the seconds from Epoch (1970
 * and beyond) for use with Watchy watch face code.
 *
 * This library is adapted to work with the Arduino ESP32 and any other project
 * that has similar libraries.
 *
 * MIT License
 *
 * Copyright (c) 2022 GuruSR
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

SmallNTP::SmallNTP () {}

void
SmallNTP::Begin (String NTPServer)
{
  IPAddress NTPAddress;
  WiFiG.hostByName (NTPServer.c_str (), NTPAddress);
  memset (ntpPacketBuffer, 0, NTP_PACKET_SIZE);
  ntpPacketBuffer[0] = 0b11100011; // LI, Version, Mode
  ntpPacketBuffer[1] = 0;          // Stratum, or type of clock
  ntpPacketBuffer[2] = 6;          // Polling Interval
  ntpPacketBuffer[3] = 0xEC;       // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  ntpPacketBuffer[12] = 49;
  ntpPacketBuffer[13] = 0x4E;
  ntpPacketBuffer[14] = 49;
  ntpPacketBuffer[15] = 52;
  ntpClient.beginPacket (NTPAddress, 123); // NTP requests are to port 123
  ntpClient.write (ntpPacketBuffer, NTP_PACKET_SIZE);
  ntpClient.endPacket ();
}

bool
SmallNTP::Query ()
{
  _timedata t_0, t_1, t_2, t_3, t_4;
  uint8_t tc = 0;
  if (ntpClient.parsePacket () >= NTP_PACKET_SIZE)
    {
      ntpClient.read (ntpPacketBuffer, NTP_PACKET_SIZE);
      ntpClient.flush ();
      t_0._tdata.sec
          = ((uint32_t)ntpPacketBuffer[16] << 24
             | (uint32_t)ntpPacketBuffer[17] << 16
             | (uint32_t)ntpPacketBuffer[18] << 8 | ntpPacketBuffer[19]);
      t_0._tdata.psec
          = ((uint32_t)ntpPacketBuffer[20] << 24
             | (uint32_t)ntpPacketBuffer[21] << 16
             | (uint32_t)ntpPacketBuffer[22] << 8 | ntpPacketBuffer[23]);
      t_1._tdata.sec
          = ((uint32_t)ntpPacketBuffer[24] << 24
             | (uint32_t)ntpPacketBuffer[25] << 16
             | (uint32_t)ntpPacketBuffer[26] << 8 | ntpPacketBuffer[27]);
      t_1._tdata.psec
          = ((uint32_t)ntpPacketBuffer[28] << 24
             | (uint32_t)ntpPacketBuffer[29] << 16
             | (uint32_t)ntpPacketBuffer[30] << 8 | ntpPacketBuffer[31]);
      t_2._tdata.sec
          = ((uint32_t)ntpPacketBuffer[32] << 24
             | (uint32_t)ntpPacketBuffer[33] << 16
             | (uint32_t)ntpPacketBuffer[34] << 8 | ntpPacketBuffer[35]);
      t_2._tdata.psec
          = ((uint32_t)ntpPacketBuffer[36] << 24
             | (uint32_t)ntpPacketBuffer[37] << 16
             | (uint32_t)ntpPacketBuffer[38] << 8 | ntpPacketBuffer[39]);
      t_3._tdata.sec
          = ((uint32_t)ntpPacketBuffer[40] << 24
             | (uint32_t)ntpPacketBuffer[41] << 16
             | (uint32_t)ntpPacketBuffer[42] << 8 | ntpPacketBuffer[43]);
      t_3._tdata.psec
          = ((uint32_t)ntpPacketBuffer[44] << 24
             | (uint32_t)ntpPacketBuffer[45] << 16
             | (uint32_t)ntpPacketBuffer[46] << 8 | ntpPacketBuffer[47]);
      t_4.__rawtdata = 0;
      if (t_1.__rawtdata && t_0.__rawtdata)
        {
          t_4.__rawtdata = (t_1.__rawtdata - t_0.__rawtdata);
          tc++;
          if (t_2.__rawtdata && t_3.__rawtdata)
            {
              t_4.__rawtdata += (t_2.__rawtdata - t_3.__rawtdata);
              tc++;
            }
          if (tc > 1)
            {
              t_4.__rawtdata /= 2ULL;
              t_3._tdata.sec += t_4._tdata.sec;
            }
        }
      SmallNTP::Results = t_3._tdata.sec - 2208988800UL;
      SmallNTP::BreakTime (SmallNTP::Results, SmallNTP::tmResults);
      return true;
    }
  return false;
}

void
SmallNTP::End ()
{
  ntpClient.stop ();
}

time_t
SmallNTP::MakeTime (tmElements_t TM)
{
  TM.Month++;
  TM.Wday++;
  return makeTime (TM);
}

void
SmallNTP::BreakTime (time_t &T, tmElements_t &TM)
{
  breakTime (T, TM);
  TM.Month--;
  TM.Wday--;
}
