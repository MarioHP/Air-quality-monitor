// SETTINGS:
// Sensirion web: https://github.com/Sensirion/arduino-i2c-sen66
// Open C:\Users\xxxx\Documents\Arduino\libraries\TFT_eSPI\User_Setup_select.h
// Comment the line #include <User_Setup.h>
// Uncomment the ine #include <User_Setups/Setup25_TTGO_T_Display.h>
// Board: esp32 ver. 2.0.17 (ESP32 Dev Module), Library: TFT_eSPI ver. 2.5.43, Sesnirion I2C SEN66 ver. 1.0.1.

// For the ElegantOTA library to work in async mode (with the ESPAsyncWebServer library), you need to do the following procedure.
// 1) Go to your Arduino libraries directory.
// 2) Open the ElegantOTA folder and then open the src folder.
// 3) Locate the ELEGANTOTA_USE_ASYNC_WEBSERVER macro in the ElegantOTA.h file, and set it to 1 as follows:
//   #define ELEGANTOTA_USE_ASYNC_WEBSERVER 1

// update firmware via WiFi http://localIP/update (Sketch > Export Compiled Binary - ino.bin)

#include "settings.h"
#include "bootlogo.h"
#include <Wire.h>
#include <SensirionI2cSen66.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <time.h>
#include <ESPAsyncWebServer.h>
#include "icons.h"
#include <driver/ledc.h>
#include <ElegantOTA.h>

#include "FontNotoSansBold15pt_mp.h"
#include "FontNotoSansBold20.h"
#include "FontNotoSansBold90ptDigits.h"

SensirionI2cSen66 sensor;
TFT_eSPI tft = TFT_eSPI(135, 240);
AsyncWebServer server(80);

#define BUTTON1 0  // S1
#define BUTTON2 35 // S2
#define BACKLIGHT_PIN 4 // pin podsviceni
#define PWM_CHANNEL 0
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8

#define TFT_MONITORGREY  0xCE79   /* 204, 204, 204 */ 
#define TFT_MONITORGREEN  0x4D02   /*  73, 160,  16 */

#define MINI_FONT FontNotoSansBold15pt_mp
#define SMALL_FONT FontNotoSansBold20
#define BIG_FONT FontNotoSansBold90ptDigits

// Měření
float pm1 = 0, pm2p5 = 0, pm4 = 0, pm10 = 0;

float lastCo2 = NAN;
float lastTemp = NAN;
float lastRh = NAN;
float lastPm2p5 = NAN;
float vocIndex = NAN;
float noxIndex = NAN;

bool displayShowTemperature = true;
bool displayShowHumidity = true;

float co2History[HISTORY_LENGTH];
float tempHistory[HISTORY_LENGTH];
float rhHistory[HISTORY_LENGTH];
String timestampHistory[HISTORY_LENGTH];
String startDateTime;

unsigned long lastUpdate = 0;
int viewMode = 0; // 0 = Home, 1 = Temp, 2 = RH, 3 = PM1.0, 4 = PM2.5, 5 = PM4.0, 6 = PM10

// Spořič
unsigned long lastActivity = 0; 
bool isScreensaverOn = false;

// Čekání na čas z NTP
bool waitForTime(unsigned long timeout_ms = 10000) {
  struct tm timeinfo;
  unsigned long start = millis();
  while (millis() - start < timeout_ms) {
    if (getLocalTime(&timeinfo)) {
      Serial.println("Čas synchronizován:");
      Serial.println(&timeinfo, "%d.%m.%Y %H:%M:%S");
      return true;
    }
    delay(500);
  }
  Serial.println("NTP synchronizace selhala");
  return false;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  sensor.begin(Wire, SEN66_I2C_ADDR_6B);

  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BACKLIGHT_PIN, OUTPUT);

  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  tft.pushImage(0, 0, 240, 135, bootlogo);
  delay(3000);

  tft.fillScreen(TFT_BLACK);
  tft.loadFont(SMALL_FONT);
  tft.setTextDatum(MC_DATUM);  
  tft.setTextColor(TFT_MONITORGREY, TFT_BLACK);
  tft.drawString("Pripojuji k WiFi", 120, 65);

  // Pokud chceš statickou IP
  WiFi.config(local_IP, gateway, subnet); 

  // Odpojit a resetovat WiFi
  WiFi.disconnect(true);
  delay(100);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("--- START SETUP ---");
  Serial.println("Begin WiFi.begin with SSID='" + String(WIFI_SSID) + "'");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tft.print(".");
  }
  Serial.println();
  Serial.println("WiFi připojeno");
  Serial.println("IP adresa: " + WiFi.localIP().toString());

  // Na displej po připojení
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);  
  tft.setTextColor(TFT_MONITORGREY);
  tft.drawString("WiFi pripojeno", 120, 50); 
  tft.drawString("IP: " + WiFi.localIP().toString(), 120, 80);
  delay(3000);

  // Nastavení a čekání na NTP čas
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  if (!waitForTime(10000)) {  // čekat max 10 sekund na NTP
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_RED);
    tft.drawString("NTP synch. selhala", 120, 65);
    Serial.println("NTP synchronizace selhala");
    delay(3000);
    
  } else {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_MONITORGREEN);
    tft.drawString("NTP OK", 120, 65);
    Serial.println("NTP OK");   
    delay(1500);
  }

  // čas spuštění
  startDateTime = getDateTimeString();  

  // Start webserveru a OTA
  ElegantOTA.begin(&server);   

  server.on("/", HTTP_GET, handleRoot);
  server.on("/data", HTTP_GET, handleData);

  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "image/x-icon", favicon_ico, favicon_ico_len);
  });

  server.on("/headericon.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "image/png", header_ico, header_ico_len);
  });

  server.begin();

  // Start senzoru
  sensor.startContinuousMeasurement();
  // Čekání na zahřátí sensoru (60 s dle datasheetu)
  tft.fillScreen(TFT_BLACK);
  tft.loadFont(SMALL_FONT);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_YELLOW);
  tft.drawString("Senzor se zahriva...", 120, 50);
  Serial.println("Senzor se zahřívá");

  // Odpočet
  for (int i = 60; i > 0; i--) {
    tft.fillRect(30, 70, 180, 30, TFT_BLACK);  
    String countdownText = "Zbyva: " + String(i) + " s";
    tft.setTextColor(TFT_YELLOW); 
    tft.drawString(countdownText, 120, 80);
    delay(1000);
  }

  Serial.println("Senzor se zahřál");

  // První update
  updateSensorAndDisplay();

  lastUpdate = millis();
  lastActivity = millis();
}

void shiftHistory(float co2, float temp, float rh) {
  for (int i = HISTORY_LENGTH - 1; i > 0; i--) {
    co2History[i] = co2History[i - 1];
    tempHistory[i] = tempHistory[i - 1];
    rhHistory[i] = rhHistory[i - 1];
    timestampHistory[i] = timestampHistory[i - 1];
  }
  co2History[0] = co2;
  tempHistory[0] = temp;
  rhHistory[0] = rh;
  timestampHistory[0] = getTimeString(); 
}


