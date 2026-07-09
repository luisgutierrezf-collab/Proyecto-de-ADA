#include "Algoritmos.h"

using namespace std;

// =========================================================
// IMPLEMENTACIÓN: HEURÍSTICA NFDH (Next Fit Decreasing Height)
// Optimizada a O(n) para volúmenes industriales masivos
// =========================================================
vector<Plancha> FFDH_Corte(vector<Pieza> piezas, double W, double H) {
    
    // 1. Preprocesamiento: Ordenar por altura descendente O(n log n)
    sort(piezas.begin(), piezas.end(), [](const Pieza& a, const Pieza& b) {
        if (a.h != b.h) return a.h > b.h;
        return a.w > b.w; // Desempate por anchura
    });

    vector<Plancha> lista_planchas;
    lista_planchas.reserve(piezas.size()); // Prevenir realojamiento de RAM

    // 2. Empaquetamiento (NFDH - Solo revisa la última plancha abierta)
    for (size_t i = 0; i < piezas.size(); i++) {
        Pieza& pieza = piezas[i];
        bool ubicada = false;

        // Si ya hay al menos una plancha, intentamos meter la pieza SOLO en ESA ÚLTIMA plancha
        if (!lista_planchas.empty()) {
            Plancha& plancha = lista_planchas.back(); // .back() obtiene solo la última
            
            // Fase A: Intentar ubicar en tiras existentes de la última plancha
            for (size_t k = 0; k < plancha.tiras.size(); k++) {
                TiraCorte& tira = plancha.tiras[k];
                if (tira.ancho_usado + pieza.w <= W && pieza.h <= tira.h_max) {
                    pieza.x = tira.ancho_usado;
                    pieza.y = tira.y_base;
                    tira.ancho_usado += pieza.w;
                    plancha.AgregarPieza(pieza);
                    ubicada = true;
                    break;
                }
            }

            // Fase B: Crear nueva tira en esa misma última plancha
            if (!ubicada && plancha.altura_usada + pieza.h <= H) {
                TiraCorte nueva_tira(plancha.altura_usada, pieza.h, pieza.w);
                pieza.x = 0;
                pieza.y = nueva_tira.y_base;
                
                plancha.tiras.push_back(nueva_tira);
                plancha.altura_usada += pieza.h;
                plancha.AgregarPieza(pieza);
                ubicada = true;
            }
        }

        // Fase C: Abrir nueva plancha maestra (porque no cupo en la última o no había ninguna)
        if (!ubicada) {
            Plancha nueva_plancha(W, H);
            TiraCorte nueva_tira(0, pieza.h, pieza.w);
            
            pieza.x = 0;
            pieza.y = 0;
            
            nueva_plancha.tiras.push_back(nueva_tira);
            nueva_plancha.altura_usada = pieza.h;
            nueva_plancha.AgregarPieza(pieza);
            
            lista_planchas.push_back(nueva_plancha);
        }
    }
    return lista_planchas;
}

// =========================================================
// IMPLEMENTACIÓN: BACKTRACKING EXACTO
// =========================================================

void Backtracking_Corte(vector<Pieza>& piezas, int indice, double W, double H, 
                        vector<Plancha>& estado_actual, 
                        vector<Plancha>& mejor_solucion, 
                        int& min_planchas) {
                            
    // PODA: Si ya abrimos más planchas que nuestro mejor récord, cancelamos esta rama
    if (estado_actual.size() >= (size_t)min_planchas) return;

    // CASO BASE: Ya ubicamos todas las piezas
    if (indice == piezas.size()) {
        if (estado_actual.size() < (size_t)min_planchas) {
            min_planchas = estado_actual.size();
            mejor_solucion = estado_actual; // Guardamos el nuevo récord
        }
        return;
    }

    Pieza pieza_actual = piezas[indice];

    // RAMIFICACIÓN 1: Intentar en planchas existentes escaneando coordenadas
    for (size_t i = 0; i < estado_actual.size(); i++) {
        Plancha& plancha = estado_actual[i];
        
        // Escaneamos píxel por píxel (incremento de 1.0)
        for (double x = 0; x <= W - pieza_actual.w; x += 20.0) {
            for (double y = 0; y <= H - pieza_actual.h; y += 20.0) {
                
                if (plancha.ValidarNoSuperposicion(x, y, pieza_actual.w, pieza_actual.h)) {
                    // Aplicar jugada
                    plancha.AgregarPieza(pieza_actual, x, y);
                    
                    // Viajar al futuro (Recursión)
                    Backtracking_Corte(piezas, indice + 1, W, H, estado_actual, mejor_solucion, min_planchas);
                    
                    // Retroceder en el tiempo (Backtrack)
                    plancha.QuitarUltimaPieza();
                }
            }
        }
    }

    // RAMIFICACIÓN 2: Sacar una plancha nueva del almacén
    Plancha nueva_plancha(W, H);
    nueva_plancha.AgregarPieza(pieza_actual, 0, 0);
    estado_actual.push_back(nueva_plancha);
    
    // Viajar al futuro
    Backtracking_Corte(piezas, indice + 1, W, H, estado_actual, mejor_solucion, min_planchas);
    
    // Retroceder en el tiempo
    estado_actual.pop_back();
}

// Función Wrapper para inicializar todo limpiamente
vector<Plancha> Ejecutar_Backtracking(vector<Pieza> piezas, double W, double H) {
    vector<Plancha> estado_inicial;
    vector<Plancha> mejor_solucion;
    int min_planchas = piezas.size() + 1; // Peor caso absoluto
    
    Backtracking_Corte(piezas, 0, W, H, estado_inicial, mejor_solucion, min_planchas);
    return mejor_solucion;
}