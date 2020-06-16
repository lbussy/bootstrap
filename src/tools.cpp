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

#include "tools.h"

void _delay(unsigned long ulDelay) {
    // Safe semi-blocking delay
#ifdef ESP32
    vTaskDelay(ulDelay); // Builtin to ESP32
#elif defined ESP8266
    unsigned long ulNow = millis();
    unsigned long ulThen = ulNow + ulDelay;
    while (ulThen > millis()) {
        yield(); // ESP8266 needs to yield()
    }
#endif
}

void resetController() {
    Log.notice(F("Reboot request - rebooting system." CR));
    _delay(5000);
    ESP.restart();
}

void setDoReset() {
    doReset = true; // Semaphore required for reset in callback
}

void setDoWiFiReset() {
    doWiFiReset = true; // Semaphore required for wifi reset in web page
}

void tickerLoop() {
    // Necessary because we cannot delay in a callback
    if (doReset) { // Check for Reset Pending
        doReset = false;
        resetController();
    }
    if (doWiFiReset) { // Need to do this to prevent WDT
        doWiFiReset = false;
        resetWifi();
    }
}

void printDebug()
{
    uint32_t free;
    uint16_t max;
    uint8_t frag;
#ifdef ESP8266
    ESP.getHeapStats(&free, &max, &frag);
#elif defined ESP32
    // total_free_bytes;      ///<  Total free bytes in the heap. Equivalent to multi_free_heap_size().
    // total_allocated_bytes; ///<  Total bytes allocated to data in the heap.
    // largest_free_block;    ///<  Size of largest free block in the heap. This is the largest malloc-able size.
    // minimum_free_bytes;    ///<  Lifetime minimum free heap size. Equivalent to multi_minimum_free_heap_size().
    // allocated_blocks;      ///<  Number of (variable size) blocks allocated in the heap.
    // free_blocks;           ///<  Number of (variable size) free blocks in the heap.
    // total_blocks;          ///<  Total number of (variable size) blocks in the heap.
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_INTERNAL);
    free = info.total_free_bytes;
    max  = info.largest_free_block;
    frag = 100 - (max * 100) / free;   
#endif
    Log.verbose(F("[MEM] Free Heap: %l | Largest Free Block: %l | Fragments: %d" CR), free, max, frag);
}

double convertFtoC(double F)
{
    // T(°C) = (T(°F) - 32) × 5/9
    return (F - 32) / 1.8;
}

double convertCtoF(double C)
{
    // T(°F) = T(°C) × 9/5 + 32
    return (C * 1.8) + 32;
}

double convertOneFtoC(double F)
{
    // T(°C) = (T(°F) X (5/9)) - (32/9))
    return F * 0.555;
}

double convertOneCtoF(double C)
{
    // T(°F) = T(°C) × 9/5 + 32
    return C * 1.8;
}

double convertGtoL(double G)
{
    // L = G / 0.26417
    return G / 0.26417;
}

double convertLtoG(double L)
{
    // G = L * 0.26417
    return L * 0.26417;
}

std::string addThousandSeparators(std::string value, char thousandSep = ',', char decimalSep = '.', char sourceDecimalSep = '.')
{
    int len = value.length();
    int negative = ((len && value[0] == '-') ? 1: 0);
    int dpos = value.find_last_of(sourceDecimalSep);
    int dlen = 3 + (dpos == std::string::npos ? 0 : (len - dpos));

    if (dpos != std::string::npos && decimalSep != sourceDecimalSep) {
        value[dpos] = decimalSep;
    }

    while ((len - negative) > dlen) {
        value.insert(len - dlen, 1, thousandSep);
        dlen += 4;
        len += 1;
    }
    return value;
}