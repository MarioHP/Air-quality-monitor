#ifndef SETTINGS_H
#define SETTINGS_H
// ENGLISH
// WiFi credentials
const char* WIFI_SSID = "";
const char* WIFI_PASSWORD = "";

// Static IP network settings
const IPAddress local_IP(xxx,xxx,xxx,xxx);
const IPAddress gateway(xxx,xxx,xxx,xxx);
const IPAddress subnet(255,255,255,0);
const IPAddress primaryDNS(8, 8, 8, 8);   // Google DNS
const IPAddress secondaryDNS(8, 8, 4, 4); // Google DNS

// Set time
const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const char* timeZone = "CET-1CEST,M3.5.0/2,M10.5.0/3"; // Central European Time + Summer Time

// Data sampling interval (in milliseconds)
//const unsigned long UPDATE_INTERVAL_MS = 1 * 60 * 1000;  // 1 minute
const unsigned long UPDATE_INTERVAL_MS = 15 * 60 * 1000;  // 15 minutes

const unsigned long UPDATE_INTERVAL_MIN = UPDATE_INTERVAL_MS / 60000;  // in minutes

// Number of sample values ​​for the graph over a period of time
// 0.5 hours / 1 minute
//const int HISTORY_LENGTH = 30;  // 0,5 * (60 / 1) = 30 measurements
// 12 hours / 15 minutes
const int HISTORY_LENGTH = 48;  // 12 * (60 / 15) = 48 measurements

// Screensaver (in milliseconds)
const unsigned long SCREENSAVER_DELAY_MS = 30000;  // 30 seconds

// Display brightness (range 0–255)
#define BRIGHTNESS_OFF     0
#define BRIGHTNESS_LOW     64    // 25 %
#define BRIGHTNESS_MEDIUM  127   // 50 %
#define BRIGHTNESS_HIGH    191   // 75 %
#define BRIGHTNESS_MAX     255   // 100 %

const int BACKLIGHT_BRIGHTNESS = BRIGHTNESS_LOW;  // Set the screen saver brightness

#endif 