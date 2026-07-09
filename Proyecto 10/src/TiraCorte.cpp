#include "TiraCorte.h"

using namespace std;

// Implementación del constructor
TiraCorte::TiraCorte(double base_y, double altura_max, double ancho_inicial) {
    y_base = base_y;
    h_max = altura_max;
    ancho_usado = ancho_inicial;
}