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
void wyswietl_2D(const std::vector<double> x, const std::vector<double> y, const std::string nazwa, const std::string y_label, const std::string x_label){
    using namespace matplot;
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
    show();
}
void wyswietl_1D(const std::vector<double> y, const std::string nazwa, const std::string y_label){
    std::vector<double> x(y.size());
    for(int i=0;i<x.size();i++) x[i] = i;
    wyswietl_2D(x,y,nazwa,y_label,"");
}
void wyswietl_dft(const std::vector<std::complex<double> > transformata, const double czestotliwosc_probkowania, const bool czy_tylko_dodatnie, const std::string nazwa, const std::string y_label){
    double d = transformata.size() / czestotliwosc_probkowania;
    std::vector<double> x,y;
    int srodek;
    if(transformata.size() % 2 == 0) srodek = transformata.size()/2 - 1;
    else srodek = (transformata.size() - 1)/2;
    for(int i=0;i<=srodek;i++){
        x.push_back(i/d);
        y.push_back(2 * ( sqrt(transformata[i].imag()*transformata[i].imag() + transformata[i].real()*transformata[i].real()) / transformata.size() ) );
    }
    if(!czy_tylko_dodatnie){
        if(transformata.size() % 2 == 0) srodek++;
        int a = srodek;
        srodek *= -1;
        while(srodek <= -1){
            x.push_back(srodek/d);
             y.push_back(2 * ( sqrt(transformata[a].imag()*transformata[a].imag() + transformata[a].real()*transformata[a].real()) / transformata.size() ) );
            srodek++;
            a++;
        }
    }
    wyswietl_2D(x,y,nazwa,y_label,"Czestotliwosc [Hz]");
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
namespace py = pybind11;

PYBIND11_MODULE(_core, k) {
    k.def("generuj_sin",&generuj_sin);
    k.def("generuj_cos",&generuj_cos);
    k.def("generuj_prostokatny", &generuj_prostokatny);
    k.def("generuj_piloksztaltny", &generuj_piloksztaltny);

    k.def("wyswietl_2D",&wyswietl_2D);
    k.def("wyswietl_1D",&wyswietl_1D);
    k.def("wyswietl_dft",&wyswietl_dft);

    k.def("dft",&dft);
    k.def("idft",&idtf);
    k.def("wartosc_bezwzgledna",&wartosc_bezwzgledna);
    
    k.def("zaszum",&zaszum);
}