// Získání času jako text 
String getTimeString() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "??:??";
  }
  char timeStr[6];
  sprintf(timeStr, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
  return String(timeStr);
}
// Získání data a času jako text
String getDateTimeString() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "??.??.???? ??:??";
  }
  char dateTimeStr[25];
  snprintf(dateTimeStr, sizeof(dateTimeStr), "%02d.%02d.%04d %02d:%02d",
        timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900,
        timeinfo.tm_hour, timeinfo.tm_min);
  return String(dateTimeStr);
}


void handleData(AsyncWebServerRequest *request) {
  String json = "{";

  // Poslední hodnoty (aktuální)
  json += "\"co2\":" + String(co2History[0], 1) + ",";
  json += "\"temp\":" + String(tempHistory[0], 1) + ",";
  json += "\"rh\":" + String(rhHistory[0], 1) + ",";
  json += "\"pm1\":" + String(pm1, 1) + ",";
  json += "\"pm2p5\":" + String(pm2p5, 1) + ",";
  json += "\"pm4\":" + String(pm4, 1) + ",";
  json += "\"pm10\":" + String(pm10, 1) + ",";
  json += "\"noxIndex\":" + String(noxIndex, 1) + ",";
  json += "\"vocIndex\":" + String(vocIndex, 1) + ",";


// Historie CO₂
json += "\"co2History\":[";
for (int i = HISTORY_LENGTH - 1; i >= 0; i--) {
  json += String(co2History[i], 1);
  if (i > 0) json += ",";
}
json += "],";

// Historie teploty
json += "\"tempHistory\":[";
for (int i = HISTORY_LENGTH - 1; i >= 0; i--) {
  json += String(tempHistory[i], 1);
  if (i > 0) json += ",";
}
json += "],";

// Historie vlhkosti
json += "\"rhHistory\":[";
for (int i = HISTORY_LENGTH - 1; i >= 0; i--) {
  json += String(rhHistory[i], 1);
  if (i > 0) json += ",";
}
json += "],"; 

// Historie času 
json += "\"timestamps\":[";
for (int i = HISTORY_LENGTH - 1; i >= 0; i--) {
  json += "\"" + timestampHistory[i] + "\"";
  if (i > 0) json += ",";
}
json += "]";

  json += "}";

  request->send(200, "application/json", json);

}

void updateSensorAndDisplay() {
float massConcentrationPm1p0;
float massConcentrationPm2p5;
float massConcentrationPm4p0;
float massConcentrationPm10p0;
float ambientHumidity;
float ambientTemperature;
uint16_t co2Equivalent;

  int16_t error = sensor.readMeasuredValues(
    massConcentrationPm1p0,
    massConcentrationPm2p5,
    massConcentrationPm4p0,
    massConcentrationPm10p0,
    ambientHumidity,
    ambientTemperature,
    vocIndex,
    noxIndex,
    co2Equivalent
  );

  if (error == 0) {
    lastCo2 = (float)co2Equivalent;
    lastTemp = ambientTemperature;
    lastRh = ambientHumidity;
    lastPm2p5 = massConcentrationPm2p5;

    pm1 = massConcentrationPm1p0;
    pm2p5 = massConcentrationPm2p5;
    pm4 = massConcentrationPm4p0;
    pm10 = massConcentrationPm10p0;

    shiftHistory(lastCo2, lastTemp, lastRh);
    displayData(lastCo2, lastTemp, lastRh, lastPm2p5);

    Serial.printf("CO2: %.0f ppm, Temp: %.1f C, RH: %.1f %%, PM1: %.1f, PM2.5: %.1f, PM4: %.1f, PM10: %.1f\n",
                  lastCo2, lastTemp, lastRh, pm1, pm2p5, pm4, pm10);
  } else {
    Serial.println("Chyba cteni senzoru!");
    lastCo2 = NAN;
    lastTemp = NAN;
    lastRh = NAN;
    lastPm2p5 = NAN;
    displayData(NAN, NAN, NAN, NAN);
  }
}

void loop() {
  bool buttonPressed = false;

  if (digitalRead(BUTTON1) == LOW) {
    viewMode = 0;
    displayData(lastCo2, lastTemp, lastRh, lastPm2p5);
    while (digitalRead(BUTTON1) == LOW);
    buttonPressed = true;
  } else if (digitalRead(BUTTON2) == LOW) {
    viewMode++;
    if (viewMode > 6) viewMode = 1;
    displayData(lastCo2, lastTemp, lastRh, lastPm2p5);
    while (digitalRead(BUTTON2) == LOW);
    buttonPressed = true;
  }

  if (buttonPressed) {
    lastActivity = millis();
    if (isScreensaverOn) {
      // Vypnout spořič - jas zpět na 100 %
      analogWrite(BACKLIGHT_PIN, 255);
      isScreensaverOn = false;
    }
  }

  if (!isScreensaverOn && (millis() - lastActivity > SCREENSAVER_DELAY_MS)) {
    // Zapnout spořič - snížit jas na 30 %
    analogWrite(BACKLIGHT_PIN, BACKLIGHT_BRIGHTNESS); 
    isScreensaverOn = true;
  }

  // Aktualizace dat
  if (millis() - lastUpdate > UPDATE_INTERVAL_MS) {
    updateSensorAndDisplay();
    lastUpdate = millis();
  }
  ElegantOTA.loop();
}


void showTemperatureIcon(int x, int y) {
  if (!displayShowTemperature) return;
  tft.setSwapBytes(true);
  tft.pushImage(x, y, 16, 16, iconTemperature);
}

void showHumidityIcon(int x, int y) {
    if (!displayShowHumidity) return;
    tft.setSwapBytes(true);
    tft.pushImage(x, y, 16, 16, iconHumidity);
}

void drawValueWithIcon(int iconY, const String& text, uint16_t color,
                       void (*drawIcon)(int, int), uint8_t textDatum, int textOffset = 4) {
  tft.setTextColor(color);
  tft.setTextDatum(textDatum);

  int textWidth = tft.textWidth(text);
  int iconWidth = 16;
  int textX, iconX;

    if (textDatum == TL_DATUM) {
      iconX = 4; 
      textX = iconX + iconWidth + textOffset;  // text vpravo od ikony
    } else if (textDatum == TR_DATUM) {
      textX = 236; 
      iconX = textX - textWidth - textOffset - iconWidth;  // ikona vlevo od textu
    } else {
      // fallback - umístění na nulu
      iconX = 0;
      textX = 0;
    }

  drawIcon(iconX, iconY);
  tft.drawString(text, textX, iconY);
}


