#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <matplot/matplot.h>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <complex>
#include <ctime>
const double pi = 3.141592653589;
std::vector<std::vector<double> > generuj_sin(const double czestotliwosc,const double amplituda,const int ilosc_punktow, const double x_min, const double x_maks, const double przesun_faz){
    std::vector<std::vector<double> >sygnal;
    sygnal.resize(2);
    double step = (x_maks - x_min) / (ilosc_punktow - 1);
    for(int i=0;i<ilosc_punktow;i++){
        sygnal[0].push_back(x_min + i * step);
        sygnal[1].push_back(amplituda * sin(2 * pi * czestotliwosc * sygnal[0][i] + przesun_faz));
    }
    return sygnal;
}
std::vector<std::vector<double> > generuj_cos(const double czestotliwosc,const double amplituda,const int ilosc_punktow, const double x_min, const double x_maks, const double przesun_faz){
    return generuj_sin(czestotliwosc, amplituda, ilosc_punktow, x_min, x_maks, przesun_faz + pi/2);
}
std::vector<std::vector<double> > generuj_prostokatny(const double czestotliwosc, const double amplituda, const int ilosc_punktow, const int wypelnienie, const double x_min, const double x_maks){
    std::vector<std::vector<double> >sygnal;
    sygnal.resize(2);
    double step = (x_maks - x_min) / (ilosc_punktow - 1);
    double okres = 1.0/czestotliwosc;
    double dol_zak = x_min;
    double gor_zak = x_min + (1-wypelnienie / 100.0) * okres;
    for(int i=0;i<ilosc_punktow;i++){
        sygnal[0].push_back(x_min + i * step);
        if(sygnal[0][i] > dol_zak + okres){
            dol_zak+=okres;
            gor_zak+=okres;
        }
        if(sygnal[0][i] <= gor_zak){
            sygnal[1].push_back(-amplituda);
        }
        else{
            sygnal[1].push_back(amplituda);
        }
    }
    return sygnal;
}

