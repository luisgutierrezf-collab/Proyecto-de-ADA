#include <iostream>
#include <vector>
#include <chrono> // Para medir el tiempo exacto de ejecución
#include <random> // Para generar las piezas aleatorias
#include <stdexcept> // Para manejar errores de memoria
#include "Algoritmos.h"

using namespace std;
using namespace std::chrono;

// =========================================================
// FUNCIÓN AUXILIAR: GENERADOR DE CASOS DE PRUEBA
// =========================================================
vector<Pieza> GenerarDataset(long long n, double max_w, double max_h) {
    vector<Pieza> dataset;
    // Pre-reservamos memoria para evitar realojamientos costosos
    dataset.reserve(n); 

    // Configuramos un generador de números aleatorios
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis_w(10.0, max_w); // Ancho entre 10 y max_w
    uniform_real_distribution<> dis_h(10.0, max_h); // Alto entre 10 y max_h

    for (long long i = 0; i < n; i++) {
        dataset.push_back(Pieza(i + 1, dis_w(gen), dis_h(gen)));
    }
    return dataset;
}

// =========================================================
// FUNCIÓN PRINCIPAL
// =========================================================
int main() {
    // Dimensiones estándar de la plancha comercial (Melamina)
    const double W = 120.0;
    const double H = 240.0;
    int opcion;

    do {
        cout << "\n======================================================\n";
        cout << "   OPTIMIZADOR DE CORTE 2D (Cutting Stock Problem)    \n";
        cout << "======================================================\n";
        cout << "1. Caso Pequeno (n = 10) -> Evaluar Backtracking vs FFDH\n";
        cout << "2. Caso Mediano (n = 10^3) -> Evaluar FFDH\n";
        cout << "3. Caso Grande  (n = 10^6) -> Evaluar FFDH\n";
        cout << "4. Caso Extremo (n = 10^10) -> Prueba de Estres (RAM)\n";
        cout << "5. Salir\n";
        cout << "Seleccione un caso de prueba: ";
        cin >> opcion;

        if (opcion < 1 || opcion > 4) {
            if (opcion != 5) cout << "Opcion invalida.\n";
            continue;
        }

        long long n = 0;
        if (opcion == 1) n = 10;
        else if (opcion == 2) n = 1000;
        else if (opcion == 3) n = 1000000;
        else if (opcion == 4) n = 10000000000LL; // 10 mil millones

        cout << "\nGenerando dataset de " << n << " piezas...\n";
        
        try {
            // El bloque try-catch es vital para sobrevivir al caso n = 10^10
            vector<Pieza> piezas = GenerarDataset(n, W, H);
            cout << "Dataset generado exitosamente.\n\n";

            // --- EJECUCIÓN DEL BACKTRACKING (Solo para n=10) ---
            if (opcion == 1) {
                cout << "--- Ejecutando METODO EXACTO (Backtracking) ---\n";
                auto start_bt = high_resolution_clock::now();
                
                vector<Plancha> sol_bt = Ejecutar_Backtracking(piezas, W, H);
                
                auto end_bt = high_resolution_clock::now();
                auto duration_bt = duration_cast<milliseconds>(end_bt - start_bt);
                
                cout << "> Planchas utilizadas: " << sol_bt.size() << "\n";
                cout << "> Tiempo de ejecucion: " << duration_bt.count() << " ms\n\n";
            }

            // --- EJECUCIÓN DE LA HEURÍSTICA FFDH ---
            cout << "--- Ejecutando HEURISTICA (FFDH) ---\n";
            auto start_ffdh = high_resolution_clock::now();
            
            vector<Plancha> sol_ffdh = FFDH_Corte(piezas, W, H);
            
            auto end_ffdh = high_resolution_clock::now();
            auto duration_ffdh = duration_cast<milliseconds>(end_ffdh - start_ffdh);
            
            cout << "> Planchas utilizadas: " << sol_ffdh.size() << "\n";
            cout << "> Tiempo de ejecucion: " << duration_ffdh.count() << " ms\n";

        } catch (const bad_alloc& e) {
            // Aquí capturamos el colapso de la memoria RAM
            cout << "\n[ERROR CRITICO DE HARDWARE]\n";
            cout << "Se ha producido un desbordamiento de memoria (Heap Overflow).\n";
            cout << "Explicacion: La Complejidad Espacial es O(n). Intentar almacenar " << n 
                 << " objetos excede la capacidad de la memoria RAM disponible.\n";
        }

    } while (opcion != 5);

    cout << "Saliendo del programa...\n";
    return 0;
}