#ifndef TIRACORTE_H
#define TIRACORTE_H

class TiraCorte {
public:
    double y_base;       // Coordenada Y donde inicia la base de esta repisa
    double h_max;        // Altura máxima de la tira (dictada por la primera pieza)
    double ancho_usado;  // Espacio horizontal (eje X) que ya ha sido ocupado

    // Constructor
    TiraCorte(double base_y, double altura_max, double ancho_inicial);
};

#endif