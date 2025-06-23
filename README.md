# **Monitor kvality vzduchu**
Monitor byl navržen speciálně pro senzor Sensirion SEN66-SIN-T s externím napájením přes USB-C.

[![Ukázka ovládání a webserveru](https://img.youtube.com/vi/wReb6p8iHjs/0.jpg)](https://www.youtube.com/watch?v=wReb6p8iHjs)



## **Hardware:**

Sensirion SEN66-SIN-T: https://www.laskakit.cz/en/senserion-sen66-sin-t-senzor-kvality-ovzdusi/

LilyGO TTGO T-Display: https://www.laskakit.cz/en/lilygo-ttgo-t-displej-esp32-1-14-tft-wifi-modul/

Kabel s konektorem JST GHT-06V-S: https://www.laskakit.cz/en/laskakit-airboard-propojovaci-kabel-pro-senserion-sen6x-senzor-kvality-ovzdusi/

USB-C kabel pro napájení a nahrání firmwaru

Krabička (3D tisk) 

https://www.printables.com/model/1334579-monitor-kvality-vzduchu-air-quality-monitor

https://makerworld.com/en/models/1540218-air-quality-monitor


## **Zapojení:**

![image](https://github.com/user-attachments/assets/97987479-7970-4fbd-a6ae-965af0acefc4)

## **Software a funkce**

Výchozí nastavení v **settings.h**

- WIFI_SSID = ""; – název wifi
- WIFI_PASSWORD = "";  – heslo na wifi

- IPAddress local_IP(xxx,xxx,xxx,xxx); – IP adresa nonitoru v síti
- IPAddress gateway(xxx,xxx,xxx,xxx); – výchozí brána
- IPAddress subnet(255,255,255,0); – maska sítě

- UPDATE_INTERVAL_MS = 15 * 60 * 1000;  – interval měření dat v milisekundách (15 minut)
- HISTORY_LENGTH = 48; – počet vzorků dat v historii grafu za určitý čas (12 hodin)
- SCREENSAVER_DELAY_MS = 30000; – čas spořiče displeje v milisekundách (30 sekund)
- BACKLIGHT_BRIGHTNESS = BRIGHTNESS_LOW; – jas spořiče displeje (25 %)


### **Displej**

Měření a zobrazení – CO<sub>2</sub>, Teplota, Relativní vlhkost, pevné částice prachu (PM)

Hlavní obrazovka:

- Teplota (°C)
- Relativní vlhkost (%)
- CO<sub>2</sub> (ppm) 
- vyhodnocení kvality vzduchu
- síla WiFi signálu (4 sloupce)

Barvy textu dle nameřené hodnoty ppm CO<sub>2</sub>

![image](https://github.com/user-attachments/assets/05cfe83a-9df1-44b4-934e-d6a4b8b0d85d)


Po nečinnosti delší než je výchozí nastavený čas (30 sekund) se automaticky spustí spořič displeje - sníží jas na nastavenou hodnotu (25 %).

**Tlačítko S1** – vedlejší obrazovka , zobrazení a přepínání idividuálních hodnot (smyčka):

1.	Teplota
2.	Relativní vlhkost
3.	PM1.0
4.	PM2.5
5.	PM4.0
6.	PM10

**Tlačítko S2** – návrat na hlavní obrazovku

### **Webserver**
![webserver_1 github](https://github.com/user-attachments/assets/74aa0455-97f6-462d-b3ab-d2b9fba40bcc)


Pro zobrazení stránky webserveru monitoru zadej do prohlížeče IP adresu monitoru, která se zobrazí na displeji po přihlášení monitoru do sítě. Stránka je responzivní, přizpůsobí se mobilním zařízením.

>Příklad: http://192.168.1.1

Stránka webserveru zobrazuje v tabulce všechny hodnoty měřené snímačem Sensirion SEN66-SIN-T:

- CO<sub>2</sub>
- Teplota
- Relativní vlhkost
- pevné částice prachu (PM1.0, PM2.5 PM4.0, PM10)
- VOC index
- NOx index


Tlačítky nad tabulkou lze přepínat mezi tabulkou hodnot, grafy a servisem. Graf zobrazuje data za posledních 12 hodin u hodnot CO<sub>2</sub>, Teploty a Relativní vlhkosti.


Kliknutím v tabulce na název veličiny (mimo teplotu a vlhkost) se zobrazí podrobnosti o veličině pod tabulkou. Opětovným kliknutím nebo klikem mimo tabulku se text schová.


> [!NOTE]
Firmware lze upgradovat pomocí wifi (**OTA firmware**) v sekci **Servis**

> [!NOTE]
Aktuální měřené hodnoty lze zobrazit i ve formátu **JSON**

>{"co2":562.0,"temp":24.5,"rh":32.0,"pm1":3.0,"pm2p5":4.0,"pm4":4.6,"pm10":5.0,"noxIndex":1.0,"vocIndex":56.0}

## **Poděkování**
Fonty a ikony převzaty z tohoto projektu https://github.com/melkati/CO2-Gadget

## **Licence**
Firmware pro monitor kvality vzduchu je svobodný software: můžete jej dále šířit a/nebo upravovat.




# **Air quality monitor**
The monitor was designed specifically for the Sensirion SEN66-SIN-T sensor with external power supply via USB-C.

## **Hardware:**

Sensirion SEN66-SIN-T: https://sensirion.com/products/catalog/SEN66

LilyGO TTGO T-Display: https://lilygo.cc/products/lilygo%C2%AE-ttgo-t-display-1-14-inch-lcd-esp32-control-board

Cable with JST connector GHT-06V-S: https://www.aliexpress.com/item/1005007295303376.html

USB-C cable for power and firmware upload

Box (3D printed) 

https://www.printables.com/model/1334579-monitor-kvality-vzduchu-air-quality-monitor

https://makerworld.com/en/models/1540218-air-quality-monitor

## **Wiring:**

![image](https://github.com/user-attachments/assets/97987479-7970-4fbd-a6ae-965af0acefc4)


## **Software and Features**

Default settings in **settings.h**

- WIFI_SSID = ""; – wifi name
- WIFI_PASSWORD = ""; – wifi password

- IPAddress local_IP(xxx,xxx,xxx,xxx); – IP address of the monitor on the network
- IPAddress gateway(xxx,xxx,xxx,xxx); – default gateway
- IPAddress subnet(255,255,255,0); – network mask
 
- UPDATE_INTERVAL_MS = 15 * 60 * 1000; – data measurement interval in milliseconds (15 minutes)
- HISTORY_LENGTH = 48; – number of data samples in the graph history for a certain time (12 hours)
- SCREENSAVER_DELAY_MS = 30000; – screen saver time in milliseconds (30 seconds)
- BACKLIGHT_BRIGHTNESS = BRIGHTNESS_LOW; – screen saver brightness (25 %)

### **Display**

Measurement and display – CO<sub>2</sub>, Temperature, Relative humidity, PM

Main screen:

- Temperature (°C)
- Relative humidity (%)
- CO<sub>2</sub> (ppm)
- air quality assessment
- WiFi signal strength (4 columns)

Text colors according to measured ppm CO<sub>2</sub> value

![image](https://github.com/user-attachments/assets/d38cdb66-6e3e-4a12-88f0-3ed19751da84)

After inactivity longer than the default set time (30 seconds), the screen saver will automatically start - it will reduce the brightness to the set value (25 %).

**S1 button** – secondary screen, display and switch individual values ​​(loop):

1. Temperature
2. Relative humidity
3. PM1.0
4. PM2.5
5. PM4.0
6. PM10

**S2 button** – return to main screen

### **Webserver**

To view the monitor’s webserver page, enter the monitor’s IP address in the browser, which will be displayed on the display after the monitor is connected to the network. The page is responsive, and adapts to mobile devices.

>Example: http://192.168.1.1

The webserver page displays all values ​​measured by the Sensirion SEN66-SIN-T sensor in a table:

- CO<sub>2</sub>
- Temperature
- Relative humidity
- Particulate matter (PM1.0, PM2.5 PM4.0, PM10)
- VOC index
- NOx index

The buttons above the table can be used to switch between the table of values, graphs and service. The graph displays data for the last 12 hours for CO<sub>2</sub>, Temperature and Relative humidity values.

Clicking on the name of a quantity in the table (except for temperature and humidity) will display the details of the quantity below the table. Clicking again or clicking outside the table will hide the text.

> [!NOTE]
Firmware can be upgraded via wifi (**OTA firmware**) in the **Service section**.

> [!NOTE]
Current measured values ​​can also be displayed in **JSON** format

>{"co2":562.0,"temp":24.5,"rh":32.0,"pm1":3.0,"pm2p5":4.0,"pm4":4.6,"pm10":5.0,"noxIndex":1.0,"vocIndex":56.0}

## **Credits**
Fonts and icons taken from this project https://github.com/melkati/CO2-Gadget

## **License**
The firmware for the air quality monitor is free software: you can redistribute and/or modify it.
