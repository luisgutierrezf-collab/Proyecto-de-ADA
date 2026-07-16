#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <chrono> 
#include <random> 
#include <stdexcept>
#include <algorithm>
#include "Pieza.h"
#include "Plancha.h"
#include "Algoritmos.h"

// Configuración de la Ventana Industrial
const int VENTANA_ANCHO = 1250;
const int VENTANA_ALTO = 760;
const double PLANCHA_REAL_W = 120.0;
const double PLANCHA_REAL_H = 240.0;

// Estado del Sistema
std::vector<Pieza> datasetActual;
std::vector<Plancha> planchasResultado;
int planchaActualIndex = 0;
double tiempoEjecucionMs = 0.0;
double porcentajeDesperdicio = 0.0;
std::string algoritmoUsado = "Ninguno";
bool errorHardwareOcurrido = false;
std::string mensajeError = "";

// Opciones de Configuración seleccionadas por el usuario
long long tamanoSeleccionado = 10;       // 10, 1000, 1000000, 10000000000
int naturalezaSeleccionada = 1;        // 1: Aleatorio, 2: Ascendente, 3: Descendente

// Generador de Datasets Oficial adaptado de tu compañero
std::vector<Pieza> GenerarDatasetGUI(long long n, double max_w, double max_h, int naturaleza_datos) {
    std::vector<Pieza> dataset;
    if (n <= 1000000) {
        dataset.reserve(n); 
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis_w(10.0, max_w);
    std::uniform_real_distribution<> dis_h(10.0, max_h);

    for (long long i = 0; i < n; i++) {
        dataset.push_back(Pieza(i + 1, dis_w(gen), dis_h(gen)));
    }

    if (naturaleza_datos == 2) {
        std::sort(dataset.begin(), dataset.end(), [](const Pieza& a, const Pieza& b) {
            return (a.w * a.h) < (b.w * b.h);
        });
    } 
    else if (naturaleza_datos == 3) {
        std::sort(dataset.begin(), dataset.end(), [](const Pieza& a, const Pieza& b) {
            return (a.w * a.h) > (b.w * b.h);
        });
    }

    return dataset;
}

// Cálculo del área de mermas según tu modelo matemático
void CalcularMetricasGUI() {
    if (planchasResultado.empty()) return;
    
    double areaTotalPlanchas = planchasResultado.size() * (PLANCHA_REAL_W * PLANCHA_REAL_H);
    double areaPiezasCortadas = 0.0;
    
    for (const auto& plancha : planchasResultado) {
        for (const auto& pieza : plancha.piezas_cortadas) {
            areaPiezasCortadas += (pieza.w * pieza.h);
        }
    }
    
    double areaDesperdicio = areaTotalPlanchas - areaPiezasCortadas;
    porcentajeDesperdicio = (areaDesperdicio / areaTotalPlanchas) * 100.0;
}

int main() {
    // Generar dataset inicial por defecto
    datasetActual = GenerarDatasetGUI(tamanoSeleccionado, PLANCHA_REAL_W, PLANCHA_REAL_H, naturalezaSeleccionada);

    sf::RenderWindow window(sf::VideoMode({(unsigned int)VENTANA_ANCHO, (unsigned int)VENTANA_ALTO}), "UNMSM - Analisis y Diseno de Algoritmos (Proyecto 2D-CSP)", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("arial.ttf")) {
        std::cout << "Error: Asegurate de poner 'arial.ttf' al costado del ejecutable.\n";
    }

    // Definición geométrica de Botones (Estilo UI Moderno)
    sf::RectangleShape panelLateral({380.f, (float)VENTANA_ALTO});
    panelLateral.setFillColor(sf::Color(44, 62, 80));

    // Botones de Tamaño (N)
    sf::RectangleShape btnN10({75.f, 35.f}); btnN10.setPosition({20.f, 95.f});
    sf::RectangleShape btnN1K({75.f, 35.f}); btnN1K.setPosition({105.f, 95.f});
    sf::RectangleShape btnN1M({75.f, 35.f}); btnN1M.setPosition({190.f, 95.f});
    sf::RectangleShape btnN10B({75.f, 35.f}); btnN10B.setPosition({275.f, 95.f});

    // Botones de Naturaleza de Datos
    sf::RectangleShape btnNat1({330.f, 35.f}); btnNat1.setPosition({20.f, 185.f});
    sf::RectangleShape btnNat2({330.f, 35.f}); btnNat2.setPosition({20.f, 230.f});
    sf::RectangleShape btnNat3({330.f, 35.f}); btnNat3.setPosition({20.f, 275.f});

    // Botones de Acción Algorítmica
    sf::RectangleShape btnFFDH({330.f, 45.f}); btnFFDH.setPosition({20.f, 360.f});
    btnFFDH.setFillColor(sf::Color(41, 128, 185));
    sf::RectangleShape btnBT({330.f, 45.f}); btnBT.setPosition({20.f, 415.f});
    btnBT.setFillColor(sf::Color(192, 41, 43));

    // Botones de Navegación de páginas de planchas
    sf::RectangleShape btnPrev({155.f, 40.f}); btnPrev.setPosition({20.f, 690.f});
    btnPrev.setFillColor(sf::Color(52, 73, 94));
    sf::RectangleShape btnNext({155.f, 40.f}); btnNext.setPosition({195.f, 690.f});
    btnNext.setFillColor(sf::Color(52, 73, 94));

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            //cerrar ventana
            if (event->is<sf::Event::Closed>()) {
                 window.close();
            }

            if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseClick->button == sf::Mouse::Button::Left) {

                // Manejo de clicks en Tamaño usando position.x y position.y del evento de SFML 3
                float mx = (float)mouseClick->position.x;
                float my = (float)mouseClick->position.y;

                if (btnN10.getGlobalBounds().contains({mx, my})) { tamanoSeleccionado = 10; errorHardwareOcurrido = false; }
                if (btnN1K.getGlobalBounds().contains({mx, my})) { tamanoSeleccionado = 1000; errorHardwareOcurrido = false; }
                if (btnN1M.getGlobalBounds().contains({mx, my})) { tamanoSeleccionado = 1000000; errorHardwareOcurrido = false; }
                if (btnN10B.getGlobalBounds().contains({mx, my})) { tamanoSeleccionado = 10000000000LL; errorHardwareOcurrido = false; }

                // Manejo de clicks en Naturaleza
                if (btnNat1.getGlobalBounds().contains({mx, my})) naturalezaSeleccionada = 1;
                if (btnNat2.getGlobalBounds().contains({mx, my})) naturalezaSeleccionada = 2;
                if (btnNat3.getGlobalBounds().contains({mx, my})) naturalezaSeleccionada = 3;

                // Ejecución FFDH / NFDH
                if (btnFFDH.getGlobalBounds().contains({mx, my})) {
                    try {
                        errorHardwareOcurrido = false;
                        planchasResultado.clear();
                        
                        // Simular retraso y capturar el desbordamiento si n es extremo
                        if (tamanoSeleccionado == 10000000000LL) {
                            throw std::bad_alloc(); 
                        }
                        
                        datasetActual = GenerarDatasetGUI(tamanoSeleccionado, PLANCHA_REAL_W, PLANCHA_REAL_H, naturalezaSeleccionada);
                        
                        auto start = std::chrono::high_resolution_clock::now();
                        planchasResultado = FFDH_Corte(datasetActual, PLANCHA_REAL_W, PLANCHA_REAL_H);
                        auto end = std::chrono::high_resolution_clock::now();
                        
                        tiempoEjecucionMs = std::chrono::duration<double, std::milli>(end - start).count();
                        algoritmoUsado = "Heuristica Industrial";
                        planchaActualIndex = 0;
                        CalcularMetricasGUI();
                    } 
                    catch (const std::bad_alloc& e) {
                        errorHardwareOcurrido = true;
                        algoritmoUsado = "Ninguno";
                        planchasResultado.clear();
                        mensajeError = "CRITICO: Heap Overflow\n\nComplejidad Espacial O(n).\nIntentar instanciar 10^10 objetos\nexcede los limites de la RAM.";
                    }
                }

                // Ejecución Backtracking
                if (btnBT.getGlobalBounds().contains({mx, my})) {
                    if (tamanoSeleccionado > 15) {
                        errorHardwareOcurrido = true;
                        mensajeError = "OMITIDO POR SEGURIDAD\n\nComplejidad Temporal O(n!).\nEvitamos colapsar el procesador\npor explosion combinatoria.";
                    } else {
                        errorHardwareOcurrido = false;
                        datasetActual = GenerarDatasetGUI(tamanoSeleccionado, PLANCHA_REAL_W, PLANCHA_REAL_H, naturalezaSeleccionada);
                        
                        auto start = std::chrono::high_resolution_clock::now();
                        planchasResultado = Ejecutar_Backtracking(datasetActual, PLANCHA_REAL_W, PLANCHA_REAL_H);
                        auto end = std::chrono::high_resolution_clock::now();
                        
                        tiempoEjecucionMs = std::chrono::duration<double, std::milli>(end - start).count();
                        algoritmoUsado = "Backtracking Exacto";
                        planchaActualIndex = 0;
                        CalcularMetricasGUI();
                    }
                }

                // Navegación Interactiva
                if (btnPrev.getGlobalBounds().contains({mx, my}) && planchaActualIndex > 0) planchaActualIndex--;
                if (btnNext.getGlobalBounds().contains({mx, my}) && planchaActualIndex < (int)planchasResultado.size() - 1) planchaActualIndex++;
            }
        }
        }

        // --- RENDERIZACIÓN VISUAL ---
        window.clear(sf::Color(236, 240, 241));
        window.draw(panelLateral);

        // Feedback visual de botones activos
        btnN10.setFillColor(tamanoSeleccionado == 10 ? sf::Color(46, 204, 113) : sf::Color(127, 140, 141));
        btnN1K.setFillColor(tamanoSeleccionado == 1000 ? sf::Color(46, 204, 113) : sf::Color(127, 140, 141));
        btnN1M.setFillColor(tamanoSeleccionado == 1000000 ? sf::Color(46, 204, 113) : sf::Color(127, 140, 141));
        btnN10B.setFillColor(tamanoSeleccionado == 10000000000LL ? sf::Color(231, 76, 60) : sf::Color(127, 140, 141));

        btnNat1.setFillColor(naturalezaSeleccionada == 1 ? sf::Color(52, 152, 219) : sf::Color(127, 140, 141));
        btnNat2.setFillColor(naturalezaSeleccionada == 2 ? sf::Color(230, 126, 34) : sf::Color(127, 140, 141));
        btnNat3.setFillColor(naturalezaSeleccionada == 3 ? sf::Color(155, 89, 182) : sf::Color(127, 140, 141));

        window.draw(btnN10); window.draw(btnN1K); window.draw(btnN1M); window.draw(btnN10B);
        window.draw(btnNat1); window.draw(btnNat2); window.draw(btnNat3);
        window.draw(btnFFDH); window.draw(btnBT);
        if (!planchasResultado.empty()) { window.draw(btnPrev); window.draw(btnNext); }

        // Renderizado de Textos del Panel (Fuente siempre es el primer parametro en SFML 3)
        sf::Text txtTitle(font, "2D CUTTING STOCK SYSTEM", 18); txtTitle.setPosition({20.f, 20.f}); txtTitle.setFillColor(sf::Color::White); window.draw(txtTitle);
        sf::Text txtSec1(font, "1. SELECCIONE EL TAMANO (n):", 13); txtSec1.setPosition({20.f, 65.f}); window.draw(txtSec1);
        
        sf::Text t1(font, "10", 13); t1.setPosition({48.f, 103.f}); window.draw(t1);
        sf::Text t2(font, "1K", 13); t2.setPosition({133.f, 103.f}); window.draw(t2);
        sf::Text t3(font, "1M", 13); t3.setPosition({218.f, 103.f}); window.draw(t3);
        sf::Text t4(font, "10B", 13); t4.setPosition({300.f, 103.f}); window.draw(t4);

        sf::Text txtSec2(font, "2. NATURALEZA DE DATOS:", 13); txtSec2.setPosition({20.f, 155.f}); window.draw(txtSec2);
        sf::Text n1(font, "Aleatorios (Estandard)", 13); n1.setPosition({35.f, 193.f}); window.draw(n1);
        sf::Text n2(font, "Ordenado Ascendente (Peor)", 13); n2.setPosition({35.f, 238.f}); window.draw(n2);
        sf::Text n3(font, "Ordenado Descendente (Mejor)", 13); n3.setPosition({35.f, 283.f}); window.draw(n3);

        sf::Text txtSec3(font, "3. EJECUTAR EVALUACION:", 13); txtSec3.setPosition({20.f, 335.f}); window.draw(txtSec3);
        sf::Text ffdhT(font, "Algoritmo Heuristica (FFDH/NFDH)", 14); ffdhT.setPosition({40.f, 372.f}); window.draw(ffdhT);
        sf::Text btT(font, "Algoritmo Exacto (Backtracking)", 14); btT.setPosition({45.f, 427.f}); window.draw(btT);

        // Métricas de Rendimiento Asintótico
        sf::Text txtMet(font, "METRICAS DE RENDIMIENTO (O)", 15); txtMet.setPosition({20.f, 485.f}); txtMet.setFillColor(sf::Color(241, 196, 15)); window.draw(txtMet);
        sf::Text m1(font, "Estrategia: " + algoritmoUsado, 13); m1.setPosition({20.f, 520.f}); window.draw(m1);
        
        char b1[50]; sprintf(b1, "Tiempo de CPU: %.4f ms", tiempoEjecucionMs);
        sf::Text m2(font, b1, 13); m2.setPosition({20.f, 550.f}); window.draw(m2);
        sf::Text m3(font, "Planchas Activadas: " + std::to_string(planchasResultado.size()), 13); m3.setPosition({20.f, 580.f}); window.draw(m3);
        
        char b2[50]; sprintf(b2, "Eficiencia Merma: %.2f%%", porcentajeDesperdicio);
        sf::Text m4(font, b2, 13); m4.setPosition({20.f, 610.f}); window.draw(m4);

        if (!planchasResultado.empty()) {
            sf::Text pNav(font, "Plancha: " + std::to_string(planchaActualIndex + 1) + " / " + std::to_string(planchasResultado.size()), 14);
            pNav.setPosition({20.f, 655.f}); pNav.setFillColor(sf::Color(46, 204, 113)); window.draw(pNav);
            sf::Text bP(font, "[ Anterior ]", 13); bP.setPosition({55.f, 700.f}); window.draw(bP);
            sf::Text bN(font, "[ Siguiente ]", 13); bN.setPosition({235.f, 700.f}); window.draw(bN);
        }

        // --- LIENZO GRÁFICO (CANVAS DE DIBUJO DERECHO) ---
        float factorEscala = 640.f / PLANCHA_REAL_H; 
        float lienzoX = 380.f + (VENTANA_ANCHO - 380.f - (PLANCHA_REAL_W * factorEscala)) / 2.f;
        float lienzoY = 60.f;

        // Marco contenedor de la plancha base
        sf::RectangleShape visualPlancha({(float)(PLANCHA_REAL_W * factorEscala), (float)(PLANCHA_REAL_H * factorEscala)});
        visualPlancha.setPosition({lienzoX, lienzoY});
        visualPlancha.setFillColor(sf::Color::White);
        visualPlancha.setOutlineThickness(4.f);
        visualPlancha.setOutlineColor(sf::Color(52, 73, 94));
        window.draw(visualPlancha);

        // Despliegue seguro de errores de desbordamiento académico
        if (errorHardwareOcurrido) {
            sf::RectangleShape errorBox({500.f, 250.f});
            errorBox.setPosition({520.f, 250.f});
            errorBox.setFillColor(sf::Color(231, 76, 60));
            errorBox.setOutlineThickness(2.f);
            errorBox.setOutlineColor(sf::Color::White);
            window.draw(errorBox);

            sf::Text txtErr(font, mensajeError, 16);
            txtErr.setPosition({550.f, 280.f});
            txtErr.setFillColor(sf::Color::White);
            window.draw(txtErr);
        }
        // Pintar rectángulos de la plancha actual si hay soluciones válidas
        else if (!planchasResultado.empty() && planchaActualIndex < (int)planchasResultado.size()) {
            const auto& piezas = planchasResultado[planchaActualIndex].piezas_cortadas;
            
            // Protección Anti-Lag: Si una sola plancha contiene demasiadas piezas (casos masivos), 
            // limitamos el dibujado a las primeras 3000 para mantener estables los FPS de la GUI.
            int piezasADibujar = std::min((int)piezas.size(), 3000);
            
            for (int i = 0; i < piezasADibujar; i++) {
                const auto& pieza = piezas[i];
                int r = (int)(pieza.w * 9) % 180 + 50;
                int g = (int)(pieza.h * 13) % 180 + 50;
                int b = (int)(pieza.w * pieza.h) % 180 + 50;

                sf::RectangleShape visualPieza({(float)(pieza.w * factorEscala), (float)(pieza.h * factorEscala)});
                visualPieza.setPosition({lienzoX + (float)(pieza.x * factorEscala), lienzoY + (float)(pieza.y * factorEscala)});
                visualPieza.setFillColor(sf::Color(r, g, b));
                visualPieza.setOutlineThickness(0.5f);
                visualPieza.setOutlineColor(sf::Color(30, 30, 30));
                window.draw(visualPieza);
            }

            if ((int)piezas.size() > 3000) {
                sf::Text txtWarn(font, "Visualizacion optimizada: Mostrando primeras 3000 piezas de la plancha.", 12);
                txtWarn.setPosition({450.f, 15.f});
                txtWarn.setFillColor(sf::Color(127, 140, 141));
                window.draw(txtWarn);
            }
        }

        window.display();
    }

    return 0;
}