#ifndef _DEBUG_H_
#define _DEBUG_H_

#define DEBUG                                         //Uncomment this line to debug via a connected serial terminal

#ifdef DEBUG
  #define DPRINT(...)    Serial.print(__VA_ARGS__)
  #define DPRINTLN(...)  Serial.println(__VA_ARGS__)
#else
  #define DPRINT(...)     //now defines a blank line
  #define DPRINTLN(...)   //now defines a blank line
#endif

#endif