#include "globals.h"
#include "network.h"
#include "api.h"
#include "schedule.h"
#include "webserver.h"

// Define globals
const char* apiURL = "http://70.34.253.46/api";
const char* apiUser = "test";
const char* apiPassword = "D00psko15";

// Create the web server instance on port 80
ESP8266WebServer server(80);

// Data from API
HourPrice prices[24];
int priceCount = 0;

// Default selling hours and schedule
int numSellHours = 4;
bool gridSchedule[24] = { false };

void setup() {
  delay(100);
  Serial.begin(115200);

  // Initialize WiFi and NTP
  initNetwork();

  // Fetch API data and compute schedule
  fetchEnergyPrices();
  computeGridSchedule();

  // Register web endpoints with the new interface:
  server.on("/", handleIndex);                      // Main page (index.html)
  server.on("/ustawienia_godzin", handleUstawieniaGodzin); // Hours settings page
  server.on("/set", handleSet);                       // Update numSell from index page
  server.on("/set_wifi", handleSetWifi);              // Process WiFi settings (stub)
  server.on("/set_api", handleSetApi);                // Process API credentials (stub)
  server.on("/set_hours", handleSetHours);            // Process hours selection from dedicated page

  server.begin();
  Serial.println("interface webowy uruchomiony");
}

void loop() {
  server.handleClient();

  // Refresh API data every 30 minutes
  static unsigned long lastUpdate = 0;
  static unsigned long refreshTime = 3600000 / 2; // 30 min
  if (millis() - lastUpdate > refreshTime) {
    fetchEnergyPrices();
    computeGridSchedule();
    lastUpdate = millis();
  }

  // Get current hour (NTP-synced)
  time_t now = time(nullptr);
  struct tm* timeInfo = localtime(&now);
  int currentHour = timeInfo->tm_hour;
  Serial.print("teraz jest godzina: ");
  Serial.println(currentHour);

  // Decide whether to sell energy or charge batteries
  if (gridSchedule[currentHour]) {
    Serial.println("wysylanie energii do sieci.");
    // Add hardware control code for grid sale here
  } else {
    Serial.println("ladowanie baretii.");
    // Add hardware control code for battery charging here
  }
  delay(10000);  // 10 s
}
