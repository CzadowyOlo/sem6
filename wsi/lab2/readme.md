### LISTA 2 - kółko i kzyżyk

# rozważmy następującą modyfikację gry kólko i krzyżyk:
- gra jest na planszy 5 × 5,
- wygrywa gracz, który pierwszy ustawi cztery swoje symbole w jednej linii bez przerw,
- jeśli jednak gracz ustawi nieprzerwaną linię tylko z trzech swoich symboli, to przegrywa
(o ile ostatni ruch nie ustawił jakiejś czwórki).

# W załączonym pliku labor2.zip znajduje się prosta implementacja serwera gry i klienta
dla gracza. Protokół gry przewiduje:
1. połączenie się klientów z serwerem (serwer obsługuje tylko jedną grę z dwoma gra-
czami),
2. poczekanie przez klienta na komunikat z serwera 700, na który należy odpowiedzieć
numerem gracza 1 (dla X) lub 2 (dla O),
3. gracz 1 otrzymuje następnie komunikat 600, że może rozpocząć grę,
4. gracze na przemian wysyłają do serwera swoje ruchy, oznaczające wybrane pole we-
dług poniższej tabeli:
| 11 | 12 | 13 | 14 | 15 |
| 12 | 22 | 23 | 24 | 25 |
| 31 | 32 | 33 | 34 | 35 |
| 41 | 42 | 43 | 44 | 45 |
| 51 | 52 | 53 | 54 | 55 |
5. serwer po sprawdzeniu ruchu gracza przesyła go do przeciwnika (jeśli nie powoduje
on końca gry) lub przesyła najpierw przeciwnikowi a następnie graczowi komunikat o
końcu gry zgodnie z poniższymi regułami
• 1xx - wygrał gracz, który otrzymał ten komunikat (xx=00 jeśli było to w wyniku
jego ruchu, w p.p. xx to ruch przeciwnika powodujący koniec gry),
• 2xx - przegrał gracz, który otrzymał ten komunikat (xx=00 jeśli było to w wyniku
jego ruchu, w p.p. xx to ruch przeciwnika powodujący koniec gry),
• 3xx - zremisował gracz, który otrzymał ten komunikat (xx=00 jeśli było to w wy-
niku jego ruchu, w p.p. xx to ruch przeciwnika powodujący koniec gry),
• 400 - wygrana z powodu błędu przeciwnika,
• 500 - przegrana z powodu własnego błędu.
