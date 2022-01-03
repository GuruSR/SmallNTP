# SmallNTP
Original version corrected from Arduino, which is an industry standard copy of the normal NTP function which has been around for decades.  This library offers async NTP, which returns the seconds from Epoch (1970 and beyond) in 2 variations for use with Watchy watch face code.

 **Begin(String NTPServer):**  Start a call to the NTPServer asking for the current Epoch time (UTC starting at 1970).
 
 **bool Query():**  Returns true if the NTPServer's response has come in, this can be done in a loop.
 
 **End():**  Stop SmallNTP from continuing in the background, do this if you get a Query() result of true or false.
 
 
 Two variables that are filled when **Query()** returns `true`.
 
 **time_t Results:** has the results of the NTPServer response in `time_t` format, ready for use with `localtime`.
 
 **tmElements_t tmResults** has the results from above in a format to use in either WatchyRTC or SmallRTC (GuruSR).
 
 Neither of the two above variables survive a Deep Sleep, do not rely on them after a Deep Sleep.
