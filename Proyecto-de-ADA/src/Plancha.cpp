#include "Plancha.h"

using namespace std;

// Constructor: Inicializa las dimensiones de la plancha y la altura usada
Plancha::Plancha(double ancho_max, double alto_max) {
    W = ancho_max;
    H = alto_max;
    altura_usada = 0.0; // Al principio, la plancha está completamente vacía
}

// Sobrecarga 1: Agregar pieza especificando coordenadas (Usado por Backtracking)
void Plancha::AgregarPieza(Pieza p, double pos_x, double pos_y) {
    p.x = pos_x;
    p.y = pos_y;
    p.ubicada = true;
    piezas_cortadas.push_back(p);
}

// Sobrecarga 2: Agregar pieza con coordenadas ya calculadas (Usado por FFDH)
void Plancha::AgregarPieza(Pieza p) {
    piezas_cortadas.push_back(p);
}

// Método de descarte (Usado por Backtracking al retroceder)
void Plancha::QuitarUltimaPieza() {
    if (!piezas_cortadas.empty()) {
        piezas_cortadas.pop_back(); // Elimina el último elemento del vector
    }
}

// Método fundamental para garantizar las restricciones espaciales
bool Plancha::ValidarNoSuperposicion(double eval_x, double eval_y, double eval_w, double eval_h) {
    // 1. Restricción de Límites Espaciales (Contención en los Bordes)
    if (eval_x < 0 || eval_y < 0 || (eval_x + eval_w > W) || (eval_y + eval_h > H)) {
        return false; // La pieza excede los límites físicos
    }

    // 2. Restricción de No Superposición Geométrica
    for (size_t i = 0; i < piezas_cortadas.size(); i++) {
        Pieza existente = piezas_cortadas[i];
        
        // Para que NO haya superposición, debe cumplirse obligatoriamente 
        // al menos UNA de las cuatro condiciones de separación:
        bool separacion_izquierda = (eval_x + eval_w <= existente.x);
        bool separacion_derecha   = (existente.x + existente.w <= eval_x);
        bool separacion_abajo     = (eval_y + eval_h <= existente.y);
        bool separacion_arriba    = (existente.y + existente.h <= eval_y);
        
        // Si ninguna de las 4 condiciones se cumple, significa que las piezas colisionan
        if (!(separacion_izquierda || separacion_derecha || separacion_abajo || separacion_arriba)) {
            return false; // Hay superposición, posición inválida
        }
    }
    return true;
}