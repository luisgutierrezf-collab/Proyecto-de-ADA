#include <iostream>
#include <vector>
#include <chrono> 
#include <random> 
#include <stdexcept>
#include <algorithm> // Necesario para ordenar los datos
#include "Algoritmos.h"

using namespace std;
using namespace std::chrono;

// =========================================================
// FUNCIÓN AUXILIAR: GENERADOR DE CASOS DE PRUEBA (ACTUALIZADO)
// =========================================================
vector<Pieza> GenerarDataset(long long n, double max_w, double max_h, int naturaleza_datos) {
    vector<Pieza> dataset;
    dataset.reserve(n); 

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis_w(10.0, max_w);
    uniform_real_distribution<> dis_h(10.0, max_h);

    // 1. Generación base (Aleatoria)
    for (long long i = 0; i < n; i++) {
        dataset.push_back(Pieza(i + 1, dis_w(gen), dis_h(gen)));
    }

    // 2. Modificación según la "Naturaleza de los Datos" (Rúbrica 5.a)
    if (naturaleza_datos == 2) {
        // Ordenado (Área ascendente - Peor caso teórico para empaquetamiento)
        sort(dataset.begin(), dataset.end(), [](const Pieza& a, const Pieza& b) {
            return (a.w * a.h) < (b.w * b.h);
        });
    } 
    else if (naturaleza_datos == 3) {
        // Ordenado Inversamente (Área descendente - Mejor caso)
        sort(dataset.begin(), dataset.end(), [](const Pieza& a, const Pieza& b) {
            return (a.w * a.h) > (b.w * b.h);
        });
    }
    // Si es 1 (Aleatorio), se queda como se generó en el bucle for.

    return dataset;
}

// =========================================================
// FUNCIÓN PRINCIPAL Y MENÚ INTERACTIVO
// =========================================================
int main() {
    const double W = 120.0;
    const double H = 240.0;
    int opcion_tamano;
    int opcion_naturaleza;

    do {
        cout << "\n======================================================\n";
        cout << "   OPTIMIZADOR DE CORTE 2D (Cutting Stock Problem)    \n";
        cout << "======================================================\n";
        cout << "1. Caso Pequeno (n = 10) -> Evaluar Backtracking vs Heuristica\n";
        cout << "2. Caso Mediano (n = 10^3) -> Evaluar Heuristica\n";
        cout << "3. Caso Grande  (n = 10^6) -> Evaluar Heuristica\n";
        cout << "4. Caso Extremo (n = 10^10) -> Prueba de Estres (RAM)\n";
        cout << "5. Salir\n";
        cout << "Seleccione el TAMANO del caso de prueba: ";
        cin >> opcion_tamano;

        if (opcion_tamano == 5) {
            break;
        }
        if (opcion_tamano < 1 || opcion_tamano > 4) {
            cout << "Opcion invalida.\n";
            continue;
        }

        // --- NUEVO MENÚ INTERACTIVO: NATURALEZA DE LOS DATOS ---
        cout << "\n--- NATURALEZA DE LOS DATOS ---\n";
        cout << "1. Aleatorios (Generacion estandar)\n";
        cout << "2. Ordenados (Area ascendente - Peor caso)\n";
        cout << "3. Inversamente Ordenados (Area descendente - Mejor caso)\n";
        cout << "Seleccione la NATURALEZA de las piezas: ";
        cin >> opcion_naturaleza;

        // Validación básica por si el usuario teclea un número incorrecto
        if (opcion_naturaleza < 1 || opcion_naturaleza > 3) {
            cout << "Opcion invalida. Se usaran datos aleatorios por defecto.\n";
            opcion_naturaleza = 1;
        }

        // Asignación de N
        long long n = 0;
        if (opcion_tamano == 1) n = 10;
        else if (opcion_tamano == 2) n = 1000;
        else if (opcion_tamano == 3) n = 1000000;
        else if (opcion_tamano == 4) n = 10000000000LL;

        cout << "\nGenerando dataset de " << n << " piezas...\n";
        
        try {
            // AQUI SE PASA LA VARIABLE AL GENERADOR:
            vector<Pieza> piezas = GenerarDataset(n, W, H, opcion_naturaleza);
            cout << "Dataset generado exitosamente.\n\n";

            // --- EJECUCIÓN DEL BACKTRACKING (Solo para n=10) ---
            if (opcion_tamano == 1) {
                cout << "--- Ejecutando METODO EXACTO (Backtracking) ---\n";
                auto start_bt = high_resolution_clock::now();
                
                vector<Plancha> sol_bt = Ejecutar_Backtracking(piezas, W, H);
                
                auto end_bt = high_resolution_clock::now();
                auto duration_bt = duration_cast<milliseconds>(end_bt - start_bt);
                
                cout << "> Planchas utilizadas: " << sol_bt.size() << "\n";
                cout << "> Tiempo de ejecucion: " << duration_bt.count() << " ms\n\n";
            }

            // --- EJECUCIÓN DE LA HEURÍSTICA ---
            cout << "--- Ejecutando HEURISTICA (NFDH) ---\n";
            auto start_ffdh = high_resolution_clock::now();
            
            // Aunque la función se llame FFDH_Corte en Algoritmos.h, 
            // internamente ya corre tu optimización industrial NFDH
            vector<Plancha> sol_ffdh = FFDH_Corte(piezas, W, H);
            
            auto end_ffdh = high_resolution_clock::now();
            auto duration_ffdh = duration_cast<milliseconds>(end_ffdh - start_ffdh);
            
            cout << "> Planchas utilizadas: " << sol_ffdh.size() << "\n";
            cout << "> Tiempo de ejecucion: " << duration_ffdh.count() << " ms\n";

        } catch (const bad_alloc& e) {
            cout << "\n[ERROR CRITICO DE HARDWARE]\n";
            cout << "Se ha producido un desbordamiento de memoria (Heap Overflow).\n";
            cout << "Explicacion: La Complejidad Espacial es O(n). Intentar almacenar " << n 
                 << " objetos excede la capacidad de la memoria RAM disponible.\n";
        }

    } while (true);

    cout << "Saliendo del programa...\n";
    return 0;
}