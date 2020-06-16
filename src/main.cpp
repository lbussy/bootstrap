#include "main.h"

void setup()
{
    serial();; // Starts serial and ArduinoLog

    // Set wifi reset pin
    pinMode(RESETWIFI, INPUT_PULLUP);
    // Set LED pin
    pinMode(LED, OUTPUT);
    // Check if portal is requested
    if (digitalRead(RESETWIFI) == LOW)
    {
        Log.notice(F("Pin %d low, presenting portal." CR), RESETWIFI);
        doWiFi(true);
    }
    else
    {
        Log.verbose(F("WiFi: Normal boot." CR));
        doWiFi();
    }

    setClock();         // Set NTP Time

    mdnssetup();        // Set up mDNS responder
    tcpCleanup();       // Get rid of -8 errors
    initWebServer();    // Turn on web server

    Log.notice(F("Started %s version %s (%s) [%s]." CR), API_KEY, version(), branch(), build());
}

void loop()
{
    // Declarations for main program
    // ...
    while (true)
    { // Loop for program
#ifdef ESP8266
        MDNS.update();
        yield();
#endif
    }
}
