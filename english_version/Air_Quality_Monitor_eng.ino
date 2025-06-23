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

//ENGLISH

#include "settings.h"
#include "bootlogo.h"
#include "html.h"
#include <Wire.h>
#include <SensirionI2cSen66.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <time.h>
#include "esp_sntp.h"
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
#define BACKLIGHT_PIN 4 
#define PWM_CHANNEL 0
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8

#define TFT_MONITORGREY  0xCE79   /* 204, 204, 204 */ 
#define TFT_MONITORGREEN  0x4D02   /*  73, 160,  16 */

#define MINI_FONT FontNotoSansBold15pt_mp
#define SMALL_FONT FontNotoSansBold20
#define BIG_FONT FontNotoSansBold90ptDigits

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

unsigned long lastActivity = 0; 
bool isScreensaverOn = false;

bool waitForTime(unsigned long timeout_ms = 10000) {
  struct tm timeinfo;
  unsigned long start = millis();
  while (millis() - start < timeout_ms) {
    if (getLocalTime(&timeinfo)) {
      Serial.println("Time synchronized:");
      Serial.println(&timeinfo, "%d.%m.%Y %H:%M:%S");
      return true;
    }
    delay(500);
  }
  Serial.println("NTP synchronization failed");
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
  tft.drawString("Connecting to WiFi", 120, 65);

  WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS); 

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
  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());

  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);  
  tft.setTextColor(TFT_MONITORGREY);
  tft.drawString("WiFi connected", 120, 50); 
  tft.drawString("IP: " + WiFi.localIP().toString(), 120, 80);
  delay(3000);

  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);  
  tft.setTextColor(TFT_MONITORGREY);
  tft.drawString("Time synchronization...", 120, 65); 
  delay(2000);

  configTzTime(timeZone, ntpServer1, ntpServer2);

  if (!waitForTime(15000)) {  // čekat max 15 sekund na NTP
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_RED);
    tft.drawString("NTP sync failed", 120, 65);
    Serial.println("NTP synchronization failed");
    delay(3000);
    
  } else {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_MONITORGREEN);
    tft.drawString("NTP OK", 120, 65);
    Serial.println("NTP OK");   
    delay(1500);
  }

  startDateTime = getDateTimeString();  

  ElegantOTA.begin(&server);   

  server.on("/", HTTP_GET, handleRoot);
  server.on("/now", HTTP_GET, handleNow);
  server.on("/data", HTTP_GET, handleData);

  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "image/x-icon", favicon_ico, favicon_ico_len);
  });

  server.on("/headericon.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "image/png", header_ico, header_ico_len);
  });

  server.begin();

  sensor.startContinuousMeasurement();

  tft.fillScreen(TFT_BLACK);
  tft.loadFont(SMALL_FONT);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_YELLOW);
  tft.drawString("Sensor warming up...", 120, 50);
  Serial.println("Sensor warming up...");

  for (int i = 60; i > 0; i--) {
    tft.fillRect(30, 70, 180, 30, TFT_BLACK);  
    String countdownText = "Remaining: " + String(i) + " s";
    tft.setTextColor(TFT_YELLOW); 
    tft.drawString(countdownText, 120, 80);
    delay(1000);
  }

  Serial.println("The sensor has warmed up.");

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


String getTimeString() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "??:??";
  }
  char timeStr[6];
  sprintf(timeStr, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
  return String(timeStr);
}

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

void handleNow(AsyncWebServerRequest *request) {
  updateSensorAndDisplay();  

  String json = "{";
  json += "\"co2\":" + String(lastCo2, 1) + ",";
  json += "\"temp\":" + String(lastTemp, 1) + ",";
  json += "\"rh\":" + String(lastRh, 1) + ",";
  json += "\"pm1\":" + String(pm1, 1) + ",";
  json += "\"pm2p5\":" + String(pm2p5, 1) + ",";
  json += "\"pm4\":" + String(pm4, 1) + ",";
  json += "\"pm10\":" + String(pm10, 1) + ",";
  json += "\"noxIndex\":" + String(noxIndex, 1) + ",";
  json += "\"vocIndex\":" + String(vocIndex, 1);
  json += "}";

  request->send(200, "application/json", json);
}

void handleData(AsyncWebServerRequest *request) {
  String json = "{";

  json += "\"co2\":" + String(co2History[0], 1) + ",";
  json += "\"temp\":" + String(tempHistory[0], 1) + ",";
  json += "\"rh\":" + String(rhHistory[0], 1) + ",";
  json += "\"pm1\":" + String(pm1, 1) + ",";
  json += "\"pm2p5\":" + String(pm2p5, 1) + ",";
  json += "\"pm4\":" + String(pm4, 1) + ",";
  json += "\"pm10\":" + String(pm10, 1) + ",";
  json += "\"noxIndex\":" + String(noxIndex, 1) + ",";
  json += "\"vocIndex\":" + String(vocIndex, 1) + ",";


json += "\"co2History\":[";
for (int i = HISTORY_LENGTH - 1; i >= 0; i--) {
  json += String(co2History[i], 1);
  if (i > 0) json += ",";
}
json += "],";

json += "\"tempHistory\":[";
for (int i = HISTORY_LENGTH - 1; i >= 0; i--) {
  json += String(tempHistory[i], 1);
  if (i > 0) json += ",";
}
json += "],";

json += "\"rhHistory\":[";
for (int i = HISTORY_LENGTH - 1; i >= 0; i--) {
  json += String(rhHistory[i], 1);
  if (i > 0) json += ",";
}
json += "],"; 

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
    Serial.println("Sensor reading error!");
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
      analogWrite(BACKLIGHT_PIN, 255);
      isScreensaverOn = false;
    }
  }

  if (!isScreensaverOn && (millis() - lastActivity > SCREENSAVER_DELAY_MS)) {
    analogWrite(BACKLIGHT_PIN, BACKLIGHT_BRIGHTNESS); 
    isScreensaverOn = true;
  }

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

