#include <windows.h>
#include <gdiplus.h>
#include <stdio.h>
#include <vector>
#include <queue>
#include <string>
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;
ULONG_PTR gdiplusToken;
//X windy = 615, szer = 370, wys = 175, Y zatrzymania windy = [736,562,388,214,40]
const int y_pietr[5] = { 911,737,563,389,215 };

class czlowiek {
    private:
        int aktualne_pietro;
        int docelowe_pietro;
        bool czy_w_gore;
        int masa;
        int x_aktualne = 0, x_docelowe = 0, pietro_poruszania = 0, y_offset = 0,x_poczatkowe=0;
        bool y_offset_rosnie = true; // Kierunek ruchu w pionie (true = w górę, false = w dół)
        bool porusza_sie = false;
        Image* zdjecie = NULL;
    public:
        czlowiek(int aktualne, int cel) {
            aktualne_pietro = aktualne;
            docelowe_pietro = cel;
            if (cel > aktualne) czy_w_gore = true;
            else czy_w_gore = false;
            switch (cel)
            {
            case 0:
                zdjecie = new Image(L"czlowiek_0.png");
                break;
            case 1:
                zdjecie = new Image(L"czlowiek_1.png");
                break;
            case 2:
                zdjecie = new Image(L"czlowiek_2.png");
                break;
            case 3:
                zdjecie = new Image(L"czlowiek_3.png");
                break;
            default:
                zdjecie = new Image(L"czlowiek_4.png");
                break;
            }
            masa = 70;
        }
        ~czlowiek() {
            delete zdjecie;
        }
        VOID wyswietl_na_pietrze(Graphics& gBuffer, int pozycja) {
            if (porusza_sie) return;
            int x, delta_x;
            if (aktualne_pietro % 2 == 0) {
                x = 545;
                delta_x = -45;
            }
            else {
                x = 1011;
                delta_x = 45;
            }
            gBuffer.DrawImage(zdjecie, x + pozycja * delta_x, y_pietr[aktualne_pietro] - 104, 44, 104);
        }
        VOID wyswietl_w_windzie(Graphics& gBuffer, int y_windy, int pozycja_w_windzie) {
            if (porusza_sie) return;
            gBuffer.DrawImage(zdjecie, 620 + 45 * pozycja_w_windzie, y_windy + 70, 44, 104);
        }
        VOID wyswietl_poruszajacego(Graphics& gBuffer) {
            gBuffer.DrawImage(zdjecie, x_aktualne, y_pietr[pietro_poruszania] - 104 + y_offset, 44, 104);
        }
        bool czy_sie_porusza() {
            return porusza_sie;
        }
        int get_aktualne() {
            return aktualne_pietro;
        }
        int get_docelowe() {
            return docelowe_pietro;
        }
        bool get_kierunek() {
            return czy_w_gore;
        }
        int get_x() {
            return x_aktualne;
        }
        VOID inicjalizuj_ruch(int x0,int x1,int pietro) {
            x_aktualne = x0;
            x_poczatkowe = x0;
            x_docelowe = x1;
			y_offset = 0;
            pietro_poruszania = pietro;
            porusza_sie = true;
        }
        void ruch() {
            int predkosc;
            if (x_aktualne < x_docelowe) predkosc = 10;
            else predkosc = -10;
            if (abs(x_aktualne - x_docelowe) < 10) {
                x_aktualne = x_docelowe;
                porusza_sie = false;
                return;
            }
            x_aktualne += predkosc;

            // Animacja chodzenia
            if (y_offset_rosnie) {
                y_offset++;
                if (y_offset >= 0) y_offset_rosnie = false;
            }
            else {
                y_offset--;
                if (y_offset <= -6) y_offset_rosnie = true;
            }
        }
};
std::vector<std::vector<czlowiek*> > ludzie_na_pietrach;
std::vector<czlowiek*> oczekujacy;
class winda_klassa { //stany windy 0 - zamknieta i na pietrze, 1 - otwieranie windy , 2 - winda otwarta, 3 - zamykanie windy, 4 - poruszajaca sie winda
    private:
        int y_windy = 736;
        int pietro_windy = 0;
        int ilosc_ludzi = 0;
        bool kierunek = true;
        std::vector<czlowiek*> ludzie_w_windzie;
        std::vector<czlowiek*> poruszajacy_w_windzie;
        std::vector<czlowiek*> wchodzacy;
        std::vector<czlowiek*> wychodzacy; 
        std::vector<czlowiek*> poruszajacy_na_pietrze;
        int stan_windy = 0;
        int czas_nieaktywnosci = 0;
        int cel = 0;
        int wys_drzwi = 0;
        const int yWindyNaPietrach[5] = { 736, 562, 388, 214, 40 };
    public:
        winda_klassa() {
            ludzie_w_windzie.resize(8,NULL);
        }
        ~winda_klassa() {
            for (int i = 0; i < ludzie_w_windzie.size(); i++)
                if(ludzie_w_windzie[i] != NULL) delete ludzie_w_windzie[i];
            for (int i = 0; i < wychodzacy.size(); i++)
                if (wychodzacy[i] != NULL) delete wychodzacy[i];
        }
        VOID wyswietl_winde(Graphics& gBuffer) {
            
            Pen orangePen(Color(255, 255, 165, 0), 3);
            gBuffer.DrawRectangle(&orangePen, 615, y_windy, 370, 175);
            //wyświetl licznik masy
            gBuffer.DrawRectangle(&orangePen, 780, y_windy, 40, 25);
            std::wstring numStr = std::to_wstring(ilosc_ludzi*70);
			FontFamily  fontFamily(L"Arial");
			Font        font(&fontFamily, 13, FontStyleBold, UnitPixel);
			PointF      pointF(788, y_windy + 5);
			SolidBrush  textBrush(Color(255, 0, 0, 0));
			gBuffer.DrawString(numStr.c_str(), -1, &font, pointF, &textBrush);


            for (int i = 0; i < ludzie_w_windzie.size(); i++)
                if (ludzie_w_windzie[i] != NULL)
                    ludzie_w_windzie[i]->wyswietl_w_windzie(gBuffer, y_windy, i);
            if (wys_drzwi > 0) {
                Pen drzwiPen(Color(255, 255, 255, 255), 5);
                if (pietro_windy % 2 == 0) {
                    gBuffer.DrawLine(&drzwiPen, 600, y_windy + 174, 600, y_windy + 174 - wys_drzwi);
                    gBuffer.DrawLine(&drzwiPen, 615, y_windy + 174, 615, y_windy + 174 - wys_drzwi);
                }
                else {
                    gBuffer.DrawLine(&drzwiPen, 1000, y_windy + 174, 1000, y_windy + 174 - wys_drzwi);
                    gBuffer.DrawLine(&drzwiPen, 985, y_windy + 174, 985, y_windy + 174 - wys_drzwi);
                }                
            }
            for (int i = 0; i < poruszajacy_na_pietrze.size(); i++) poruszajacy_na_pietrze[i]->wyswietl_poruszajacego(gBuffer);
            for (int i = 0; i < poruszajacy_w_windzie.size(); i++) poruszajacy_w_windzie[i]->wyswietl_poruszajacego(gBuffer);
            for (int i = 0; i < wychodzacy.size(); i++) wychodzacy[i]->wyswietl_poruszajacego(gBuffer);
            for (int i = 0; i < wchodzacy.size(); i++) wchodzacy[i]->wyswietl_poruszajacego(gBuffer);
        }
        VOID aktualizacja_windy() {
            if (!oczekujacy.empty()) czas_nieaktywnosci = 0;
            if (stan_windy == 0) { //zamknieta i na pietrze
                if (ilosc_ludzi == 0) {
                    if (oczekujacy.empty()) {
                        if (pietro_windy == 0) return;
                        if (czas_nieaktywnosci > 200) {
                            kierunek = false;
                            stan_windy = 4;
                            cel = 0;
                        }
                        else {
                            czas_nieaktywnosci++;
                        }
                    }
                    else {
                        //czy ktos jest na aktualnym piętrze kto chce jechać w tą samą strone co jedzie winda
                        //jeżelli tak to sie otwiera
                        //jeżeli nie to wybiera pierwszego człowieka w kolejce i jedzie do niego
                        bool czy_ktos_jest = false;
                        for(int i=0;i<ludzie_na_pietrach[pietro_windy].size();i++)
                            if (ludzie_na_pietrach[pietro_windy][i]->get_kierunek() == kierunek) {
                                czy_ktos_jest = true;
                                break;
                            }
                        if (!ludzie_na_pietrach[pietro_windy].empty() && ludzie_na_pietrach[pietro_windy][0] == oczekujacy[0]) czy_ktos_jest = true;
                        if (czy_ktos_jest) stan_windy = 1;
                        else {
                            cel = oczekujacy[0]->get_aktualne();
                            if (cel > pietro_windy) kierunek = true;
                            else kierunek = false;
                            stan_windy = 4;
                        }
                    }
                }
                else {
                    //czy ktos chce wejsc na aktualnym pietrze lub czy ktos z wind chce wyjsc, jak nie to jedzie dalej w tym samym kierunku
                    bool czy_sie_otworzy = false;
                    if (ilosc_ludzi != 8) {
                        for (int i = 0; i < ludzie_na_pietrach[pietro_windy].size(); i++)
                            if (ludzie_na_pietrach[pietro_windy][i]->get_kierunek() == kierunek) {
                                czy_sie_otworzy = true;
                                break;
                            }
                    }
                    for (int i = 0; i < ludzie_w_windzie.size(); i++) {
                        if (ludzie_w_windzie[i]!=NULL && ludzie_w_windzie[i]->get_docelowe() == pietro_windy) {
                            czy_sie_otworzy = true;
                            break;
                        }
                    }
                    if (czy_sie_otworzy) stan_windy = 1;
                    else stan_windy = 4;
                }
            }
            if (stan_windy == 1) { //otwieranie windy
                wys_drzwi += 4;
                if (wys_drzwi == 160) stan_windy = 2;
            }
            if (stan_windy == 2) { //otwarta
                int x_brzegu, delta_x,x_pietra;
                if (pietro_windy % 2 == 0) {
                    x_pietra = 545;
                    x_brzegu = -50;
                    delta_x = -45;
                }
                else {
                    x_pietra = 1011;
                    x_brzegu = 1600;
                    delta_x = 45;
                }
                //ludzie wychodzacy
                int ile_wyszlo = 0;
                for (int i = 0; i < ilosc_ludzi; i++) {
                    if (ludzie_w_windzie[i]->get_docelowe() == pietro_windy) {
                        wychodzacy.push_back(ludzie_w_windzie[i]);
                        ludzie_w_windzie[i]->inicjalizuj_ruch(620 + 45 * i,x_brzegu,pietro_windy);
                        ludzie_w_windzie[i] = NULL;
                        ile_wyszlo++;
                    }
                }
                ilosc_ludzi -= ile_wyszlo;
                //przestawianie ludzi w windzie
                std::queue<int> kolejka_indeksow;
                for (int i = 0; i < ludzie_w_windzie.size(); i++) {
                    if (ludzie_w_windzie[i] != NULL) kolejka_indeksow.push(i);
                }
                for (int i = 0; i < ilosc_ludzi; i++) {
                    if (ludzie_w_windzie[i] == NULL) {
                        int nastepny = kolejka_indeksow.front();
                        ludzie_w_windzie[nastepny]->inicjalizuj_ruch(620 + 45 * nastepny, 620 + 45 * i, pietro_windy);
                        poruszajacy_w_windzie.push_back(ludzie_w_windzie[nastepny]);
                        ludzie_w_windzie[i] = ludzie_w_windzie[nastepny];
                        ludzie_w_windzie[nastepny] = NULL;
                        kolejka_indeksow.pop();
                    }
                    else kolejka_indeksow.pop();
                }
                //wchodzenie ludzi i ustalenie kierunku
                for (int i = 0; i < ludzie_na_pietrach[pietro_windy].size(); i++) {
                    if (ilosc_ludzi == 8) break;
                    czlowiek* czlowiek_wchodzacy = ludzie_na_pietrach[pietro_windy][i];
                    if (czlowiek_wchodzacy == oczekujacy[0] && ilosc_ludzi == 0) kierunek = czlowiek_wchodzacy->get_kierunek();
                    czlowiek_wchodzacy->inicjalizuj_ruch(x_pietra + i*delta_x, 620 + 45 * ilosc_ludzi,pietro_windy);
                    wchodzacy.push_back(czlowiek_wchodzacy);
                    ludzie_w_windzie[ilosc_ludzi] = czlowiek_wchodzacy;
                    ludzie_na_pietrach[pietro_windy][i] = NULL;
                    ilosc_ludzi++;
                }
                bool czy_wszyscy_inny_kierunek = true;
                for (int i = 0; i < ilosc_ludzi; i++) {
                    if (ludzie_w_windzie[i]->get_kierunek() == kierunek) {
                        czy_wszyscy_inny_kierunek = false;
                        break;
                    }
                }
                if (czy_wszyscy_inny_kierunek) kierunek = !kierunek;
                //przestawianie ludzi na pietrach
                for (int i = 0; i < ludzie_na_pietrach[pietro_windy].size(); i++) {
                    if (ludzie_na_pietrach[pietro_windy][i] != NULL) kolejka_indeksow.push(i);
                }
                int miejsce = 0;
                while (!kolejka_indeksow.empty()) {
                    if (ludzie_na_pietrach[pietro_windy][miejsce] == NULL) {
                        int nastepny = kolejka_indeksow.front();
                        ludzie_na_pietrach[pietro_windy][nastepny]->inicjalizuj_ruch(x_pietra + nastepny * delta_x, x_pietra + miejsce * delta_x, pietro_windy);
                        poruszajacy_na_pietrze.push_back(ludzie_na_pietrach[pietro_windy][nastepny]);
                        ludzie_na_pietrach[pietro_windy][miejsce] = ludzie_na_pietrach[pietro_windy][nastepny];
                        ludzie_na_pietrach[pietro_windy][nastepny] = NULL;
                    }
                    miejsce++;
                    kolejka_indeksow.pop();
                }
                while (!ludzie_na_pietrach[pietro_windy].empty() && ludzie_na_pietrach[pietro_windy].back() == NULL) ludzie_na_pietrach[pietro_windy].pop_back();
                //usuniecie ludzi w windzie z kolejki
                for (int i = 0; i < ilosc_ludzi; i++) {
                    for (int j = 0; j < oczekujacy.size(); j++) {
                        if (oczekujacy[j] == ludzie_w_windzie[i]) {
                            oczekujacy[j] = NULL;
                            break;
                        }
                    }
                }
                std::vector<czlowiek*> nowe_oczekujacy;
                for (int i = 0; i < oczekujacy.size(); i++) {
                    if (oczekujacy[i] != NULL) nowe_oczekujacy.push_back(oczekujacy[i]);
                }
                oczekujacy = nowe_oczekujacy;
                //sprawdzanie czy ludzie skończli się poruszać
                bool czy_wszyscy_sa_poza_winda = true;
                for (int i = 0; i < wychodzacy.size(); i++) {
                    int x_wychodzacego = wychodzacy[i]->get_x();
                    if (x_wychodzacego > 600 && x_wychodzacego < 1010) {
                        czy_wszyscy_sa_poza_winda = false;
                        break;
                    }
                }
                if (czy_wszyscy_sa_poza_winda && wchodzacy.empty() && poruszajacy_w_windzie.empty()) stan_windy = 3;
            }
            if (stan_windy == 3) { //zamykanie
                wys_drzwi -= 4;
                if (ilosc_ludzi != 8 && !ludzie_na_pietrach[pietro_windy].empty()) {
                    for (int i = 0; i < ludzie_na_pietrach[pietro_windy].size(); i++)
                        if (ludzie_na_pietrach[pietro_windy][i]->get_kierunek() == kierunek)
                            stan_windy = 1;
                }
                if (wys_drzwi == 0) stan_windy = 0;
            }
            if (stan_windy == 4) { //winda porusza sie
                int kierunek_int;
                if (kierunek) kierunek_int = -1;
                else kierunek_int = 1;
                y_windy += 6 * kierunek_int;
                int nastepnePietro = pietro_windy - kierunek_int;
                if (y_windy == yWindyNaPietrach[nastepnePietro]) {
                    pietro_windy = nastepnePietro;
                    stan_windy = 0;
                }
            }
        }
        VOID aktualizacja_poruszajacych() {
            std::vector<czlowiek*> pomocniczy;
            for (int i = 0; i < wychodzacy.size(); i++) {
                wychodzacy[i]->ruch();
                if (!wychodzacy[i]->czy_sie_porusza()) {
                    delete wychodzacy[i];
                    wychodzacy[i] = NULL;
                }
                else pomocniczy.push_back(wychodzacy[i]);
            }
            wychodzacy = pomocniczy;
            pomocniczy.clear();
            for (int i = 0; i < wchodzacy.size(); i++) {
                wchodzacy[i]->ruch();
                if (!wchodzacy[i]->czy_sie_porusza()) wchodzacy[i] = NULL;
                else pomocniczy.push_back(wchodzacy[i]);
            }
            wchodzacy = pomocniczy;
            pomocniczy.clear();
            for (int i = 0; i < poruszajacy_na_pietrze.size(); i++) {
                poruszajacy_na_pietrze[i]->ruch();
                if (!poruszajacy_na_pietrze[i]->czy_sie_porusza()) poruszajacy_na_pietrze[i] = NULL;
                else pomocniczy.push_back(poruszajacy_na_pietrze[i]);
            }
            poruszajacy_na_pietrze = pomocniczy;
            pomocniczy.clear();
            for (int i = 0; i < poruszajacy_w_windzie.size(); i++) {
                poruszajacy_w_windzie[i]->ruch();
                if (!poruszajacy_w_windzie[i]->czy_sie_porusza()) poruszajacy_w_windzie[i] = NULL;
                else pomocniczy.push_back(poruszajacy_w_windzie[i]);
            }
            poruszajacy_w_windzie = pomocniczy;
        }
};
winda_klassa winda;
VOID RysujHud(Graphics &gBuffer) {
    //Podpis
    FontFamily  fontFamily(L"Times New Roman");
    Font        font(&fontFamily, 12, FontStyleRegular, UnitPixel);
    PointF      pointF(1060, 945);
    SolidBrush  solidBrush(Color(255, 0, 0, 0));
    gBuffer.DrawString(L"Techniki Programowania - projekt 4 - Zadanie 3 (Winda) - Filip Skudlarz 203314, Aleksander Kargol 203225", -1, &font, pointF, &solidBrush);

    //Szyb Windy
    Pen blackPen(Color(255, 0, 0, 0), 5);
    gBuffer.DrawRectangle(&blackPen, 600, 25, 400, 900);

    //Piętra
    Pen bluePen(Color(255, 0, 0, 255), 3);
    gBuffer.DrawLine(&bluePen, 0, 911, 598, 911); //Pietro 0
    gBuffer.DrawLine(&bluePen, 1002, 737, 1600, 737); //Pietro 1
    gBuffer.DrawLine(&bluePen, 0, 563, 598, 563); //Pietro 2
    gBuffer.DrawLine(&bluePen, 1002, 389, 1600, 389); //Pietro 3
    gBuffer.DrawLine(&bluePen, 0, 215, 598, 215); //Pietro 4
}
VOID wyswietl_ludzi_na_pietrach(Graphics& gBuffer) {
    int x,delta_x;
    for (int pietro = 0; pietro < 5; pietro++) {
        for (int i = 0; i < ludzie_na_pietrach[pietro].size(); i++) {
            ludzie_na_pietrach[pietro][i]->wyswietl_na_pietrze(gBuffer, i);
        }
    }
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int id;
    czlowiek* temp = NULL;
    switch (message)
    {
    case WM_CREATE:
        ludzie_na_pietrach.resize(5);
        SetTimer(hWnd, 1, 25, NULL);  //Timer co 25 ms wiec 40 fps
        return 0;

    case WM_TIMER:
        winda.aktualizacja_windy();
        winda.aktualizacja_poruszajacych();
        InvalidateRect(hWnd, NULL, TRUE);  //Odświeża okno
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT rc;
        GetClientRect(hWnd, &rc);
        int width = rc.right - rc.left; 
        int height = rc.bottom - rc.top; 
        Bitmap buffer(width, height, PixelFormat32bppARGB); //tworzenie białego buffora 
        Graphics gBuffer(&buffer);
        gBuffer.Clear(Color(255, 255, 255, 255));
       
        RysujHud(gBuffer);
        wyswietl_ludzi_na_pietrach(gBuffer);
        winda.wyswietl_winde(gBuffer);

        Graphics gScreen(hdc); 
        gScreen.DrawImage(&buffer, 0, 0); //kopiowanie buffora na ekran

        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        for (int i = 0; i < ludzie_na_pietrach.size(); i++) {
            for (int j = 0; j < ludzie_na_pietrach[i].size(); j++) {
                delete ludzie_na_pietrach[i][j];
            }
        }
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        return 0;
    case WM_COMMAND:
        id = LOWORD(wParam);
        temp = new czlowiek(id / 10, id % 10);
        ludzie_na_pietrach[id / 10].push_back(temp);
        oczekujacy.push_back(temp);
        temp = NULL;
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("Winda");
    wc.hbrBackground = NULL;
    RegisterClass(&wc);

    HWND hWnd = CreateWindow(wc.lpszClassName, TEXT("Winda - Filip i Aleksander"),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN, 0, 0, 1616, 999,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    HWND przyciski[5][5];
    for (int pietro = 0; pietro < 5; pietro++) {
        int x;
        if (pietro % 2 == 0) x = 10;
        else x = 1550;
        for (int cel = 0; cel < 5; cel++) {
            if (pietro == cel) continue;
            char buffer[16];
            sprintf_s(buffer, "%d", cel);
            przyciski[pietro][cel] = CreateWindowA("BUTTON", buffer, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                x, y_pietr[pietro] - 50 - 43 * cel, 40, 40, hWnd, (HMENU)(10 * pietro + cel), hInstance, NULL);
        }
    }
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return 0;
}
