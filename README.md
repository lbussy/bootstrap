# Project Bootstrap

This is a generic PlatformIO project for ESP32/8266 including WiFi and debug logging.  It is intended to quickly give a semi-blank slate for new progects or just tests, without having to worry about coding in frequently used functions like Wifi and debug logging.

Features include:

- [WiFiManager](https://github.com/tzapu/WiFiManager) WiFi setup
- mDNS responder
- NTP support
- [ArduinoLog](https://github.com/thijse/Arduino-Log) to simplify serial debug printing
- [Async Web Server](https://github.com/me-no-dev/ESPAsyncWebServer/)
- SPIFFS editor

## To use:

- Clone the repo by issuing the command `git clone https://github.com/lbussy/bootstrap.git`
- `cd bootstrap`
- Sever the upstream relationship with the command `git remote remove upstream`
- Select the proper board type by commenting/uncommenting the desired `default_envs` in the `[platformio]` section of `platformio.ini`
- Optionally: Rename the directory to something other than `bootstrap`
- Upload file system image
- Upload program
- Connect to "bootstrap" AP with password "bootstrap" and configure your WiFi
- Optionally: Connect to SPIFFS editor at /edit/ with username "admin" and password "p@ssword"
