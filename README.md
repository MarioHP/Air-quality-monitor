**Monitor Kvality vzduchu**

**Hardware:**

Sensirion SEN66-SIN-T - Senzor kvality ovzduší

LilyGO TTGO T-Display

USB-C kabel pro napájení a nahrání firmwaru

Krabička (tisk na 3D tiskárně)



**Zapojení:**

![image](https://github.com/user-attachments/assets/97987479-7970-4fbd-a6ae-965af0acefc4)

**Software**

Výchozí nastavení v **settings.h**

1)	WIFI_SSID = ""; – název wifi
2)	WIFI_PASSWORD = "";  – heslo na wifi
3)	UPDATE_INTERVAL_MS = 15 * 60 * 1000;  – interval měření dat v milisekundách (15 minut)
4)	HISTORY_LENGTH = 48; – počet vzorků dat v historii grafu za určitý čas (12 hodin)
5)	SCREENSAVER_DELAY_MS = 30000; – čas spořiče displeje v milisekundách (30 sekund)
6)	BACKLIGHT_BRIGHTNESS = BRIGHTNESS_LOW; – jas spořiče displeje (25 %)


**Displej**

Měření a zobrazení – CO2, Teplota, Relativní vlhkost, pevné částice prachu (PM)

Hlavní obrazovka:

1.	Teplota (°C)
2.	Relativní vlhkost (%)
3.	CO2 (ppm) 
4.	bargraf CO2 (rozsah 0 ÷ 5000 ppm)

Barvy textu dle nameřené hodnoty ppm CO2

![image](https://github.com/user-attachments/assets/52d9c6c8-b26e-44fd-a0ec-544d14e47a32)



**Tlačítko S1** – vedlejší obrazovka , zobrazení a přepínání idividuálních hodnot (smyčka):

1.	Teplota
2.	Relativní vlhkost
3.	PM1.0
4.	PM2.5
5.	PM4.0
6.	PM10

**Tlačítko S2** – návrat na hlavní obrazovku

**Webserver**

Pro zobrazení stránky webserveru monitoru zadej do prohlížeče IP adresu monitoru, která se zobrazí na displeji po přihlášení monitoru do sítě. Stránka je responzivní, přizpůsobí se mobilním zařízením.

Příklad: http://192.168.1.1

Stránka webserveru zobrazuje v tabulce všechny hodnoty měřené snímačem Sensirion SEN66-SIN-T:

1.	CO2
2.	Teplota
3.	Relativní vlhkost
4.	pevné částice prachu (PM1.0, PM2.5 PM4.0, PM10)
5.	VOC index
6.	NOx index

Měřené hodnoty je možno získat i ve formátu JSON na adrese http://**IP adresa monitoru**/data

{"co2":0.0,"temp":0.0,"rh":0.0,"pm1":0.0,"pm2p5":0.0,"pm4":0.0,"pm10":0.0}


Tlačítky nad tabulkou lze přepínat mezi grafy a tabulkou. Graf zobrazuje data za posledních 12 hodin u hodnot CO2, Teploty a Relativní vlhkosti.


Kliknutím v tabulce na název veličiny (mimo teplotu a vlhkost) se zobrazí podrobnosti o veličině pod tabulkou. Opětovným kliknutím nebo klikem mimo tabulku se text schová.




**Air quality monitor**
