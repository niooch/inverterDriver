#!bin/bash

# pobierz dzisiejsza date:
DATE=$(date '+%d-%m-%Y')
DATE_FROM="${DATE}T00:00:00Z"
DATE_TO="${DATE}T23:59:59Z"

# url do strony:
API_URL="https://energy-instrat-api.azurewebsites.net/api/prices/energy_price_rdn_hourly?date_from=${DATE_FROM}&date_to=${DATE_TO}"

echo "pobieranie danych z ${API_URL} na dzien ${DATE}..."
RESPONCE=$(curl -s "${API_URL}")

# sprawdzenie czy nie zwrocono bledu
if [[ -z "$RESPONCE" || "$RESPONCE" == "[]" || "$RESPONCE" == "" ]]; then
	echo "nie znaleziono danych dla dnia ${DATE}"
	exit 1
fi
# posortuj i wyswietl
echo "$RESPONCE" | jq -r '
    sort_by(.fixing_i.price)[] |
    "\(.date | strptime("%Y-%m-%dT%H:%M:%SZ") | strftime("%H:%M")): \(.fixing_i.price)"
'
