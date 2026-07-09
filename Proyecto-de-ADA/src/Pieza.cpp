#include "Pieza.h"

using namespace std;

// Implementación del constructor
Pieza::Pieza(int id_pieza, double ancho, double alto) {
    id = id_pieza;
    w = ancho;
    h = alto;
    
    // Al crearse, la pieza aún no tiene coordenadas definidas en la plancha
    x = 0.0;
    y = 0.0;
    
    // Por defecto, una pieza nueva no está ubicada
    ubicada = false;
}