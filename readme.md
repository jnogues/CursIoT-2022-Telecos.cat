Repositori Curs IoT 2022 Telecos.cat

![](https://eetac.upc.edu/ca/noticies/masteam-matt-talk-carles-sabater-telecos-cat-helping-you-to-grow-in-your-professional-career/@@images/6fd521cc-8f7d-4ffe-98b0-ebe5a1206d85.png)

## Notes complementàries de la 1a Sessió 10.02.2022

1. [Presentació.](http://167.86.87.117/deTot/presentacioIoT2022-telecos-1.pdf)
2. [Ivan Grokhotkov, lider portabilitat ESP8266 a Arduino.](https://github.com/igrr)
3. [Espressif, fabricant de l'ESP8266 i ESP32.](https://www.espressif.com/en)
4. [On comprar les nodeMCU, AZDelivery (Amazom).](https://www.amazon.es/AZDelivery-NodeMCU-ESP8266-ESP-12E-Desarrollo/dp/B06Y1LZLLY?ref_=ast_sto_dp&th=1)
5. [Pcb de pràctiques.](https://oshwlab.com/jnogues/nodemcupcb-2019)
6. [Repositori oficial del servidor privat de blynk.](https://github.com/blynkkk/blynk-server)
7. [Pàgina oficial Blynk.](https://blynk.io/en/developers)
8. Instal·lar plugin ESP8266 a l'Arduino IDE `https://arduino.esp8266.com/stable/package_esp8266com_index.json `
9. [LLibreria de blynk a utilitzar, 0.6.1](https://github.com/blynkkk/blynk-library/releases/tag/v0.6.1)
10. [Aplicació de Blynk per Android.](https://github.com/jnogues/CursIoT-2022-Telecos.cat/tree/main/app)
11. [Primeres passes amb Blynk App.](https://github.com/jnogues/CursIoT-2022-Telecos.cat/tree/main/videos)
12. [Sketchs de pràctiques.](https://github.com/jnogues/CursIoT-2022-Telecos.cat/tree/main/code)
13. [Pocket IoT, Thingspeak???](https://play.google.com/store/apps/details?id=com.buzsikdev.pocketiot&hl=ca&gl=US)

---

## Notes complementàries de 2a Sessió 17.02.2022

1. LLista de la compra: 
 * [nodeMCU](https://www.amazon.es/AZDelivery-NodeMCU-ESP8266-ESP-12E-Desarrollo/dp/B06Y1LZLLY?ref_=ast_sto_dp&th=1&psc=1).
 * [DS18B20](https://www.amazon.es/AZDelivery-conexi%C3%B3n-circuito-arranque-temperatura/dp/B07WGH7HTF/ref=sr_1_2_sspa?__mk_es_ES=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=34BJTU67SAN66&keywords=azdelivery+ds18b20&qid=1645175050&sprefix=azdelivery+ds18b20%2Caps%2C71&sr=8-2-spons&psc=1&smid=A1X7QLRQH87QA3&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUE2TkNDR0xKV09CWTQmZW5jcnlwdGVkSWQ9QTAyODEwNDExRUhUQ0FER1NPUTdaJmVuY3J5cHRlZEFkSWQ9QTA3MTU5ODExMTZZNUdRVFM5SE1LJndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==).
 * [bme280](https://www.amazon.es/AZDelivery-el%C3%A9ctrico-Temperatura-Impresi%C3%B3n-Raspberry/dp/B07D8T4HP6/ref=sr_1_1?__mk_es_ES=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=30THXSRAFJ2K0&dd=7-wYWwZWa4vjNgfJ26iXbQ%2C%2C&keywords=bme280&qid=1645175125&refinements=p_90%3A6820340031&rnid=6820335031&sprefix=bme280%2Caps%2C69&sr=8-1)
2. MQTT, [Podcast de ProgamarFacil](https://programarfacil.com/esp8266/mqtt-esp8266-raspberry-pi/), [Schneider](https://youtu.be/3huVaqwZFHg), [Jonathan Gonzalez - ACONTRATECH](https://youtu.be/JMaA8Oi8Bzs).
3. IP del broker mosquitto del Rambla Prim, 5.196.88.155, port 1883.
4. Per instl·lar els clients **mosquitto_sub i mosquitto_pub** en LINUX, `sudo apt install mosquitto-clients`.
5. Per subscriure't a un topic: `mosquitto_sub -v -h 5.196.88.155 -t /kkp2/#`.
6. Per publicar en un topic: `mosquitto_pub -h 5.196.88.155 -t /kkp2/temperatura -m 23.5`.
7. Broker públic: `test.mosquitto.org`.
8. Vídeos d'introducció a Blynk 2.0, [techiesms](https://youtu.be/IKbbvEzZ7wg), [
jadsa tv](https://youtu.be/HRGQQATYJCQ).
9. Node-red, [Podcast de ProgramarFacil](https://youtu.be/ZgG-rKLP_XI), [Un loco y su tecnología
](https://youtu.be/tiG6DmSvia4).
10. Instal·lar mosquitto a Windows [Luis Llamas](https://www.luisllamas.es/como-instalar-mosquitto-el-broker-mqtt/)