std::vector<std::vector<double> > generuj_piloksztaltny(const double czestotliwosc, const double amplituda, const int ilosc_punktow, const double x_min, const double x_maks){
    std::vector<std::vector<double> >sygnal;
    sygnal.resize(2);
    double step = (x_maks - x_min) / (ilosc_punktow - 1);
    double okres = 1.0/czestotliwosc;
    double ilosc_w_okresie = okres/step;
    double wartosc = -1;
    for(int i=0;i<ilosc_punktow;i++){
        sygnal[0].push_back(x_min + i * step);
        sygnal[1].push_back(amplituda * wartosc);
        wartosc += 2/ilosc_w_okresie;
        if(wartosc > 1){
            wartosc = -1;
        }
    }
    return sygnal;
}
std::vector<double> wartosc_bezwzgledna(const std::vector<std::complex<double> > x){
    std::vector<double> wynik;
    for(int i=0;i<x.size();i++){
        wynik.push_back(sqrt(x[i].imag() * x[i].imag() + x[i].real() * x[i].real()));
    }
    return wynik;
}
std::vector<std::complex<double> > dft(const std::vector<double> x){
    std::vector<std::complex<double> > transformata;
    for(int k=0;k<x.size();k++){
        std::complex<double> suma(0,0);
        for(int n=0;n<x.size();n++){
            double kat = -2 * pi * k * n / x.size();
            std::complex<double> wynik(cos(kat),sin(kat));
            suma += x[n] * wynik;
        }
        transformata.push_back(suma);
    }
    return transformata;
}
std::vector<double> idtf(const std::vector<std::complex<double> > transformata){
    std::vector<double> x;
    for(int n=0;n<transformata.size();n++){
        std::complex<double> suma(0,0);
        for(int k=0;k<transformata.size();k++){
            double kat = 2 * pi * k * n / transformata.size();
            std::complex<double> wynik(cos(kat),sin(kat));
            suma += transformata[k] * wynik;
        }
        x.push_back(suma.real()/transformata.size());
    }
    return x;
}
void wyswietl_2D(const std::vector<double> x, const std::vector<double> y, const std::string nazwa, const std::string y_label, const std::string x_label,const std::string nazwa_pliku){
    using namespace matplot;
    auto fig = figure();
    plot(x,y)->line_width(3).color("red");
    double ymaks = y[0], ymin = y[0],xmin = x[0], xmaks = x[0];
    for(int i=0;i<y.size();i++){
        if(y[i]>ymaks) ymaks = y[i];
        if(y[i]<ymin) ymin = y[i];
        if(x[i]>xmaks) xmaks = x[i];
        if(x[i]<xmin) xmin = x[i];
    }
    ylim({ymin - 1, ymaks + 1});
    xlim({xmin,xmaks});
    title(nazwa);
    xlabel(x_label);
    ylabel(y_label);
    grid(on);
    if(nazwa_pliku == "") show();
    else{
        fig->size(600, 420);
        save(nazwa_pliku);
    }
}
void wyswietl_1D(const std::vector<double> y, const std::string nazwa, const std::string y_label,const std::string nazwa_pliku){
    std::vector<double> x(y.size());
    for(int i=0;i<x.size();i++) x[i] = i;
    wyswietl_2D(x,y,nazwa,y_label,"",nazwa_pliku);
}
void wyswietl_amplitude_dft(const std::vector<std::complex<double> > transformata, const double czestotliwosc_probkowania, const bool czy_tylko_dodatnie, const std::string nazwa,const std::string nazwa_pliku){
    double d = transformata.size() / czestotliwosc_probkowania;
    std::vector<double> x,y;
    std::vector<double> modul = wartosc_bezwzgledna(transformata);
    int srodek;
    if(transformata.size() % 2 == 0) srodek = transformata.size()/2 - 1;
    else srodek = (transformata.size() - 1)/2;
    for(int i=0;i<=srodek;i++){
        x.push_back(i/d);
        double wartosc = modul[i] / czestotliwosc_probkowania;
        if(czy_tylko_dodatnie) wartosc*=2;
        y.push_back(wartosc);
    }
    if(!czy_tylko_dodatnie){
        if(transformata.size() % 2 == 0) srodek++;
        int a = srodek;
        srodek *= -1;
        while(srodek <= -1){
            x.push_back(srodek/d);
            double wartosc = modul[a] / czestotliwosc_probkowania;
            y.push_back(wartosc);
            srodek++;
            a++;
        }
    }
    wyswietl_2D(x,y,nazwa,"Amplituda","Czestotliwosc [Hz]",nazwa_pliku);
}
void wyswietl_img(const std::vector<std::vector<double>> img,const std::string nazwa,const std::string nazwa_pliku){
    using namespace matplot;
    auto fig = figure();
    imagesc(img);
    colormap(palette::gray());
    title(nazwa);
    fig->size(600, 420);
    if(nazwa_pliku == "") show();
    else{
        fig->size(600, 420);
        save(nazwa_pliku);
    }
}
std::vector<double> zaszum(const std::vector<double> sygnal,const double moc_zaszumiania){
    std::vector<double> wynik;
    double max = sygnal[0],min = sygnal[0];
    for(int i=1;i<sygnal.size();i++){
        if(sygnal[i] > max) max = sygnal[i];
        if(sygnal[i] < min) min = sygnal[i];
    }
    double max_zaszumienie = (max - min) * (moc_zaszumiania / 100);
    std::srand(std::time(NULL));
    for(int i=0;i<sygnal.size();i++){
        double z = (std::rand() *1.0) / RAND_MAX * max_zaszumienie;
        if(std::rand()%2==0) z*=-1;
        wynik.push_back(sygnal[i] + z);
    }
    return wynik;
}
std::vector<double> filtracja1D(const std::vector<double> sygnal,const std::vector<double> filtr){
    std::vector<double> wynik;
    for(int i=0;i<sygnal.size();i++){
        double suma = 0;
        for(int j=0;j<filtr.size();j++){
            int nowe_i = i + j - (filtr.size()/2);
            if(nowe_i >= sygnal.size()) nowe_i = sygnal.size() - 1;
            if(nowe_i < 0) nowe_i = 0;
            suma += sygnal[nowe_i] * filtr[j];
        }
        wynik.push_back(suma);
    }
    return wynik;
}
std::vector<std::vector<double>> filtracja2D(const std::vector<std::vector<double>> sygnal, const std::vector<std::vector<double>> filtr){
    int wys_syg = sygnal.size();
    int szer_syg = sygnal[0].size();
    int filtr_wys = filtr.size();
    int filtr_szer = filtr[0].size();
    int filtr_srodek_x = filtr_szer / 2;
    int filtr_srodek_y = filtr_wys / 2;
    std::vector<std::vector<double>> wynik(wys_syg, std::vector<double>(szer_syg,0));
    for(int i=0;i<wys_syg;i++){
        for(int j=0;j<szer_syg;j++){
            double suma = 0;
            for(int m=0;m<filtr_wys;m++){
                for(int n=0;n<filtr_szer;n++){
                    int y = i + m - filtr_srodek_y;
                    int x = j + n - filtr_srodek_x;
                    if(x<0) x=0;
                    if(y<0) y=0;
                    if(x>=szer_syg) x = szer_syg - 1;
                    if(y>=wys_syg) y = wys_syg - 1;
                    suma += sygnal[y][x] * filtr[m][n];
                }
            }
            wynik[i][j] = suma;
        }
    }
    return wynik;
}
namespace py = pybind11;

PYBIND11_MODULE(_core, k) {
    k.def("generuj_sin",&generuj_sin);
    k.def("generuj_cos",&generuj_cos);
    k.def("generuj_prostokatny", &generuj_prostokatny);
    k.def("generuj_piloksztaltny", &generuj_piloksztaltny);

    k.def("wyswietl_2D",&wyswietl_2D);
    k.def("wyswietl_1D",&wyswietl_1D);
    k.def("wyswietl_amplitude_dft",&wyswietl_amplitude_dft);
    k.def("wyswietl_img",&wyswietl_img);

    k.def("dft",&dft);
    k.def("idft",&idtf);
    k.def("wartosc_bezwzgledna",&wartosc_bezwzgledna);
    
    k.def("zaszum",&zaszum);

    k.def("filtracja1D",&filtracja1D);
    k.def("filtracja2D",&filtracja2D);
}
