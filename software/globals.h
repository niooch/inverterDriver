#ifndef GLOBALS_H
#define GLOBALS_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// ----- DANE -----
extern const char* apiURL;
extern const char* apiUser;
extern const char* apiPassword;

// Global web server instance
extern ESP8266WebServer server;

// Struktura danych z API
struct HourPrice {
  int hour;
  float price;
};

extern HourPrice prices[24];
extern int priceCount;

// Ilość godzin sprzedaży (default)
extern int numSellHours;

// Tablica z godzinami do sprzedaży
extern bool gridSchedule[24];

#endif

