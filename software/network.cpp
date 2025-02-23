#include "network.h"
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include "globals.h"

void initNetwork() {
    // Set device hostname
    WiFi.hostname("czastozysk");

    // Configure a static IP address (adjust these values to your network settings)
    IPAddress local_IP(192, 168, 1, 184);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);
    IPAddress primaryDNS(8, 8, 8, 8);     // optional
    IPAddress secondaryDNS(8, 8, 4, 4);   // optional

    WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);

    // Start WiFiManager to allow configuration if credentials are not stored
    WiFiManager wifiManager;
    Serial.println("odpalam access point...");
    if (!wifiManager.autoConnect("fotowoltaika driver")) {
        Serial.println("nie udalo sie utworzyc ap");
        delay(1000);
    }
    Serial.println("polaczono z wifi");
    Serial.print("adres ip lokalny: ");
    Serial.println(WiFi.localIP());

    // Set up NTP with a 3600 sec offset
    configTime(3600, 0, "pool.ntp.org", "time.nist.gov");
}
