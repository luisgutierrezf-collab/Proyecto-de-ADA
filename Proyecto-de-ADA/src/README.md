# Optimizador2D — Proyecto de Análisis y Diseño de Algoritmos (2D Cutting Stock Problem)

## 📋 Descripción

Este proyecto implementa y compara dos algoritmos para resolver el **problema de corte 2D** (2D Cutting Stock Problem): dado un conjunto de piezas rectangulares, se busca ubicarlas dentro de planchas de un tamaño fijo (120 x 240) minimizando el desperdicio de material.

Los algoritmos implementados son:

- **Heurística NFDH/FFDH** — complejidad `O(n log n)`, apta para volúmenes grandes de datos.
- **Backtracking exacto** — complejidad `O(n!)`, solo habilitado para conjuntos pequeños (n ≤ 15) por su costo computacional.

El programa `Optimizador2D` es una **interfaz gráfica interactiva** (hecha con la librería SFML) que permite generar datasets de distinto tamaño y naturaleza, ejecutar cualquiera de los dos algoritmos, y visualizar cómo quedan distribuidas las piezas dentro de cada plancha, junto con métricas de tiempo de ejecución y porcentaje de desperdicio.

> ⚠️ **Importante:** este es un proyecto pensado para **Windows** (el ejecutable incluido es un `.exe` y la configuración de VS Code apunta al compilador de Microsoft). Las instrucciones de abajo están hechas para Windows 10/11.

---

## 🖥️ Requisitos previos

Antes de empezar solo necesitas tener instalado:

