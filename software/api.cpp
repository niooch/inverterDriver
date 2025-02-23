#include "api.h"
#include "globals.h"
#include <ESP8266HTTPClient.h>

void fetchEnergyPrices() {
    Serial.println("pobieram dane z API...");
    WiFiClient client;
    HTTPClient http;

    if (http.begin(client, apiURL)) {
        http.setAuthorization(apiUser, apiPassword);
        int httpCode = http.GET();
        if (httpCode > 0) {
            if (httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.println("API:");
                Serial.println(payload);
                parseAPIResponse(payload);
            } else {
                Serial.print("blad pobierania danych, kod: ");
                Serial.println(httpCode);
            }
        } else {
            Serial.print("HTTP GET, error: ");
            Serial.println(http.errorToString(httpCode).c_str());
        }
        http.end();
    } else {
        Serial.println("nie podlaczono do API.");
    }
}

void parseAPIResponse(String response) {
    priceCount = 0;
    int start = 0;
    while (start < response.length()) {
        int endLine = response.indexOf('\n', start);
        if (endLine == -1) { endLine = response.length(); }
        String line = response.substring(start, endLine);
        line.trim();
        if (line.length() > 0) {
            int colonIndex = line.indexOf(':');
            if (colonIndex != -1) {
                String timePart = line.substring(0, colonIndex);
                timePart.trim();
                int hour = timePart.substring(0, 2).toInt();
                int secondColon = line.indexOf(':', colonIndex + 1);
                if (secondColon != -1) {
                    String priceStr = line.substring(secondColon + 1);
                    priceStr.trim();
                    float price = priceStr.toFloat();
                    if (priceCount < 24) {
                        prices[priceCount].hour = hour;
                        prices[priceCount].price = price;
                        priceCount++;
                    }
                }
            }
        }
        start = endLine + 1;
    }
    Serial.println("Sparsowano odpowiedz api:");
    for (int i = 0; i < priceCount; i++) {
        Serial.print(prices[i].hour);
        Serial.print(":00 -> ");
        Serial.println(prices[i].price);
    }
}
