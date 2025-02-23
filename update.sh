#!/bin/bash

data=${1:-$(date +'%Y-%m-%d')}

api_link="https://api.raporty.pse.pl/api/crb-rozl"

echo "pobieranie danych z strony $api_link na dzien $data"

#na podstawie dokumentacji z strony pse
dane=$(curl -s --globoff "${api_link}?$filter=business_date%20eq%20%27${data}%27" -H "Accept: application/json")

#debuging
echo "surowe dane:"
echo "$dane"


#sprawdzenie czy udane pobranie danych
if [ $? -ne 0 ]; then
	echo "blad: nie udalo sie pobrac danych z stony"
	exit 1
fi

#parse danych
ceny=$(echo "$dane" | jq -r '.value[] | "\(.udtczas_oreb) - \(.cor_pp_rozl)"' | sort)
#sprawdzenie parsu
if [ -z "$ceny" ]; then
	echo "nie znaleziono dla $data"
	exit 1
fi

#wyswietl dane o ile sie udalo
echo "ceny godzinowe dla dnia $data:"
echo "$ceny"
