#pragma once

#define DEBUG

#ifdef DEBUG
#define Log(msg) Serial.println(msg)
#define LogVal(msg, val) Serial.print(msg);Serial.println(val);
#else
#define Log(msg)
#define LogVal(msg, val)
#endif
