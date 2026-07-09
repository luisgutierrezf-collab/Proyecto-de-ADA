#ifndef PLANCHA_H
#define PLANCHA_H

#include <vector>
#include "Pieza.h"
#include "TiraCorte.h"

using namespace std;

class Plancha {
public:
    double W; // Ancho máximo comercial (120)
    double H; // Alto máximo comercial (240)
    
    // Almacenamiento dinámico nativo de C++
    vector<Pieza> piezas_cortadas;
    vector<TiraCorte> tiras;       
    
    // Control del espacio vertical consumido (Usado en Fase B del FFDH)
    double altura_usada;

    // Constructor
    Plancha(double ancho_max, double alto_max);

    // Método matemático fundamental: Restricciones de exclusión mutua espacial
    bool ValidarNoSuperposicion(double eval_x, double eval_y, double eval_w, double eval_h);

    // Sobrecarga de métodos para soportar los dos algoritmos con firmas distintas
    void AgregarPieza(Pieza p, double pos_x, double pos_y); // Usado por el Backtracking
    void AgregarPieza(Pieza p);                             // Usado por el FFDH
    
    // Método vital para la "poda" recursiva
    void QuitarUltimaPieza();
};

#endif