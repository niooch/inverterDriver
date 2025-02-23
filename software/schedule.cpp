#include "schedule.h"
#include "globals.h"
#include <Arduino.h>

void computeGridSchedule() {
  for (int i = 0; i < 24; i++) {
    gridSchedule[i] = false;
  }
  if (priceCount == 0) {
    Serial.println("Brak danych cenowych do obliczenia harmonogramu.");
    return;
  }
  int sellCount = min(numSellHours, priceCount);
  for (int i = priceCount - sellCount; i < priceCount; i++) {
    int hr = prices[i].hour;
    gridSchedule[hr] = true;
  }
  Serial.print("ustawiono godziny sprzedazy: ");
  for (int i = 0; i < 24; i++) {
    if (gridSchedule[i]) {
      Serial.print(i);
      Serial.print(":00 ");
    }
  }
  Serial.println();
}