void displayData(float co2, float temp, float rh, float pm) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_MONITORGREY, TFT_BLACK);

  switch (viewMode) {
    case 0: {  // Home view
      // CO2 barva
      uint16_t co2Color = TFT_DARKGREY;
      String kvalita = "-";

      if (!isnan(co2)) {
        if (co2 <= 1000) {
          co2Color = TFT_MONITORGREEN;
          kvalita = "Dobra kvalita";
        } else if (co2 <= 2000) {
          co2Color = TFT_YELLOW;
          kvalita = "Doporuceno vetrat";
        } else if (co2 <= 3000) {
          co2Color = TFT_ORANGE;
          kvalita = "Nutne vetrat!";
        } else {
          co2Color = TFT_RED;
          kvalita = "Spatna kvalita, vetrat!";
        }
      }

      // Teplota a vlhkost
      tft.loadFont(SMALL_FONT);
      String tempText = isnan(temp) ? "-.- °C" : String(temp, 1) + " °C";
      String rhText   = isnan(rh)   ? "-.- %"  : String(rh, 1)   + " %";
      drawValueWithIcon(1, tempText, TFT_ORANGE, showTemperatureIcon, TL_DATUM);
      drawValueWithIcon(1, rhText, TFT_CYAN, showHumidityIcon, TR_DATUM);

      // CO2 hodnota + ppm jednotka
      tft.loadFont(BIG_FONT); 
      tft.setTextDatum(MC_DATUM); 
      tft.setTextColor(co2Color);
      tft.drawString(isnan(co2) ? "0000" : String((int)co2) , 120, 70);
      tft.setTextColor(TFT_DARKGREY);
      tft.loadFont(MINI_FONT);
      tft.drawString("ppm", 120, 106);

      tft.loadFont(SMALL_FONT); 
      tft.setTextColor(TFT_LIGHTGREY);
      tft.setTextDatum(MC_DATUM);
      tft.drawString(kvalita, 120, 128);
      break;
    }

    case 1:
      tft.loadFont(SMALL_FONT);
      tft.drawString("Teplota (°C)", 120, 25);    
      tft.loadFont(BIG_FONT);
      tft.setTextDatum(MC_DATUM);
      tft.setTextColor(TFT_ORANGE);
      tft.drawString(isnan(temp) ? "0.0" : String(temp, 1), 120, 95);
      break;

    case 2:
      tft.loadFont(SMALL_FONT);
      tft.drawString("Vlhkost (%)", 120, 25);      
      tft.loadFont(BIG_FONT);
      tft.setTextDatum(MC_DATUM);
      tft.setTextColor(TFT_CYAN);
      tft.drawString(isnan(rh) ? "0.0" : String(rh, 1) , 120, 95);
      break;

    case 3:
      tft.loadFont(SMALL_FONT);
      tft.drawString("PM1.0 (ug/m3)", 120, 25);          
      tft.loadFont(BIG_FONT);
      tft.setTextDatum(MC_DATUM);
      tft.setTextColor(TFT_CYAN);
      tft.drawString(isnan(pm1) ? "00" : String(pm1, 1), 120, 95);
      break;

    case 4:
      tft.loadFont(SMALL_FONT);
      tft.drawString("PM2.5 (ug/m3)", 120, 25);          
      tft.loadFont(BIG_FONT);
      tft.setTextDatum(MC_DATUM);
      tft.setTextColor(TFT_MAGENTA);
      tft.drawString(isnan(pm2p5) ? "00" : String(pm2p5, 1), 120, 95);
      break;

    case 5:
      tft.loadFont(SMALL_FONT);
      tft.drawString("PM4.0 (ug/m3)", 120, 25);          
      tft.loadFont(BIG_FONT);
      tft.setTextDatum(MC_DATUM);
      tft.setTextColor(TFT_YELLOW);
      tft.drawString(isnan(pm4) ? "00" : String(pm4, 1), 120, 95);
      break;

    case 6:
      tft.loadFont(SMALL_FONT);
      tft.drawString("PM10 (ug/m3)", 120, 25);          
      tft.loadFont(BIG_FONT);
      tft.setTextDatum(MC_DATUM);
      tft.setTextColor(TFT_RED);
      tft.drawString(isnan(pm10) ? "00" : String(pm10, 1), 120, 95);
      break;

  }
}

