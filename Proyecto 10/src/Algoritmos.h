#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include <vector>
#include <algorithm> // Necesario para usar std::sort en nuestra heurística
#include "Pieza.h"
#include "Plancha.h"

using namespace std;

// =========================================================
// ALGORITMO 2: HEURÍSTICA FFDH (O(n log n))
// =========================================================
vector<Plancha> FFDH_Corte(vector<Pieza> piezas, double W, double H);


// =========================================================
// ALGORITMO 1: MÉTODO EXACTO (Backtracking O(n!))
// =========================================================
// Función "Wrapper" (Envoltorio) para llamar desde el main
vector<Plancha> Ejecutar_Backtracking(vector<Pieza> piezas, double W, double H);

// Función recursiva pura - El motor matemático
void Backtracking_Corte(vector<Pieza>& piezas, int indice, double W, double H, 
                        vector<Plancha>& planchas_actuales, 
                        vector<Plancha>& mejor_solucion, 
                        int& min_planchas);

#endif