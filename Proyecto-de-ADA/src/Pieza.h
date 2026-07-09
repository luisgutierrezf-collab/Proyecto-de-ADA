#ifndef PIEZA_H
#define PIEZA_H

class Pieza {
public:
    int id;          // Identificador único de la pieza
    double w;        // Ancho de la pieza (wi)
    double h;        // Alto de la pieza (hi)
    double x;        // Coordenada X asignada
    double y;        // Coordenada Y asignada
    bool ubicada;    // Bandera para saber si ya fue posicionada en una plancha

    // Constructor
    Pieza(int id_pieza, double ancho, double alto);
};

#endif