void handleRoot(AsyncWebServerRequest *request) {
  String html = R"rawliteral(
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="cs" xml:lang="cs">
<head>
  <meta charset="UTF-8">
  <title>Měření kvality vzduchu</title>
  <link rel="icon" type="image/x-icon" href="/favicon.ico">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link href="https://fonts.googleapis.com/css2?family=Roboto&display=swap" rel="stylesheet">
  <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
  
  <style>
    html {
    overflow-y: scroll;
    }
    body {
      font-family: 'Roboto', sans-serif;
      text-align: center;
      background: #f5f5f5;
      margin: 0;
      padding: 0 10px;
    }

    .headline {
      font-weight: 900;
      letter-spacing: 1px;
      font-size: 26px;
      color: #009688cc; 
      border: 1px solid #b2dfdb; 
      background-color: #e0f7fa; 
      padding: 12px 20px;
      display: inline-block;
      margin: 20px auto 0 auto;
      box-shadow: 2px 2px 6px rgba(0, 150, 136, 0.1); 
      border-radius: 4px;
    }


    #topMenu {
      margin: 20px 0;
      text-align: center;
      display: flex;
      justify-content: center;
      gap: 12px;
      flex-wrap: nowrap;
      overflow-x: auto;
      -webkit-overflow-scrolling: touch; 
    }
    
    #topMenu button {
      margin: 0; 
      padding: 10px 16px;
      width: 100px;
      flex-shrink: 0;
      background-color: #f5f5f5;
      border: 1px solid #ccc;
      border-radius: 4px;
      font-size: 1em;
      cursor: pointer;
      transition: background-color 0.2s ease, transform 0.1s ease;
      box-shadow: 0 2px 4px rgba(0,0,0,0.1);
      color: #333;

      display: inline-flex;
      align-items: center;
      justify-content: center;
      gap: 8px;
    }

    #topMenu button .material-icons {
      font-size: 20px;
    }

    #topMenu .btn-label {
      font-size: 1em;
      font-weight: 500;
    }

    #topMenu button:hover {
      background-color: #fff;
      //transform: translateY(-1px);
    }

    #topMenu button.active {
      background-color: #68b141;
      color: white;
      border-color: #4CAF50;
    }

    .container,
    #chart-container,
    #service-container {
      margin-top: 20px;
      max-width: 600px;
      margin-left: auto;
      margin-right: auto;
    }

    #chart-container {
      max-width: 600px;
      margin: 20px auto;
      height: auto;       
      max-height: 80vh;  
      position: relative;
      overflow: visible; 
    }

    .table-wrapper {
      width: 100%;
      overflow-x: auto;
      -webkit-overflow-scrolling: touch;
    }

    .note-text {
      color: #bbb; 
      font-size: 0.875em;
      margin-top: 10px;
    }



    .mainTable {
      margin: 2rem auto;
      border-collapse: separate;  
      border-spacing: 0;
      width: 100%;
      max-width: 600px;
      font-size: 1em;
      box-shadow: 0 6px 10px rgba(0, 0, 0, 0.15);
      border-radius: 6px;
      overflow: hidden; 
      table-layout: fixed;
      background-color: #fafdfa; 
    }

    .mainTable th, 
    .mainTable td {
      padding: 14px 16px;
      text-align: center;
      border-bottom: 1px solid #cfd8cd; 
      word-wrap: break-word;
      width: 33.33%;
      color: #2e4429; 
    }

    .mainTable th {
      background-color: #b7d4b7; 
      color: #1e341e;            
      text-transform: uppercase;
      letter-spacing: 0.07em;
      font-weight: 700;
      user-select: none;
      box-shadow: inset 0 -3px 5px -3px rgba(0, 0, 0, 0.15); 
      white-space: nowrap;  /* zabránit zalomení */
    }

    .mainTable tbody tr:nth-child(even) {
      background-color: #f7fef7; 
    }

    .mainTable tbody tr:hover {
      background-color: #e3f2e3; 
      transition: background-color 0.25s ease;
      cursor: pointer;
    }

    .mainTable td:last-child {
      text-align: center;
    }

    .mainTable td:nth-child(2) {
      color: black !important;
      font-weight: 600;
    }

    .mainTable td[data-label="Veličina"] {
      cursor: pointer;
}

    .value {
      padding: 4px 10px;
      border: 1px solid #ccc;
      border-radius: 8px;
      font-weight: 600;
      color: black;
      display: inline-block;
      min-width: 40px;
      text-align: center;
      background-color: transparent;
    }   

    .green { background-color: #e6f4e6; }
    .yellow { background-color: #fff9e6; }
    .orange { background-color: #ffd6b3; }
    .red { background-color: #ffb3b3; }
  

    .statusTable {
      width: 100%;
      max-width: 600px;
      margin: 20px auto;
      border-collapse: separate;
      border-spacing: 0;
      border-radius: 6px;
      overflow: hidden;
      box-shadow: 0 2px 6px rgba(0, 0, 0, 0.04);
      background-color: #fcfcfd; 
      border: 1px solid #e0e4ea; 
      table-layout: fixed;
    }

    .statusTable th,
    .statusTable td {
      padding: 12px 16px;
      text-align: left;
      border-bottom: 1px solid #eceff4; 
      color: #5e6b7a; 
    }

    .statusTable th {
      background-color: #f2f5fa; 
      font-weight: 600;
      color: #445566;
      text-transform: uppercase;
      letter-spacing: 0.04em;
      user-select: none;
      box-shadow: none;
    }

    .statusTable tbody tr:nth-child(even) {
      background-color: #f8f9fb; 
    }

    .statusTable tbody tr:hover {
      background-color: #f0f3f8; 
      transition: background-color 0.2s ease;
      cursor: default;
    }

    .statusTable td {
      font-weight: 400;
      color: #5a6c7a;
    }


    .infoTable {
      width: 100%;
      max-width: 600px;
      margin: 1.5rem auto;
      border-collapse: collapse;
      font-size: 0.95em;
      color: #444;
      background-color: #fafafa;
      border: 1px solid #ddd;
      border-radius: 6px;
      overflow: hidden;
      table-layout: fixed;
    }

    .infoTable th, 
    .infoTable td {
      padding: 10px 14px;
      text-align: left;
      border-bottom: 1px solid #eee;
    }

    .infoTable th {
      background-color: #f0f0f0;
      font-weight: 600;
      color: #555;
      user-select: none;
    }

    .infoTable tr:last-child td {
      border-bottom: none;
    }

    .infoTable tr:hover {
      background-color: #f9f9f9;
      transition: background-color 0.3s ease;
    }


    .soft-link {
      color: #3b82f6;
      text-decoration: none;
      font-weight: 500;
      transition: color 0.3s;
    }

    .soft-link:hover {
      color: #1e40af; 
    }

    #infoBox {
      max-width: 600px;
      width: 100%;
      margin: 20px auto 0 auto;
      padding: 0px;
      /* background: #f9f9f9; */
      font-size: 0.9em;
      border: none;
      box-sizing: border-box;
    }
    
    #alert {
      background-color: #e61919;  
      color: white;
      font-weight: 600;
      padding: 12px 20px;
      margin: 20px auto;
      width: 100%;
      max-width: 600px;
      border-radius: 4px;
      display: none;
      font-size: 1.1em;
      box-shadow: 0 3px 8px rgba(0,0,0,0.15);
      text-align: center;
      border: 1px solid #e06c10;
      font-family: 'Roboto', sans-serif;
      letter-spacing: 0.5px;
      user-select: none;
      transition: background-color 0.3s ease;
      box-sizing: border-box;
    }

    #alert strong {
      font-weight: 700;
    }

 

    /* mobilní layout */
    @media (max-width: 480px) {

    .headline {
      font-size: 24px;
      padding: 8px 10px;
      margin: 15px auto 0 auto;
      //letter-spacing: 0.8px;
      display: block; 
      width: 90%; 
    }
  
      #topMenu button {
        width: 80px;
        padding: 8px 10px;
        font-size: 0.9em;
        gap: 6px;
      }

        /* Mobilní úpravy všech tabulek */
      .table-wrapper {
        width: 90%;
        margin: 0 auto 20px auto;
        padding: 0;
        overflow-x: auto;
      }

      .mainTable,
      .statusTable,
      .infoTable {
        width: 100%;
        table-layout: auto;
        border-radius: 0;
        box-shadow: none;
        margin: 0 auto;
      }

      .mainTable th, .mainTable td,
      .statusTable th, .statusTable td,
      .infoTable th, .infoTable td {
        font-size: 0.85em;
        padding: 10px 10px;
        white-space: normal;          
        word-break: break-word;       
        text-align: center;
      }

      html, body {
        margin: 0;
        padding: 0;
        overflow-x: hidden;
      }
    }
    
  </style>
</head>
<body>

  <h1 class="headline">
    <img src="/headericon.png" alt="ikona" style="width:32px; height:32px; vertical-align:middle; margin-right:8px;">
    MONITOR KVALITY VZDUCHU
  </h1>

  <div id="topMenu">
    <button onclick="showSection('table')" id="btnTable" title="Zobrazit tabulku">
      <span class="btn-label">Hodnoty</span>
    </button>
    <button onclick="showSection('chart')" id="btnChart" title="Zobrazit grafy">
      <span class="btn-label">Grafy</span>
    </button>
    <button onclick="showSection('servis')" id="btnUpdate" title="Servis">
      <span class="btn-label">Servis</span>
    </button> 
  </div>