- **Windows 10 u 11**
- **Visual Studio Code** — [descargar aquí](https://code.visualstudio.com/)
- Conexión a internet (para descargar el compilador y las librerías la primera vez)

No necesitas tener nada más instalado: en esta guía instalaremos el compilador de C++ y la librería gráfica SFML desde cero.

---

## 🚀 Guía paso a paso

### Paso 1: Descargar el repositorio

1. Entra a la página del repositorio en tu navegador: `https://github.com/luisgutierrezf-collab/Proyecto-de-ADA`
2. Haz clic en el botón verde **`<> Code`**.
3. Selecciona **`Download ZIP`**.
4. Cuando termine de descargar, haz clic derecho sobre el archivo ZIP y elige **"Extraer todo..."**. Guarda la carpeta extraída en un lugar fácil de encontrar, por ejemplo `C:\Proyectos\`.

> Alternativa (si ya tienes Git instalado): abre una terminal y ejecuta
> `git clone https://github.com/luisgutierrezf-collab/Proyecto-de-ADA.git`

### Paso 2: Abrir el proyecto en VS Code

1. Abre **Visual Studio Code**.
2. Ve a **Archivo (File) → Abrir carpeta (Open Folder...)**.
3. Selecciona la carpeta **`Proyecto-de-ADA`** que acabas de extraer (la carpeta que contiene la subcarpeta `.vscode` y otra carpeta también llamada `Proyecto-de-ADA`).

### Paso 3: Instalar la extensión de C/C++ en VS Code

1. En el panel izquierdo de VS Code, haz clic en el ícono de **Extensiones** (el ícono de cuadrados, o presiona `Ctrl+Shift+X`).
2. Busca **"C/C++"** (de Microsoft).
3. Haz clic en **Instalar**.

### Paso 4: Instalar el compilador (MinGW-w64) y la librería SFML mediante MSYS2

Este es el paso más importante: el programa necesita un compilador de C++ (`g++`) y la librería gráfica **SFML 3**, que instalaremos juntos usando **MSYS2** (un gestor de paquetes para Windows).

1. Descarga el instalador de MSYS2 desde la página oficial: **https://www.msys2.org/**
2. Ejecuta el instalador y deja todas las opciones por defecto (la ruta de instalación por defecto es `C:\msys64`).
3. Al terminar la instalación, se abrirá automáticamente una terminal llamada **"MSYS2 MSYS"**. En esa terminal escribe:
   ```
   pacman -Syu
   ```
   y presiona Enter. Es posible que te pida cerrar la ventana a la mitad del proceso (te lo indicará en un mensaje); si eso pasa, ciérrala, abre de nuevo **"MSYS2 MSYS"** desde el menú Inicio, y vuelve a ejecutar el mismo comando hasta que ya no haya actualizaciones pendientes.
4. Ahora abre desde el menú Inicio la terminal **"MSYS2 MinGW64"** (ojo: es distinta a la anterior, fíjate que diga *MinGW64* y no solo *MSYS*). En esa terminal instala el compilador y SFML con:
   ```
   pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-sfml
   ```
   Cuando pregunte `Enter a selection (default=all)`, simplemente presiona **Enter**. Cuando pregunte `Proceed with installation? [Y/n]`, escribe **Y** y presiona Enter.

### Paso 5: Agregar el compilador al PATH de Windows

Para que VS Code pueda encontrar el compilador `g++` y las librerías de SFML, hay que agregar su carpeta al PATH del sistema:

1. Presiona la tecla **Windows**, escribe **"Variables de entorno"** y abre **"Editar las variables de entorno del sistema"**.
2. Haz clic en el botón **"Variables de entorno..."**.
3. En la sección de abajo (**Variables del sistema**), busca la variable llamada **`Path`**, selecciónala y haz clic en **"Editar..."**.
4. Haz clic en **"Nuevo"** y agrega esta ruta exacta:
   ```
   C:\msys64\mingw64\bin
   ```
5. Acepta todas las ventanas haciendo clic en **Aceptar**.
6. **Cierra por completo VS Code y vuelve a abrirlo** para que reconozca el cambio.

### Paso 6: Verificar que todo quedó instalado correctamente

1. En VS Code, abre una terminal nueva: **Terminal → Nueva terminal** (o `Ctrl+ñ` / `` Ctrl+` ``).
2. Escribe:
   ```
   g++ --version
   ```
3. Si aparece información de la versión del compilador (por ejemplo `g++.exe (Rev...) 13.x.x`), todo está listo. Si en cambio dice que el comando no se reconoce, revisa el Paso 5 (probablemente el PATH no quedó bien guardado o falta reiniciar VS Code).

### Paso 7: Compilar el programa

1. En la misma terminal de VS Code (asegúrate de que la carpeta abierta sea `Proyecto-de-ADA`, la de más afuera), ejecuta este comando completo:
   ```
   g++ "Proyecto-de-ADA/src/main_gui.cpp" "Proyecto-de-ADA/src/Pieza.cpp" "Proyecto-de-ADA/src/Plancha.cpp" "Proyecto-de-ADA/src/TiraCorte.cpp" "Proyecto-de-ADA/src/Algoritmos.cpp" -o "Proyecto-de-ADA/Optimizador2D.exe" -lsfml-graphics -lsfml-window -lsfml-system -std=c++17
   ```
2. Este comando compila todos los archivos fuente y genera (o reemplaza) el ejecutable `Optimizador2D.exe` **dentro de la carpeta interna `Proyecto-de-ADA`**, junto a `arial.ttf` y los demás recursos. Esto es importante porque el programa busca la fuente `arial.ttf` en la misma carpeta donde se encuentra el `.exe`.
3. Si no aparece ningún mensaje de error, la compilación fue exitosa.

> 💡 **Atajo:** si solo quieres ejecutar el programa sin modificar el código, una vez completados los Pasos 4 a 6 (compilador y SFML instalados, PATH configurado) **no es obligatorio compilar**: el repositorio ya trae un `Optimizador2D.exe` precompilado en `Proyecto-de-ADA/Proyecto-de-ADA/Optimizador2D.exe`. Al tener las DLLs de SFML disponibles en el PATH, ese ejecutable debería funcionar directamente (puedes saltar al Paso 8).

### Paso 8: Ejecutar el programa

Tienes dos formas de ejecutarlo:

**Desde la terminal de VS Code:**
```
cd "Proyecto-de-ADA"
./Optimizador2D.exe
```

**Desde el Explorador de archivos de Windows:**
1. Navega hasta la carpeta `Proyecto-de-ADA\Proyecto-de-ADA`.
2. Haz doble clic en `Optimizador2D.exe`.

Debería abrirse una ventana titulada **"UNMSM - Análisis y Diseño de Algoritmos (Proyecto 2D-CSP)"**.

### Paso 9: Cómo usar la interfaz

1. **Selecciona el tamaño del dataset (n):** botones `10`, `1K`, `1M` o `10B` (este último es un caso de prueba de manejo de errores, provocará un mensaje controlado de desbordamiento de memoria).
2. **Selecciona la naturaleza de los datos:** aleatorios, ordenados ascendente o descendente.
3. **Ejecuta un algoritmo:**
   - **Algoritmo Heurística (FFDH/NFDH):** funciona con cualquier tamaño de dataset.
   - **Algoritmo Exacto (Backtracking):** solo funciona con `n ≤ 15`; con datasets más grandes el programa lo bloqueará por seguridad (evita que el algoritmo `O(n!)` congele la computadora).
4. Usa los botones **`[ Anterior ]`** y **`[ Siguiente ]`** para navegar entre las distintas planchas generadas como resultado.
5. En el panel izquierdo verás las métricas: tiempo de CPU, número de planchas usadas y porcentaje de desperdicio (merma).

---

## 🛠️ Solución de problemas comunes

| Problema | Causa probable | Solución |
|---|---|---|
| `'g++' no se reconoce como un comando...` | El PATH no tiene la ruta de MinGW64 o no reiniciaste VS Code | Revisa el Paso 5 y reinicia VS Code por completo |
| Falta `libsfml-graphics-3.dll` (o similar) al abrir el `.exe` | Las DLLs de SFML no están en el PATH | Verifica que `C:\msys64\mingw64\bin` esté en el PATH del sistema (Paso 5) y reinicia la computadora si sigue fallando |
| `Error: Asegúrate de poner 'arial.ttf' al costado del ejecutable` | Ejecutaste el `.exe` desde una carpeta distinta a donde está `arial.ttf` | Ejecuta el programa desde dentro de `Proyecto-de-ADA/Proyecto-de-ADA` (Paso 8) |
| El instalador de pacman se cierra solo a la mitad | Es normal en la primera actualización de MSYS2 | Vuelve a abrir "MSYS2 MSYS" y ejecuta `pacman -Syu` de nuevo hasta que no haya más actualizaciones |
| Errores de compilación mencionando SFML | Se instaló SFML pero se está compilando desde una terminal que no es "MSYS2 MinGW64" ni tiene el PATH configurado | Asegúrate de haber seguido el Paso 5 y de reiniciar la terminal de VS Code |

---

## 📁 Estructura del proyecto

```
Proyecto-de-ADA/
├── .vscode/
│   └── c_cpp_properties.json
└── Proyecto-de-ADA/
    ├── Optimizador2D.exe        # Ejecutable precompilado (interfaz gráfica)
    ├── arial.ttf, ARIALN*.ttf   # Fuentes usadas por la interfaz SFML
    ├── graficos.py              # Script en Python para generar gráficos de escalabilidad
    ├── grafico_*.png            # Gráficos de resultados ya generados
    └── src/
        ├── main.cpp             # Versión de consola (sin interfaz gráfica)
        ├── main_gui.cpp         # Programa principal de la interfaz gráfica (Optimizador2D)
        ├── Algoritmos.h/.cpp    # Implementación de FFDH/NFDH y Backtracking
        ├── Pieza.h/.cpp         # Clase que representa una pieza a cortar
        ├── Plancha.h/.cpp       # Clase que representa una plancha y sus piezas
        └── TiraCorte.h/.cpp     # Clase auxiliar para el empaquetado por tiras
```
