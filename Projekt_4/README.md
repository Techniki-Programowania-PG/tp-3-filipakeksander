# Projekt 4: Winda

## Autorzy:
- Filip Skudlarz 203314
- Aleksander Kargol 203225

## Informacje dla uzytkownika programu:
Program jest przeznaczony do korzystania w systemie Windows przy ustawieniach systemu na **100% skali obrazu**. W przeciwnym wypadku moze nie wyswietlac sie w sposob oczekiwany.

![Image](https://github.com/user-attachments/assets/c22061a5-1b63-4be0-895a-a001df6ac917)
## Opis dzialania:

**Stany windy:**
Winda dziala w 5 stanach:

0 (Zamknieta i na pietrze) - czeka na decyzje (otwarcie drzwi lub ruch)
1 (Otwieranie drzwi) - drzwi windy otwieraja sie
2 (Otwarta) - ludzie wchodza/wychodza, winda podejmuje decyzje o kierunku jazdy
3 (Zamykanie drzwi) - drzwi sie zamykaja
4 (Poruszanie sie) - winda jedzie w gore lub w dol do nastepnego pietra

**Decyzje windy:**

Gdy winda jest zamknieta (stan 0):
- Jesli jest pusta:
  - Sprawdza, czy sa ludzie w kolejce
  - Jesli tak: jedzie do pietra, gdzie jest pierwszy czekajacy
  - Jesli nie: po 5 sekundach bezczynnosci wraca na parter (pietro 0)
- Jesli ma pasazerow:
  - Sprawdza, czy ktos chce wysiasc na aktualnym pietrze lub czy nowi pasazerowie chca wsiasc
  - Jesli tak: otwiera drzwi
  - Jesli nie: jedzie dalej w obecnym kierunku

Gdy winda jest otwarta (stan 2):
- Wysiadanie: Pasazerowie, ktorych celem jest aktualne pietro, wychodza
- Wsiadanie: Ludzie z pietra wsiadaja (jesli jest miejsce i jada w tym samym kierunku)
- Zmiana kierunku: Jesli wszyscy w windzie jada w przeciwnym kierunku niz obecny, winda zmienia kierunek

**Animacje:**

- Ludzie:
  - Poruszaja sie po osi X (do/od windy) z efektem chodzenia
  - Wyswietlani sa na odpowiednich pozycjach w windzie

- Drzwi windy:
  - Otwieranie/zamykanie przy wpuszczaniu i wypuszczaniu ludzi
  - Ruch windy w gore i w dol

**Sterowanie:**

Przyciski na pietrach dodaja ludzi do kolejki (oczekujacy)

**Masa przewożonych pasażerów:**

Każdy człowiek waży 70 kg, a aktualna masa przewożonych pasażerów jest wyświetlana w windzie nad pasażerami

## Demonstracja działania programu:

https://github.com/user-attachments/assets/e1667d91-14be-44e7-a28f-7202e67c8547
