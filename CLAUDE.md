# CLAUDE.md

IDEASPARK-OLED-TIME: an ESP8266 sketch that shows the current time on the 0.96" OLED of an ideaspark ESP8266 board, fetching it over WiFi from an NTP server. Public repo. The README only notes a hardware quirk (see Gotchas).

## Layout
- `platformio.ini` - env `nodemcuv2` (`espressif8266@4.2.1`, Arduino, 115200 baud) with Adafruit GFX, SSD1306 and BusIO, NTPClient and Time libraries. A trailing empty `[env:myenv]` section is left over and does nothing.
- `src/oled.ino` - the whole sketch: I2C on `Wire.begin(D6, D5)`, SSD1306 128x64 at address 0x3C, NTPClient against pool.ntp.org with a fixed UTC-5 offset (hard-coded, no automatic daylight saving), FreeSans fonts.
- `include/`, `lib/`, `test/` hold only PlatformIO's placeholder READMEs.

## Build / flash
- `pio run -t upload`, then `pio device monitor` at 115200. I found no tests and no CI.

## Gotchas
- The WiFi SSID and password are hard-coded as constants at the top of `src/oled.ino`. This repo is public, so never commit real credentials; replace them with placeholders or load them from an untracked file before committing, and tell the owner if real ones are already in the history.
- Per the README, PlatformIO sometimes fails to reset this board and the serial monitor shows garbage; power-cycle the board to recover.
- The time zone offset is a constant in the sketch; change it there.

## Deploy
- Push to `main`. There are no tags, releases or workflows.
