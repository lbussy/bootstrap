#ifndef _MAIN_H
#define _MAIN_H

#ifdef ESP32
#include <ESPmDNS.h>
#elif defined ESP8266
#include <ESP8266mDNS.h>
#endif

#include "config.h"
#include "wifihandler.h"
#include "webpagehandler.h"
#include "ntphandler.h"
#include "version.h"
#include "tools.h"
#include "serialhandler.h"

#include <ArduinoLog.h>
#include <Arduino.h>

void setup();
void loop();

#endif // _MAIN_H
