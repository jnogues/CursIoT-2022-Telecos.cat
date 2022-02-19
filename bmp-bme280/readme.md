## Tips and tricks pels sensors BMP/BME280

Al comprar aquests sensors mai sabem si ens enviaran un o l'altre. Per això aquó proposo un flux de treball per comprovar-ho.

1. Primer connectem el sensor a 3,3V, GND, SDA (GPIO4), SCL (GPIO5).
2. Provem quina adreça té el nostre sensor amb aquest programa: [scanI2C](https://github.com/jnogues/CursIoT-2022-Telecos.cat/blob/main/bmp-bme280/scanI2C.ino).
3. Hauria de ser 0x76 o 0x77.
4. Ara instl·lem aquestes dos llibreries mitjançant un fitxer zip, [BMP280](https://github.com/Seeed-Studio/Grove_BMP280/archive/refs/heads/master.zip), [BME280](https://github.com/Seeed-Studio/Grove_BME280/archive/refs/heads/master.zip).
5. Si creiem que tenim un BMP280, provem aquest programa: 

