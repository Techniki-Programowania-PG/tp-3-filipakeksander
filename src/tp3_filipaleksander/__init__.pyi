"""
Biblioteka generująca, przekształcająca i wyświetlająca sygnały
"""
def generuj_sin(czestotliwosc: float,amplituda: float,ilosc_punktow: int, x_min: float,x_maks: float,przesuniecie: float) -> list:
    """
    Generuje sygnał sinusowy o podanej częstotliwości i przesunięciu w fazie

    Zwraca 2-wymiarową tablice o wielkości równej ilości punktów

    wyjście[0] - oś x (czasu)

    wyjście[i] - oś y (wartości)
    """
def generuj_cos(czestotliwosc: float,amplituda: float,ilosc_punktow: int,x_min: float,x_maks: float,przesuniecie: float) -> list:
    """
    Generuje sygnał cosinusowy o podanej częstotliwości i przesunięciu w fazie

    Zwraca 2-wymiarową tablice o wielkości równej ilości punktów

    wyjście[0] - oś x (czasu)

    wyjście[i] - oś y (wartości)
    """
def generuj_prostokatny(czestotliwosc: float,amplituda: float,ilosc_punktow: int, wypelnienie: int,x_min: float,x_maks: float) -> list:
    """
    Generuje sygnał prostokątny o podanej częstotliwości i wypełnieniu

    Zwraca 2-wymiarową tablice o wielkości równej ilości punktów

    Wypełnienie od 0% do 100%

    wyjście[0] - oś x (czasu)

    wyjście[i] - oś y (wartości)
    """
def generuj_piloksztaltny(czestotliwosc: float,amplituda: float,ilosc_punktow: int,x_min: float,x_maks: float) -> list:
    """
    Generuje sygnał piłokształtny o podanej częstotliwości

    Zwraca 2-wymiarową tablice o wielkości równej ilości punktów

    wyjście[0] - oś x (czasu)

    wyjście[i] - oś y (wartości)
    """
def wyswietl_2D(x: list,y: list,nazwa: str,y_label: str,x_label: str,nazwa_pliku: str) -> None:
    """
    Wyświetla wykres y(x)

    Jeżeli nazwa_pliku == "" to biblioteka tylko wyświetla wykres, w przeciwnym wypadku zapisuje go pod podaną nazwą (gdy zapisujesz plik wyświetlanie będzie nie do końca poprawne)
    """
def wyswietl_1D(y: list,nazwa: str,y_label: str,nazwa_pliku: str) -> None:
    """
    Wyświetla wykres y

    Jeżeli nazwa_pliku == "" to biblioteka tylko wyświetla wykres, w przeciwnym wypadku zapisuje go pod podaną nazwą (gdy zapisujesz plik wyświetlanie będzie nie do końca poprawne)
    """
def dft(x: list) -> list:
    """
    Oblicza dyskretną transformate Fouriera
    """
def idft(transformata: list) -> list:
    """
    Oblicza odwróconą dyskretną transformate Fouriera
    """
def wartosc_bezwzgledna(x: list) -> list:
    """
    Oblicza moduły licz zespolonych w tablicy
    """
def wyswietl_amplitude_dft(transformata: list,czestotliwosc_probkowania: float,czy_tylko_dodatnie: bool,nazwa: str,nazwa_pliku: str) -> None:
    """
    Ładnie wyświetla dyskretną transformate Fouriera

    Częstotliwość próbkowania jest równa: ilość_punktów / (x_maks - x_min)

    Czy_tylko_dotanie ustala czy wyświetla ujemne częstotliwości

    Jeżeli nazwa_pliku == "" to biblioteka tylko wyświetla wykres, w przeciwnym wypadku zapisuje go pod podaną nazwą (gdy zapisujesz plik wyświetlanie będzie nie do końca poprawne)
    """
def zaszum(sygnal: list,moc_zaszumiania: float) -> list:
    """
    Zaszumia sygnał

    Moc_zaszumiania odpowiada za to jak bardzo jest zaszumiony sygnał

    maksymalne zaszumnie dla punktu równa się (sygnal_max - sygnal_min) * (moc_zaszumiania / 100)
    """
def filtracja1D(sygnal: list,filtr: list) -> list:
    """
    Filtruje sygnał 1D za pomocą podanego sygnału

    Przykładowe filtry:

    Średnia ruchoma [1/3,1/3,1/3]

    Filtr różnicowy [-1,1] lub [-1,0,1]

    Filtr Gaussa [0.25,0.5,0.25]

    Filtr wykrywający piki [1,-2,1]
    """
def filtracja2D(sygnal: list,filtr: list) -> list:
    """
    Filtruje sygnał 2D za pomocą podanego sygnału

    Przykładowe filtry:

    Box blur (uśredniający) [[1/9,1/9,1/9],[1/9,1/9,1/9],[1/9,1/9,1/9]]

    Gaussian blur [[0.0625, 0.125, 0.0625],[0.125,  0.25,  0.125 ],[0.0625, 0.125, 0.0625]]

    Filtr Sobela X [[-1,0,1],[-2,0,2],[-1,0,1]]

    Filtr Sobela Y [[-1,-2,-1],[0,0,0],[1,2,1]]

    Filtr Laplace'a  [[-1,-1,-1],[-1,8,-1],[-1,-1,-1]]
    """
def wyswietl_img(img: list,nazwa: str,nazwa_pliku: str) -> None:
    """
    Wyśiwtla zdjęcie w skali szarości

    Nazwa to tytuł wykresu

    Używane do pokazu filtracji 2D

    Jeżeli nazwa_pliku == "" to biblioteka tylko wyświetla wykres, w przeciwnym wypadku zapisuje go pod podaną nazwą (gdy zapisujesz plik wyświetlanie będzie nie do końca poprawne)
    """