#ifndef SETTINGS_H
#define SETTINGS_H

// WiFi přihlašovací údaje
const char* WIFI_SSID = "";
const char* WIFI_PASSWORD = "";

// Statické IP nastavení sítě
const IPAddress local_IP(xxx,xxx,xxx,xxx);
const IPAddress gateway(xxx,xxx,xxx,xxx);
const IPAddress subnet(255,255,255,0);
const IPAddress primaryDNS(8, 8, 8, 8);   // optional
const IPAddress secondaryDNS(8, 8, 4, 4); // optional

// Nastavení času
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;           // Časový posun (ČR = UTC+1)
const int daylightOffset_sec = 3600;       // Letní čas: +1 hodina

// Interval vzorkování dat (v milisekundách)
//const unsigned long UPDATE_INTERVAL_MS = 1 * 60 * 1000;  // 1 minuta
const unsigned long UPDATE_INTERVAL_MS = 15 * 60 * 1000;  // 15 minut

const unsigned long UPDATE_INTERVAL_MIN = UPDATE_INTERVAL_MS / 60000;  // v minutach

// Počet vzorků měření hodnoty pro graf za určitý čas
// 0,5 hodiny / 1 minuta
//const int HISTORY_LENGTH = 30;  // 0,5 * (60 / 1) = 30 měření
// 12 hodin / 15 minut
const int HISTORY_LENGTH = 48;  // 12 * (60 / 15) = 48 měření

// Spořič obrazovky (v milisekundách)
const unsigned long SCREENSAVER_DELAY_MS = 30000;  // 30 sekund

// Jas displeje (rozsah 0–255)
#define BRIGHTNESS_OFF     0
#define BRIGHTNESS_LOW     64    // 25 %
#define BRIGHTNESS_MEDIUM  127   // 50 %
#define BRIGHTNESS_HIGH    191   // 75 %
#define BRIGHTNESS_MAX     255   // 100 %

const int BACKLIGHT_BRIGHTNESS = BRIGHTNESS_LOW;  // Nastav jas spořiče

#endif 
