#pragma once

#define DEBUG

#ifdef DEBUG
#define Log(msg) Serial.println(msg)
#else
#define Log(msg)
#endif
