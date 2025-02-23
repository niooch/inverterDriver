#include "network.h"
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include "globals.h"

void initNetwork() {
  WiFiManager wifiManager;
  Serial.println("odpalam access point...");
  if (!wifiManager.autoConnect("fotowoltaika driver")) {
    Serial.println("nie udalo sie utworzyc ap");
    delay(1000);
  }
  Serial.println("polaczono z wifi");
  Serial.print("adres ip lokalny: ");
  Serial.println(WiFi.localIP());

  // Ustawienie zegara NTP (offset = 3600 s, brak DST)
  configTime(3600, 0, "pool.ntp.org", "time.nist.gov");
}
