# inverterDriver

Firmware for an ESP8266-based inverter controller that switches between battery charging and exporting energy to the grid based on hourly price data. The device exposes a small web UI for monitoring and changing schedule settings.

The current implementation is an Arduino-style ESP8266 sketch split across several `.cpp` / `.h` files under [`software/`](/root/inverterDriver/software).

## What It Does

- Connects to Wi-Fi using WiFiManager.
- Starts a local web server on port `80`.
- Fetches hourly energy prices from a remote API at boot and every 30 minutes.
- Builds a 24-hour export schedule from the highest-priced hours.
- Toggles GPIO pins to select between exporting energy to the grid and charging batteries.

## Repository Layout

- [`software/software.ino`](/root/inverterDriver/software/software.ino): firmware entry point
- [`software/network.cpp`](/root/inverterDriver/software/network.cpp): Wi-Fi setup, captive portal, NTP
- [`software/api.cpp`](/root/inverterDriver/software/api.cpp): price download and parsing
- [`software/schedule.cpp`](/root/inverterDriver/software/schedule.cpp): schedule calculation
- [`software/webserver.cpp`](/root/inverterDriver/software/webserver.cpp): built-in web UI and form handlers
- [`software/index.html`](/root/inverterDriver/software/index.html): older static HTML mockup
- [`software/ustawienia_godzin.html`](/root/inverterDriver/software/ustawienia_godzin.html): older static HTML mockup
- [`software/software_old`](/root/inverterDriver/software/software_old): older monolithic version of the sketch
- [`nowe.sh`](/root/inverterDriver/nowe.sh): helper script for querying price data from Instrat
- [`update.sh`](/root/inverterDriver/update.sh): helper script for querying price data from PSE

## Hardware and Runtime Behavior

The firmware currently drives two GPIO outputs:

- `GPIO3` high, `GPIO4` low: export energy to the grid
- `GPIO3` low, `GPIO4` high: charge batteries

The control decision is evaluated in the main loop every 10 seconds using the current hour and the computed `gridSchedule[24]`.

## Dependencies

This project targets the ESP8266 Arduino ecosystem.

Required board support and libraries:

- ESP8266 Arduino core
- `ESP8266WiFi`
- `ESP8266WebServer`
- `ESP8266HTTPClient`
- `WiFiManager`

The helper scripts also require:

- `curl`
- `jq`

## Configuration

The API endpoint and credentials are hardcoded near the top of [`software/software.ino`](/root/inverterDriver/software/software.ino). Change them before deploying to real hardware.

The device uses:

- Wi-FiManager AP name: `czastozysk`
- hostname: `czastozysk`
- NTP servers: `pool.ntp.org`, `time.nist.gov`

There is commented-out support for a static IP in [`software/network.cpp`](/root/inverterDriver/software/network.cpp).

## Building and Flashing

There is no `platformio.ini` or Arduino CLI config in this repository, so the expected workflow is the Arduino IDE or an equivalent ESP8266-capable toolchain.

Typical setup:

1. Install the ESP8266 board package.
2. Install the `WiFiManager` library.
3. Open [`software/software.ino`](/root/inverterDriver/software/software.ino).
4. Select your ESP8266 board and serial port.
5. Update API credentials if needed.
6. Build and flash the firmware.
7. Open the serial monitor at `115200` baud.

## First Boot

On first boot, if Wi-Fi credentials are not already stored, the device starts a captive portal. Connect to the `czastozysk` access point and use WiFiManager to join your local network.

After connecting, the firmware prints connection details to the serial monitor, including the local IP address. Open that address in a browser to access the web UI.

## Web Interface

Registered routes:

- `/`: main page with Wi-Fi/API forms
- `/ustawienia_godzin`: pricing view and schedule form
- `/set`: updates `numSellHours`
- `/set_wifi`: placeholder handler
- `/set_api`: placeholder handler
- `/set_hours`: accepts selected hours and sale count

Important: the current web UI is generated directly in C++ by [`software/webserver.cpp`](/root/inverterDriver/software/webserver.cpp). The checked-in HTML files are not what the firmware serves at runtime.

## Scheduling Logic

The current schedule implementation:

1. Downloads hourly prices.
2. Parses lines in `HH:MM: price` form.
3. Stores up to 24 entries in memory.
4. Marks the highest `numSellHours` entries as export hours.

This assumes the received price list is already ordered in a useful way. The code does not sort prices itself.

## Helper Scripts

`nowe.sh`

- Fetches the current day's hourly energy prices from the Instrat API.
- Prints sorted `HH:MM: price` output.

Run:

```bash
./nowe.sh
```

`update.sh`

- Fetches price data for a selected date from the PSE API.

Run:

```bash
./update.sh 2026-03-10
```

## Current Limitations

- Wi-Fi settings submitted through the web page are only logged to serial; they are not saved or applied.
- API settings submitted through the web page are only logged to serial; they are not persisted.
- Manual hour selection from `/set_hours` is overwritten by `computeGridSchedule()`, so the UI does not currently provide a true manual schedule.
- API credentials are stored in plain text in firmware.
- Timezone handling is hardcoded and should be reviewed before production use.
- GPIO behavior is hardcoded and there is no safety layer for real inverter hardware.

## Development Notes

Most source comments and UI text are in Polish. The current codebase is closer to a working prototype than a production-ready controller, but the project structure is already split into sensible modules for networking, API access, scheduling, and the web UI.
