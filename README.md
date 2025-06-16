# **Monitor kvality vzduchu**
Monitor byl navržen speciálně pro senzor Sensirion SEN66-SIN-T

## **Hardware:**

Sensirion SEN66-SIN-T: https://sensirion.com/products/catalog/SEN66

LilyGO TTGO T-Display: https://lilygo.cc/products/lilygo%C2%AE-ttgo-t-display-1-14-inch-lcd-esp32-control-board

USB-C kabel pro napájení a nahrání firmwaru

Krabička (tisk na 3D tiskárně)



## **Zapojení:**

![image](https://github.com/user-attachments/assets/97987479-7970-4fbd-a6ae-965af0acefc4)

## **Software a funkce**

Výchozí nastavení v **settings.h**

- WIFI_SSID = ""; – název wifi
- WIFI_PASSWORD = "";  – heslo na wifi
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
- bargraf CO<sub>2</sub> (rozsah 0 ÷ 5000 ppm)

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

Pro zobrazení stránky webserveru monitoru zadej do prohlížeče IP adresu monitoru, která se zobrazí na displeji po přihlášení monitoru do sítě. Stránka je responzivní, přizpůsobí se mobilním zařízením.

>Příklad: http://192.168.1.1

Stránka webserveru zobrazuje v tabulce všechny hodnoty měřené snímačem Sensirion SEN66-SIN-T:

- CO<sub>2</sub>
- Teplota
- Relativní vlhkost
- pevné částice prachu (PM1.0, PM2.5 PM4.0, PM10)
- VOC index
- NOx index

> [!NOTE]
Měřené hodnoty je možno získat i ve formátu JSON na adrese http://**IP adresa monitoru**/data

>{"co2":0.0,"temp":0.0,"rh":0.0,"pm1":0.0,"pm2p5":0.0,"pm4":0.0,"pm10":0.0}


Tlačítky nad tabulkou lze přepínat mezi tabulkou hodnot, grafy a servisem. Graf zobrazuje data za posledních 12 hodin u hodnot CO<sub>2</sub>, Teploty a Relativní vlhkosti.


Kliknutím v tabulce na název veličiny (mimo teplotu a vlhkost) se zobrazí podrobnosti o veličině pod tabulkou. Opětovným kliknutím nebo klikem mimo tabulku se text schová.


> [!NOTE]
Firmware lze upgradovat pomocí wifi (**OTA firmware**) na adrese http://**IP adresa monitoru**/update v sekci **Servis**


## **Poděkování**
Fonty a ikony převzaty z tohoto projektu https://github.com/melkati/CO2-Gadget

## **Licence**
Firmware pro monitor kvality vzduchu je svobodný software: můžete jej dále šířit a/nebo upravovat.

# **Air quality monitor**
