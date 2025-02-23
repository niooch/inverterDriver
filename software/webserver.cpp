#include "webserver.h"
#include "globals.h"
#include "schedule.h" // for computeGridSchedule
#include <ESP8266WebServer.h>

// Serve the main page (index.html)
void handleIndex() {
  String html = "<!DOCTYPE html><html lang='pl'><head><meta charset='UTF-8'><title>Sterowanie Inwerterem i Konfiguracja WiFi</title><style>";
  html += "body { font-family: Arial, sans-serif; margin: 20px; background-color: #f7f7f7; }";
  html += ".container { max-width: 800px; margin: auto; background: #fff; padding: 20px; border-radius: 8px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }";
  html += "h1, h2 { text-align: center; }";
  html += ".panel { border: 1px solid #ccc; padding: 15px; margin-bottom: 20px; border-radius: 5px; }";
  html += "label { display: block; margin: 10px 0 5px; }";
  html += "input[type='text'], input[type='password'] { width: 100%; padding: 8px; box-sizing: border-box; }";
  html += "input[type='submit'] { padding: 10px 20px; background-color: #28a745; color: #fff; border: none; cursor: pointer; margin-top: 10px; }";
  html += "input[type='submit']:hover { background-color: #218838; }";
  html += "nav { text-align: center; margin-bottom: 20px; }";
  html += "nav a { margin: 0 10px; text-decoration: none; color: #007bff; }";
  html += "nav a:hover { text-decoration: underline; }";
  html += "</style></head><body>";
  html += "<div class='container'><h1>Sterowanie Inwerterem i Konfiguracja WiFi</h1>";
  html += "<nav><a href='/'>Strona Główna</a> <a href='/ustawienia_godzin'>Ustawienia Godzin</a></nav>";
  // Network Status Panel
  html += "<div class='panel'><h2>Status Sieci</h2>";
  html += "<p id='wifiStatus'>Brak połączenia WiFi. Skonfiguruj sieć.</p>";
  html += "<p>Aktualny adres IP: <span id='ipAddress'>0.0.0.0</span></p></div>";
  // WiFi Settings Panel
  html += "<div class='panel'><h2>Ustawienia WiFi</h2>";
  html += "<form id='wifiForm' action='/set_wifi' method='POST'>";
  html += "<label for='ssid'>SSID:</label>";
  html += "<input type='text' id='ssid' name='ssid' placeholder='Wprowadź SSID' required>";
  html += "<label for='password'>Hasło:</label>";
  html += "<input type='password' id='password' name='password' placeholder='Wprowadź hasło' required>";
  html += "<input type='submit' value='Zapisz ustawienia WiFi'>";
  html += "</form></div>";
  // API Credentials Panel
  html += "<div class='panel'><h2>Dane API</h2>";
  html += "<form id='apiForm' action='/set_api' method='POST'>";
  html += "<label for='apiPassword'>Hasło API:</label>";
  html += "<input type='password' id='apiPassword' name='apiPassword' placeholder='Wprowadź hasło API' required>";
  html += "<input type='submit' value='Zapisz dane API'>";
  html += "</form></div>";
  html += "</div>"; // container
  html += "<script>document.getElementById('ipAddress').innerText = window.location.hostname;</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Serve the dedicated page for setting hours (ustawienia_godzin.html)
void handleUstawieniaGodzin() {
  String html = "<!DOCTYPE html><html lang='pl'><head><meta charset='UTF-8'><title>Ustawienia Godzin Sprzedaży Energii</title><style>";
  html += "body { font-family: Arial, sans-serif; margin: 20px; background-color: #f7f7f7; }";
  html += ".container { max-width: 800px; margin: auto; background: #fff; padding: 20px; border-radius: 8px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }";
  html += "h1, h2 { text-align: center; }";
  html += ".panel { border: 1px solid #ccc; padding: 15px; margin-bottom: 20px; border-radius: 5px; }";
  html += "label { display: block; margin: 10px 0 5px; }";
  html += ".hour-grid { display: flex; flex-wrap: wrap; justify-content: center; }";
  html += ".hour-grid label { margin: 5px; }";
  html += "input[type='number'] { width: 100px; padding: 8px; margin-bottom: 15px; }";
  html += "input[type='submit'] { padding: 10px 20px; background-color: #28a745; color: #fff; border: none; cursor: pointer; margin-top: 10px; }";
  html += "input[type='submit']:hover { background-color: #218838; }";
  html += "nav { text-align: center; margin-bottom: 20px; }";
  html += "nav a { margin: 0 10px; text-decoration: none; color: #007bff; }";
  html += "nav a:hover { text-decoration: underline; }";
  html += "</style></head><body>";
  html += "<div class='container'><h1>Ustawienia Godzin Sprzedaży Energii</h1>";
  html += "<nav><a href='/'>Strona Główna</a> <a href='/ustawienia_godzin'>Ustawienia Godzin</a></nav>";
  html += "<div class='panel'><h2>Wybierz opcje sprzedaży energii</h2>";
  html += "<form id='hoursForm' action='/set_hours' method='POST'>";
  html += "<label for='numSell'>Liczba godzin sprzedaży:</label>";
  html += "<input type='number' id='numSell' name='numSell' min='0' max='24' value='4' required>";
  html += "<p>Wybierz godziny, w których chcesz sprzedawać energię:</p>";
  html += "<div class='hour-grid'>";
  for (int i = 0; i < 24; i++) {
    char buffer[10];
    sprintf(buffer, "%02d:00", i);
    html += "<label><input type='checkbox' name='hour' value='" + String(i) + "'> " + String(buffer) + "</label>";
  }
  html += "</div>";
  html += "<input type='submit' value='Zapisz ustawienia godzin'>";
  html += "</form></div></div>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Process updates from the index page for numSell
void handleSet() {
  if (server.hasArg("numSell")) {
    numSellHours = server.arg("numSell").toInt();
    computeGridSchedule();
  }
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

// Stub: Process WiFi settings (to be implemented as needed)
void handleSetWifi() {
  if (server.hasArg("ssid") && server.hasArg("password")) {
    String ssid = server.arg("ssid");
    String pass = server.arg("password");
    Serial.print("Otrzymano ustawienia WiFi: ");
    Serial.print(ssid);
    Serial.print(" / ");
    Serial.println(pass);
    // Code to save and apply new WiFi credentials would go here.
  }
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

// Stub: Process API credentials (to be implemented as needed)
void handleSetApi() {
  if (server.hasArg("apiPassword")) {
    String apiPass = server.arg("apiPassword");
    Serial.print("Otrzymano hasło API: ");
    Serial.println(apiPass);
    // Code to update API credentials would go here.
  }
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

// Process the hours selection from the dedicated page
void handleSetHours() {
  if (server.hasArg("numSell")) {
    numSellHours = server.arg("numSell").toInt();
  }
  // Reset gridSchedule
  for (int i = 0; i < 24; i++) {
    gridSchedule[i] = false;
  }
  // Process each hour checkbox (if any)
  int n = server.args();
  for (int i = 0; i < n; i++) {
    if (server.argName(i) == "hour") {
      int hr = server.arg(i).toInt();
      gridSchedule[hr] = true;
    }
  }
  computeGridSchedule(); // Optionally recalculate schedule
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}