int8_t getWifiQuality() {
  int32_t dbm = WiFi.RSSI();
  if (dbm <= -100) return 0;
  if (dbm >= -50) return 100;
  return 2 * (dbm + 100);
}

void drawWifiQuality() {
  int quality = getWifiQuality();

  int barWidth = 4;
  int spacing = 2;
  int maxBarHeight = 16;
  int totalWidth = barWidth * 4 + spacing * 3;
  
  int baseX = (240 / 2) - (totalWidth / 2); 
  int baseY = 18;                            

  tft.fillRect(baseX, baseY - maxBarHeight, totalWidth, maxBarHeight, TFT_BLACK);

  int barHeights[4];
  for (int i = 0; i < 4; i++) {
    barHeights[i] = maxBarHeight * (i + 1) / 4;
  }

  float qualityRatio = quality / 100.0 * 4;

  for (int i = 0; i < 4; i++) {
    if (qualityRatio >= i + 1) {
      tft.fillRect(baseX + i * (barWidth + spacing), baseY - barHeights[i], barWidth, barHeights[i], TFT_LIGHTGREY);
    } else if (qualityRatio > i) {
      int partialHeight = (qualityRatio - i) * barHeights[i];
      tft.fillRect(baseX + i * (barWidth + spacing), baseY - partialHeight, barWidth, partialHeight, TFT_LIGHTGREY);
    }
  }
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
      textX = iconX + iconWidth + textOffset; 
    } else if (textDatum == TR_DATUM) {
      textX = 236; 
      iconX = textX - textWidth - textOffset - iconWidth; 
    } else {
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
      uint16_t co2Color = TFT_DARKGREY;
      String kvalita = "-";

      if (!isnan(co2)) {
        if (co2 <= 1000) {
          co2Color = TFT_MONITORGREEN;
          kvalita = "Good air quality";
        } else if (co2 <= 2000) {
          co2Color = TFT_YELLOW;
          kvalita = "Recommend ventilation";
        } else if (co2 <= 3000) {
          co2Color = TFT_ORANGE;
          kvalita = "Ventilate now!";
        } else {
          co2Color = TFT_RED;
          kvalita = "Poor air, ventilate!";
        }
      }

      tft.loadFont(SMALL_FONT);
      String tempText = isnan(temp) ? "-.- °C" : String(temp, 1) + " °C";
      String rhText   = isnan(rh)   ? "-.- %"  : String(rh, 1)   + " %";
      drawValueWithIcon(1, tempText, TFT_ORANGE, showTemperatureIcon, TL_DATUM);
      drawValueWithIcon(1, rhText, TFT_CYAN, showHumidityIcon, TR_DATUM);

      tft.loadFont(BIG_FONT); 
      tft.setTextDatum(MC_DATUM); 
      tft.setTextColor(co2Color);
      tft.drawString(isnan(co2) ? "0000" : String((int)co2) , 120, 70);
      tft.setTextColor(TFT_DARKGREY);
      tft.loadFont(MINI_FONT);
      tft.drawString("ppm", 120, 104);

      tft.loadFont(SMALL_FONT); 
      tft.setTextColor(TFT_LIGHTGREY);
      tft.setTextDatum(MC_DATUM);
      tft.drawString(kvalita, 120, 126);

      drawWifiQuality();
      break;
    }

    case 1:
      tft.loadFont(SMALL_FONT);
      tft.drawString("Temperature (°C)", 120, 25);    
      tft.loadFont(BIG_FONT);
      tft.setTextDatum(MC_DATUM);
      tft.setTextColor(TFT_ORANGE);
      tft.drawString(isnan(temp) ? "0.0" : String(temp, 1), 120, 95);
      break;

    case 2:
      tft.loadFont(SMALL_FONT);
      tft.drawString("Humidity (%)", 120, 25);      
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
  String html = MAIN_page;

  html.replace("{{UPDATE_INTERVAL_MIN}}", String(UPDATE_INTERVAL_MIN));
  html.replace("{{HISTORY_LENGTH}}", String(HISTORY_LENGTH));
  html.replace("{{START_DATETIME}}", startDateTime);
  html.replace("{{IP_ADDRESS}}", WiFi.localIP().toString());
  html.replace("{{SSID}}", WiFi.SSID());
  html.replace("{{RSSI}}", String(WiFi.RSSI()));

  request->send(200, "text/html", html);
}
