#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <matplot/matplot.h>
#include <vector>
#include <cmath>
std::vector<std::vector<double> > generuj_sin(double czestotliwosc,int ilosc_punktow, double x_min, double x_maks, double przesuniecie){
    std::vector<std::vector<double> >sygnal;
    sygnal.resize(2);
    double step = (x_maks - x_min) / (ilosc_punktow - 1);
    for(int i=0;i<ilosc_punktow;i++){
        sygnal[0].push_back(x_min + i * step);
        sygnal[1].push_back(sin(czestotliwosc * sygnal[0][i]));
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
}
