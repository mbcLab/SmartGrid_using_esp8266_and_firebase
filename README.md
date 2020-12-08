# Monitoring Konsumsi Daya dengan PZEM dan esp8266
### Sistem ini menggunakan mikrokontroler esp8266 __bukan esp32__
Semua library yang diperlukan sudah ada di repository ini, untuk firebase nya menggunakan email MBC, jadi silahkan menghubungi Asisten Lab MBC untuk mendapatkan aksesnya, atau edit di codingan arduino nya.
### Schematic rangkaian kontroler:
| esp8266 | PZEM | Relay |
|---------|------|-------|
|3v       |5v    |VCC    |
|GND      |GND   |GND    |
|D1       |NC    |IN     |
|D5       |TX    |NC     |
|D6       |RX    |NC     |
- [x] Monitoring komsumsi daya
- [x] Kirim data ke firebase
- [ ] Kontrol relay via firebase
