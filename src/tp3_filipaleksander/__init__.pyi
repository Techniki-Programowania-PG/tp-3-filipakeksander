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
def wyswietl_2D(x: list,y: list,nazwa: str,y_label: str,x_label: str) -> None:
    """
    Wyświetla wykres y(x)

    """
def wyswietl_1D(y: list,nazwa: str,y_label: str) -> None:
    """
    Wyświetla wykres y
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
def wyswietl_dft(transformata: list,czestotliwosc_probkowania: float,czy_tylko_dodatnie: bool,nazwa: str,y_label: str) -> None:
    """
    Ładnie wyświetla dyskretną transformate Fouriera

    Częstotliwość próbkowania jest równa: ilość_punktów / (x_maks - x_min)

    Czy_tylko_dotanie ustala czy wyświetla ujemne częstotliwości
    """
def zaszum(sygnal: list,moc_zaszumiania: float) -> list:
    """
    Zaszumia sygnał

    Moc_zaszumiania odpowiada za to jak bardzo jest zaszumiony sygnał

    maksymalne zaszumnie dla punktu równa się (sygnal_max - sygnal_min) * (moc_zaszumiania / 100)
    """