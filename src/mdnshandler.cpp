/* Copyright (C) 2019-2020 Lee C. Bussy (@LBussy)

This file is part of Lee Bussy's Bootstrap (bootstrap).

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

#include "mdnshandler.h"

void mdnssetup()
{
#ifdef ESP8266
    if (!MDNS.begin(WiFi.hostname())) // Start the mDNS responder
#elif defined ESP32
    if (!MDNS.begin(WiFi.getHostname())) // Start the mDNS responder
#endif
    {
        Log.error(F("Error setting up mDNS responder." CR));
    }
    else
    {
#ifdef ESP8266
        Log.notice(F("mDNS responder started for %s.local." CR), WiFi.hostname().c_str());
#elif defined ESP32
        Log.notice(F("mDNS responder started for %s.local." CR), WiFi.getHostname());
#endif
        MDNS.addService("http", "tcp", PORT);
        Log.notice(F("HTTP registered via mDNS on port %i." CR), PORT);
    }
}

void mdnsreset()
{
    MDNS.end();
    if (!MDNS.begin(HOSTNAME))
    {
        Log.error(F("Error resetting MDNS responder."));
    }
    else
    {
#ifdef ESP8266
        Log.notice(F("mDNS responder restarted, hostname: %s.local." CR), WiFi.hostname().c_str());
#elif defined ESP32
        Log.notice(F("mDNS responder restarted, hostname: %s.local." CR), WiFi.getHostname());
#endif
        MDNS.addService("http", "tcp", 80);
    }
}