<div class="container">

  <div id="alert">⚠️ <strong>POZOR:</strong> Vysoká koncentrace CO₂! Nutné větrat.</div>

  <div class="table-wrapper">
    <table class="mainTable">
      <thead>
        <tr><th>Veličina</th><th>Hodnota</th><th>Jednotka</th></tr>
      </thead>
      <tbody>
        <tr id="co2">
          <td data-label="Veličina">🌿 CO₂</td>
          <td data-label="Hodnota" id="co2value"><span class="value">--</span></td>
          <td data-label="Jednotka">ppm</td>
        </tr>
        <tr id="temp">
          <td data-label="Veličina">🌡️ Teplota</td>
          <td data-label="Hodnota"><span class="value">--</span></td>
          <td data-label="Jednotka">°C</td>
        </tr>
        <tr id="rh">
          <td data-label="Veličina">💧 Vlhkost</td>
          <td data-label="Hodnota"><span class="value">--</span></td>
          <td data-label="Jednotka">%</td>
        </tr>
        <tr id="pm1">
          <td data-label="Veličina">🌫️ PM1.0</td>
          <td data-label="Hodnota" id="pm"><span class="value">--</span></td>
          <td data-label="Jednotka">µg/m³</td>
        </tr>
        <tr id="pm2p5">
          <td data-label="Veličina">💨 PM2.5</td>
          <td data-label="Hodnota"><span class="value">--</span></td>
          <td data-label="Jednotka">µg/m³</td>
        </tr>
        <tr id="pm4">
          <td data-label="Veličina">☁️ PM4.0</td>
          <td data-label="Hodnota"><span class="value">--</span></td>
          <td data-label="Jednotka">µg/m³</td>
        </tr>
        <tr id="pm10">
          <td data-label="Veličina">🌪️ PM10</td>
          <td data-label="Hodnota"><span class="value">--</span></td>
          <td data-label="Jednotka">µg/m³</td>
        </tr>
        <tr id="voc">
          <td data-label="Veličina">🧪 VOC Index</td>
          <td data-label="Hodnota" id="voc"><span class="value">--</span></td>
          <td data-label="Jednotka"></td>
        </tr>
        <tr id="nox">
          <td data-label="Veličina">🚗 NOx Index</td>
          <td data-label="Hodnota" id="nox"><span class="value">--</span></td>
          <td data-label="Jednotka"></td>
        </tr>
      </tbody>
    </table>

    <p class="note-text">Interval aktualizace )rawliteral" + String(UPDATE_INTERVAL_MIN) + R"rawliteral( minut.</p>  
    <p class="note-text">Čas spuštění: )rawliteral" + startDateTime + R"rawliteral(</p>
  </div>

</div>
  
  <div id="chart-container">
    <canvas id="co2Chart"></canvas>
    <canvas id="tempChart"></canvas>
    <canvas id="rhChart"></canvas>
  </div>  

  <div id="service-container">
  <div class="table-wrapper">  
    <table class="statusTable">
      <tr><th>Parametr</th><th>Hodnota</th></tr>
      <tr><td>IP adresa:</td><td>)rawliteral" + WiFi.localIP().toString() + R"rawliteral(</td></tr>
      <tr><td>Název (SSID):</td><td>)rawliteral" + WiFi.SSID() + R"rawliteral(</td></tr>
      <tr><td>Signál (RSSI):</td><td>)rawliteral" + String(WiFi.RSSI()) + R"rawliteral( dBm</td></tr>
      <tr><td>Verze firmwaru:</td><td>1.0.0</td></tr>
      <tr><td>Aktualizace firmwaru:</td><td><a href="/update" class="soft-link">WiFi (ElegantOTA)</a></td></tr>
    </table>
  </div>

  <br>

  <div class="table-wrapper"> 
    <table class="infoTable">
      <tr><th>Hodnota RSSI</th><th>Síla signálu WiFi</th></tr>
      <tr><td>RSSI > –30 dBm</td><td>Vynikající</td></tr>
      <tr><td>RSSI &lt; –55 dBm</td><td>Velmi dobrý</td></tr>
      <tr><td>RSSI &lt; –67 dBm</td><td>Docela dobrý</td></tr>
      <tr><td>RSSI &lt; –70 dBm</td><td>Dobrý</td></tr>
      <tr><td>RSSI &lt; –80 dBm</td><td>Špatný</td></tr>
      <tr><td>RSSI &lt; –90 dBm</td><td>Slabý (nepoužitelný)</td></tr>
    </table>
  </div>

    <p class="note-text">Pro správné zobrazení času v grafu musí být monitor připojen na internet.</p>       
  </div>   

  <div id="infoBox"></div>


  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  
  <script>

  const HISTORY_LENGTH = )rawliteral" + String(HISTORY_LENGTH) + R"rawliteral(;
  UPDATE_INTERVAL_MIN = )rawliteral" + String(UPDATE_INTERVAL_MIN) + R"rawliteral(;

  let co2Data = new Array(HISTORY_LENGTH).fill(0);
  let tempData = new Array(HISTORY_LENGTH).fill(0);
  let rhData = new Array(HISTORY_LENGTH).fill(0);
  let labels = new Array(HISTORY_LENGTH).fill(0);

  // CO₂ graf
  const co2Chart = new Chart(document.getElementById('co2Chart').getContext('2d'), {
    type: 'line',
    data: {
      labels: labels,
      datasets: [{
        label: 'CO₂ (ppm)',
        data: co2Data,
        borderColor: 'rgb(75, 192, 192)',
        fill: false,
        tension: 0.3,
        pointRadius: 2
      }]
    },
    options: {
      responsive: true,
      animation: false,
      scales: {
        x: {
          ticks: {
            maxRotation: 0,
            minRotation: 0,
            autoSkip: true,
            maxTicksLimit: 8
          }
        },
        y: {
          beginAtZero: true,
          suggestedMax: 2000
        }
      }
    }
  });

  // Teplota graf
  const tempChart = new Chart(document.getElementById('tempChart').getContext('2d'), {
    type: 'line',
    data: {
      labels: labels,
      datasets: [{
        label: 'Teplota (°C)',
        data: tempData,
        borderColor: 'rgb(255, 99, 132)',
        fill: false,
        tension: 0.3,
        pointRadius: 2
      }]
    },
    options: {
      responsive: true,
      animation: false,
      scales: {
        x: {
          ticks: {
            maxRotation: 0,
            minRotation: 0,
            autoSkip: true,
            maxTicksLimit: 8
          }
        },
        y: {
          beginAtZero: true,
          suggestedMax: 50
        }
      }
    }
  });

  // Vlhkost graf
  const rhChart = new Chart(document.getElementById('rhChart').getContext('2d'), {
    type: 'line',
    data: {
      labels: labels,
      datasets: [{
        label: 'Relativní vlhkost (%)',
        data: rhData,
        borderColor: 'rgb(54, 162, 235)',
        fill: false,
        tension: 0.3,
        pointRadius: 2
      }]
    },
    options: {
      responsive: true,
      animation: false,
      scales: {
        x: {
          ticks: {
            maxRotation: 0,
            minRotation: 0,
            autoSkip: true,
            maxTicksLimit: 8
          }
        },
        y: {
          beginAtZero: true,
          suggestedMax: 100
        }
      }
    }
  });
  </script>
 
  <script>

    function setInfoBoxWidth() {
      const table = document.querySelector('.mainTable');
      if (table && infoBox) {
        infoBox.style.width = table.offsetWidth + 'px';
      }
    }

    window.addEventListener('resize', setInfoBoxWidth);
    setInfoBoxWidth();  // inicializace při načtení

    // Barvení CO2, PM podle hranic
    // Funkce pro nastavení barvy buňky podle pravidel
    function colorCell(rowId, value) {
      const row = document.getElementById(rowId);
      if (!row) return;
      const cell = row.querySelector('td[data-label="Hodnota"]');
      if (!cell) return;

      // Najdi element uvnitř, který má třídu 'value'
      const valueSpan = cell.querySelector('.value');
      if (!valueSpan) return;

      // Reset tříd na valueSpan
      valueSpan.className = 'value';

      if (rowId === 'co2') {
        if (value < 1000) valueSpan.classList.add('green');
        else if (value < 2000) valueSpan.classList.add('yellow');
        else if (value < 3000) valueSpan.classList.add('orange');
        else valueSpan.classList.add('red');
      } else if (rowId === 'temp') {
        if (value >= 30) valueSpan.classList.add('red');
      } else if (rowId === 'rh') {
        if (value > 70) valueSpan.classList.add('blue');
      } else if (['pm1', 'pm2p5', 'pm4', 'pm10'].includes(rowId)) {
        if (value < 100) valueSpan.classList.add('green');
        else if (value < 200) valueSpan.classList.add('yellow');
        else if (value < 300) valueSpan.classList.add('orange');
        else valueSpan.classList.add('red');
      }
    }


    // Funkce pro barvu VOC indexu
    function colorVocCell(value) {
      if (isNaN(value)) return;

      const vocCell = document.querySelector('#voc[data-label="Hodnota"]');
      if (!vocCell) return;

      const valueSpan = vocCell.querySelector('.value');
      if (!valueSpan) return;

      valueSpan.className = 'value';

      if (value <= 50) valueSpan.classList.add('green');
      else if (value <= 100) valueSpan.classList.add('yellow');
      else if (value <= 200) valueSpan.classList.add('orange');
      else valueSpan.classList.add('red');
    }

    // Funkce pro barvu NOx indexu
    function colorNoxCell(value) {
      if (isNaN(value)) return;

      const noxCell = document.querySelector('#nox[data-label="Hodnota"]');
      if (!noxCell) return;

      const valueSpan = noxCell.querySelector('.value');
      if (!valueSpan) return;

      valueSpan.className = 'value';

      if (value <= 1) valueSpan.classList.add('green');
      else if (value <= 3) valueSpan.classList.add('yellow');
      else if (value <= 10) valueSpan.classList.add('orange');
      else valueSpan.classList.add('red');
    }


    // Načti všechny hodnoty z tabulky a aplikuj barvy
    function applyColors() {
      const rows = ['co2','temp','rh','pm1','pm2p5','pm4','pm10'];
      rows.forEach(id => {
        const row = document.getElementById(id);
        if (row) {
          const val = parseFloat(row.cells[1].innerText);
          colorCell(id, val);
        }
      });
      
      // VOC index do barvení
      const vocCell = document.getElementById('voc');
      if (vocCell) {
        const vocValue = parseFloat(vocCell.innerText);
        if (!isNaN(vocValue)) {
          colorVocCell(vocValue);
        }
      }

      // NOx index do barvení
      const noxCell = document.getElementById('nox');
      if (noxCell) {
        const noxValue = parseFloat(noxCell.innerText);
        if (!isNaN(noxValue)) {
          colorNoxCell(noxValue);
        }
      }
    }

    applyColors();

    // Zobraz alert hned při načtení, pokud je CO₂ > 1500
    const initialCo2 = parseFloat(document.getElementById('co2value').innerText);
    if (!isNaN(initialCo2) && initialCo2 > 2000) {
      document.getElementById("alert").style.display = "block";
    }

    function updateRow(id, value, thresholds) {
      const row = document.getElementById(id);
      if (!row) return;
      const cell = row.cells[1];
      const valueSpan = cell.querySelector('.value');
      if (!valueSpan) return;
      valueSpan.innerText = value.toFixed(1);
      colorCell(id, value);
    }

    const UPDATE_INTERVAL_MIN = )rawliteral" + String(UPDATE_INTERVAL_MIN) + R"rawliteral(;

    async function fetchData() {
    //console.log("fetchData called");
    const res = await fetch("/data");
    //console.log("Response ok?", res.ok, "status:", res.status);
    const data = await res.json();
    //console.log("Data ⇢", data);

    updateRow("co2", data.co2, [1000, 2000, 3000]);
    updateRow("temp", data.temp, [22, 26, 30]);
    updateRow("rh", data.rh, [30, 60, 80]);

    updateRow("pm1", data.pm1, [10, 25, 50]);
    updateRow("pm2p5", data.pm2p5, [10, 25, 50]);
    updateRow("pm4", data.pm4, [10, 25, 50]);
    updateRow("pm10", data.pm10, [10, 25, 50]);
    updateRow("voc", data.vocIndex, [1, 3, 5]);
    updateRow("nox", data.noxIndex, [1, 3, 5]);

    const alertDiv = document.getElementById("alert");
    alertDiv.style.display = data.co2 > 2000 ? "block" : "none";

    // Aktuální hodnoty do datových polí
    co2Data.push(data.co2);
    tempData.push(data.temp);
    rhData.push(data.rh);

    if (co2Data.length > HISTORY_LENGTH) co2Data.shift();
    if (tempData.length > HISTORY_LENGTH) tempData.shift();
    if (rhData.length > HISTORY_LENGTH) rhData.shift();

    // Zkopíruj správně časová razítka
    labels = data.timestamps.slice();

    // Použij historii ze serveru
    co2Chart.data.labels = labels;
    co2Chart.data.datasets[0].data = data.co2History;
    co2Chart.update();

    tempChart.data.labels = labels;
    tempChart.data.datasets[0].data = data.tempHistory;
    tempChart.update();

    rhChart.data.labels = labels;
    rhChart.data.datasets[0].data = data.rhHistory;
    rhChart.update(); 
    }

    setInterval(fetchData, UPDATE_INTERVAL_MIN);
    fetchData();


  </script>

  <script>    
    
    const co2Text = `
    <strong>Co je CO₂ (oxid uhličitý)?</strong><br><br>
    CO₂ je bezbarvý plyn přirozeně se vyskytující v atmosféře, který vzniká:<br>
    – dýcháním lidí a zvířat,<br>
    – spalováním fosilních paliv,<br>
    – fermentací a dalšími biologickými procesy.<br><br>

    Venkovní vzduch (čistý, přírodní prostředí):
    ~400 až 420 ppm (parts per million) (běžná koncentrace CO₂ na venkově či v přírodě – rok od roku mírně roste vlivem emisí).<br><br>
    
    Ve vnitřních prostorách jeho koncentrace často stoupá kvůli nedostatečnému větrání.<br><br>

    <strong>Proč CO₂ sledovat?</strong><br>
    Vysoké hodnoty CO₂ nesignalizují přímé zdravotní riziko, ale:<br>
    – mohou vést k únavě, snížené soustředěnosti a ospalosti,<br>
    – indikují nedostatečné větrání a akumulaci vydýchaného vzduchu.<br><br>

    <strong>Doporučené hodnoty CO₂</strong><br><br>
    <table style="width:100%; border-collapse: collapse; font-size: 0.85em;">
      <tr style="background: #333; color: white;">
        <th style="padding:4px; border:1px solid #ddd;">Koncentrace</th>
        <th style="padding:4px; border:1px solid #ddd;">Kvalita vzduchu</th>
        <th style="padding:4px; border:1px solid #ddd;">Doporučení</th>
      </tr>
      <tr><td style="padding:4px; border:1px solid #ddd;">0 ÷ 1000 ppm</td><td style="padding:4px; border:1px solid #ddd;">Dobrá</td><td style="padding:4px; border:1px solid #ddd;">Optimální větrání</td></tr>
      <tr><td style="padding:4px; border:1px solid #ddd;">1001 ÷ 2000 ppm</td><td style="padding:4px; border:1px solid #ddd;">Zvýšená</td><td style="padding:4px; border:1px solid #ddd;">Doporučeno větrat</td></tr>
      <tr><td style="padding:4px; border:1px solid #ddd;">2001 ÷ 3000 ppm</td><td style="padding:4px; border:1px solid #ddd;">Špatná</td><td style="padding:4px; border:1px solid #ddd;">Nutné větrat</td></tr>
      <tr><td style="padding:4px; border:1px solid #ddd;">3000+ ppm</td><td style="padding:4px; border:1px solid #ddd;">Velmi špatná</td><td style="padding:4px; border:1px solid #ddd;">Zdravotní diskomfort</td></tr>
    </table><br><br>
    `;

    const pmText = `
    <strong>Co jsou částice PM?</strong><br>
    Částice PM (Particulate Matter) jsou mikroskopické pevné částice nebo kapky kapaliny, které se nacházejí ve vzduchu.<br><br>

    Nejčastěji měřené velikosti jsou:
    <ul style="margin-top: 8px; margin-bottom: 8px;">
      <li><strong>PM1.0</strong> – velmi jemné částice (do 1 mikrometru), mohou pronikat hluboko do plic.</li>
      <li><strong>PM2.5</strong> – jemné částice (do 2.5 mikrometru), spojené se znečištěním z dopravy, spalování a průmyslu.</li>
      <li><strong>PM4.0</strong> – střední frakce, méně běžně používaná.</li>
      <li><strong>PM10</strong> – hrubší prach (do 10 mikrometrů), zachycuje se v horních cestách dýchacích.</li>
    </ul>

    <table style="width: 100%; border-collapse: collapse; font-size: 0.95em; margin-top: 12px;">
      <tr style="background: #333; color: white;">
        <th style="padding: 6px; border: 1px solid #ddd;">Označení</th>
        <th style="padding: 6px; border: 1px solid #ddd;">Velikost částic</th>
        <th style="padding: 6px; border: 1px solid #ddd;">Příklad / Zdroj</th>
      </tr>
      <tr>
        <td style="padding: 6px; border: 1px solid #ddd;">PM1.0</td>
        <td style="padding: 6px; border: 1px solid #ddd;">≤ 1 µm</td>
        <td style="padding: 6px; border: 1px solid #ddd;">Kouř, spaliny, ultrajemné částice</td>
      </tr>
      <tr>
        <td style="padding: 6px; border: 1px solid #ddd;">PM2.5</td>
        <td style="padding: 6px; border: 1px solid #ddd;">≤ 2,5 µm</td>
        <td style="padding: 6px; border: 1px solid #ddd;">Výfukové plyny, spalování dřeva, prach</td>
      </tr>
      <tr>
        <td style="padding: 6px; border: 1px solid #ddd;">PM4.0</td>
        <td style="padding: 6px; border: 1px solid #ddd;">≤ 4 µm</td>
        <td style="padding: 6px; border: 1px solid #ddd;">Bakterie, některé pylové částice, průmyslové aerosoly</td>
      </tr>         
      <tr>
        <td style="padding: 6px; border: 1px solid #ddd;">PM10</td>
        <td style="padding: 6px; border: 1px solid #ddd;">≤ 10 µm</td>
        <td style="padding: 6px; border: 1px solid #ddd;">Pyl, prach, stavební materiály</td>
      </tr>
    </table><br>
  
    <strong>Proč jsou PM částice důležité?</strong><br>
    Dlouhodobé vystavení zvýšeným koncentracím PM může způsobovat dýchací potíže, kardiovaskulární problémy a další zdravotní komplikace.<br><br>

    <em>Čím menší částice, tím hlouběji mohou proniknout do lidského těla.</em><br><br>
    `;
      
    const vocText = `
    <strong>Co je VOC index?</strong><br>
    VOC index (Volatile Organic Compounds index) je číselný ukazatel kvality vzduchu založený na koncentraci těkavých organických sloučenin (VOC) v ovzduší.<br><br>

    VOC jsou různé organické chemikálie, které se snadno odpařují a mohou mít vliv na zdraví a komfort.<br><br>

    VOC index slouží jako indikátor kvality vnitřního vzduchu, protože vysoké hodnoty VOC mohou naznačovat znečištění nebo nepříjemné pachy.<br><br>

    <strong>Jak VOC index funguje?</strong><br>
    Senzory měří koncentraci různých VOC v ovzduší.<br>
    Tyto měření se zpracují algoritmem, který z hodnot vypočítá VOC index.<br><br>

    Hodnota VOC indexu obvykle spadá do určité škály (např. 0–500) a lze ji interpretovat takto:<br><br>

    <table style="width:100%; border-collapse: collapse; font-size: 0.85em;">
      <tr style="background: #333; color: white;">
        <th style="padding:4px; border:1px solid #ddd;">VOC index</th>
        <th style="padding:4px; border:1px solid #ddd;">Kvalita vzduchu</th>
        <th style="padding:4px; border:1px solid #ddd;">Význam</th>
      </tr>
      <tr><td style="padding:4px; border:1px solid #ddd;">0 ÷ 50</td><td style="padding:4px; border:1px solid #ddd;">Dobrá</td><td style="padding:4px; border:1px solid #ddd;">Nízká koncentrace VOC, zdravý vzduch</td></tr>
      <tr><td style="padding:4px; border:1px solid #ddd;">51 ÷ 100</td><td style="padding:4px; border:1px solid #ddd;">Přijatelná</td><td style="padding:4px; border:1px solid #ddd;">Mírně zvýšená koncentrace VOC</td></tr>
      <tr><td style="padding:4px; border:1px solid #ddd;">101 ÷ 200</td><td style="padding:4px; border:1px solid #ddd;">Střední</td><td style="padding:4px; border:1px solid #ddd;">Zvýšená koncentrace, může způsobit nepříjemnosti</td></tr>
      <tr><td style="padding:4px; border:1px solid #ddd;">201 ÷ 300</td><td style="padding:4px; border:1px solid #ddd;">Špatná</td><td style="padding:4px; border:1px solid #ddd;">Vysoká koncentrace, doporučuje se větrat</td></tr>
      <tr><td style="padding:4px; border:1px solid #ddd;">301+</td><td style="padding:4px; border:1px solid #ddd;">Velmi špatná</td><td style="padding:4px; border:1px solid #ddd;">Velmi vysoká koncentrace, může být škodlivá</td></tr>
    </table><br><br>
    `;

    const noxText = `
    <strong>Co je NOx index?</strong><br>
    NOx index (oxidů dusíku) je číselný ukazatel kvality vzduchu založený na koncentraci oxidů dusíku (NO a NO₂) v ovzduší.<br><br>

    Oxidy dusíku vznikají hlavně spalováním fosilních paliv a mohou dráždit dýchací cesty a zhoršovat kvalitu ovzduší.<br><br>

    <strong>Jak NOx index funguje?</strong><br>
    Senzory měří koncentraci NO a NO₂ a podle hodnot vypočítají NOx index.<br><br>

    <strong>Hodnoty NOx indexu a jejich význam:</strong><br><br>

<table style="width:100%; border-collapse: collapse; font-size: 0.85em;">
  <tr style="background: #333; color: white;">
    <th style="padding:4px; border:1px solid #ddd;">NOx index</th>
    <th style="padding:4px; border:1px solid #ddd;">Kvalita vzduchu</th>
    <th style="padding:4px; border:1px solid #ddd;">Význam</th>
  </tr>
  <tr>
    <td style="padding:4px; border:1px solid #ddd;">0 ÷ 1</td>
    <td style="padding:4px; border:1px solid #ddd;">Výborná</td>
    <td style="padding:4px; border:1px solid #ddd;">Minimální oxidy dusíku</td>
  </tr>
  <tr>
    <td style="padding:4px; border:1px solid #ddd;">2 ÷ 50</td>
    <td style="padding:4px; border:1px solid #ddd;">Dobrá</td>
    <td style="padding:4px; border:1px solid #ddd;">Nízká koncentrace NOx</td>
  </tr>
  <tr>
    <td style="padding:4px; border:1px solid #ddd;">51 ÷ 100</td>
    <td style="padding:4px; border:1px solid #ddd;">Přijatelná</td>
    <td style="padding:4px; border:1px solid #ddd;">Mírně zvýšená koncentrace</td>
  </tr>
  <tr>
    <td style="padding:4px; border:1px solid #ddd;">101 ÷ 200</td>
    <td style="padding:4px; border:1px solid #ddd;">Střední</td>
    <td style="padding:4px; border:1px solid #ddd;">Zvýšená koncentrace, možné zdravotní obtíže</td>
  </tr>
  <tr>
    <td style="padding:4px; border:1px solid #ddd;">201 ÷ 300</td>
    <td style="padding:4px; border:1px solid #ddd;">Špatná</td>
    <td style="padding:4px; border:1px solid #ddd;">Vysoká koncentrace – větrat doporučeno</td>
  </tr>
  <tr>
    <td style="padding:4px; border:1px solid #ddd;">301 ÷ 500</td>
    <td style="padding:4px; border:1px solid #ddd;">Velmi špatná</td>
    <td style="padding:4px; border:1px solid #ddd;">Velmi vysoká koncentrace – škodlivé</td>
  </tr>
</table>
<br><br>
    `;

    const table = document.querySelector('.mainTable');
    const infoBox = document.getElementById('infoBox');
    infoBox.style.width = table.offsetWidth + 'px';

    // Přehled textů pro jednotlivé veličiny
    const tooltipTexts = {
      co2: co2Text,
      pm1: pmText,
      pm2p5: pmText,
      pm4: pmText,
      pm10: pmText,
      voc: vocText, 
      nox: noxText
    };

    let currentlyVisible = null; // sleduje, který text je právě zobrazený

    function addTooltipListener(rowId, text) {
      const row = document.getElementById(rowId);
      if (!row) return;
      const labelCell = row.querySelector('td[data-label="Veličina"]');
      if (!labelCell) return;

      labelCell.addEventListener('click', () => {
        if (currentlyVisible === rowId) {
          infoBox.innerHTML = '';
          currentlyVisible = null;
        } else {
          infoBox.innerHTML = text;
          currentlyVisible = rowId;
        }
      });
    }

    // Aplikuj na všechny relevantní veličiny
    Object.entries(tooltipTexts).forEach(([id, text]) => {
      addTooltipListener(id, text);
    });

    // Kliknutí mimo tabulku schová infoBox
    document.addEventListener('click', (event) => {
      // Pokud klik není uvnitř .mainTable nebo infoBox
      if (!table.contains(event.target) && !infoBox.contains(event.target)) {
        infoBox.innerHTML = '';
        currentlyVisible = null;
      }
    });


    function showSection(section) {
      const sections = ['table', 'chart', 'servis'];
      sections.forEach(s => {
        const el = s === 'table' ? document.querySelector('.container') 
                  : s === 'chart' ? document.getElementById('chart-container')
                  : document.getElementById('service-container');
        if(el) el.style.display = (s === section) ? 'block' : 'none';
      });

      // aktivní tlačítko
      sections.forEach(s => {
        const btn = document.getElementById('btn' + (s === 'table' ? 'Table' : s === 'chart' ? 'Chart' : 'Update'));
        if(btn) btn.classList.toggle('active', s === section);
      });
    }

    // Po načtení stránky zobrazíme tabulku
    window.onload = () => showSection('table');

    // Nastavit výchozí zobrazení
    showSection('table');

  </script>

</body>
</html>
  )rawliteral";
  request->send(200, "text/html", html);
}
