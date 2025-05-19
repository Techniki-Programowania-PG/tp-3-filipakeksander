#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <matplot/matplot.h>
#include <vector>
#include <cmath>

std::vector<std::vector<double> > generuj_sin(double czestotliwosc,int ilosc_punktow, double x_min, double x_maks, double przesun_faz){
    std::vector<std::vector<double> >sygnal;
    sygnal.resize(2);
    double step = (x_maks - x_min) / (ilosc_punktow - 1);
    for(int i=0;i<ilosc_punktow;i++){
        sygnal[0].push_back(x_min + i * step);
        sygnal[1].push_back(sin(czestotliwosc * sygnal[0][i]+przesun_faz));
    }
    return sygnal;
}

std::vector<std::vector<double> > generuj_cos(double czestotliwosc,int ilosc_punktow, double x_min, double x_maks, double przesun_faz){
    return generuj_sin(czestotliwosc, ilosc_punktow, x_min, x_maks, przesun_faz + 1,57);
}

std::vector<std::vector<double> > generuj_prostokatny(double czestotliwosc, int wypelnienie, int ilosc_punktow, double x_min, double x_maks){
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
            sygnal[1].push_back(-1);
        }
        else{
            sygnal[1].push_back(1);
        }
    }
    return sygnal;
}

void wyswietl(std::vector<double> x,std::vector<double> y){
    using namespace matplot;
    plot(x, y)->color({0.f, 0.7f, 0.9f});
    xlim({x[0],x.back()});
    ylim({-2, +2});
    show();
}
namespace py = pybind11;

PYBIND11_MODULE(_core, k) {
    k.def("generuj_sin",&generuj_sin);
    k.def("wyswietl",&wyswietl);
    k.def("generuj_prostokatny", &generuj_prostokatny);
}
