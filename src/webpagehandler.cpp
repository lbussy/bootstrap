/* Copyright (C) 2019-2020 Lee C. Bussy (@LBussy)

This file is part of Lee Bussy's Brew Bubbbles (brew-bubbles).

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

#include "webpagehandler.h"

AsyncWebServer server(PORT);

void initWebServer()
{
    setRegPageAliases();
    setActionPageHandlers();
    setEditor();

    // File not found handler
    server.onNotFound([](AsyncWebServerRequest *request) {
        if (request->method() == HTTP_OPTIONS)
        {
            request->send(200);
        }
        else
        {
            Log.verbose(F("Serving 404." CR));
            request->redirect("/404/");
        }
    });

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    server.begin();

    Log.notice(F("Async HTTP server started on port %l." CR), PORT);
    Log.verbose(F("Open: http://%s.local to view controller application." CR), WiFi.getHostname());
}

void setRegPageAliases()
{
    // Regular page aliases

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.htm").setCacheControl("max-age=600");
    server.serveStatic("/index/", SPIFFS, "/").setDefaultFile("index.htm").setCacheControl("max-age=600");
    server.serveStatic("/404/", SPIFFS, "/").setDefaultFile("404.htm").setCacheControl("max-age=600");
}

void setActionPageHandlers()
{
    // Action Page Handlers

    server.on("/heap/", HTTP_GET, [](AsyncWebServerRequest *request) {
        uint32_t _heap = ESP.getFreeHeap();
        String heap = "Current heap: " + String(_heap);
        request->send(200, F("text/plain"), heap);
    });

    server.on("/ping/", HTTP_GET, [](AsyncWebServerRequest *request) {
        Log.verbose(F("Processing /ping/." CR));
        request->send(200, F("text/plain"), F("Ok."));
    });
}

void setEditor()
{
    // Setup SPIFFS editor
#ifdef ESP32
    server.addHandler(new SPIFFSEditor(SPIFFS, SPIFFSEDITUSER, SPIFFSEDITPW));
#elif defined(ESP8266)
    server.addHandler(new SPIFFSEditor(SPIFFSEDITUSER, SPIFFSEDITPW));
#endif
    server.on("/edit/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->redirect("/edit");
    });
}

void stopWebServer()
{
    server.reset();
    server.end();
    Log.notice(F("Web server stopped." CR));
}
