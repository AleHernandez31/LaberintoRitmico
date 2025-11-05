#include "SubMenu.h"
#include <iostream>

SubMenu::SubMenu() {
    font.loadFromFile("assets/fonts/arial.ttf");

    activo = false;
    opcionSeleccionada = 0;
    tipoActual = NINGUNO;
    textos = nullptr;
    totalOpciones = 0;
    capacidadTextos = 0;
    configurarEstilo();
}

SubMenu::~SubMenu() {
    liberarTextos();
}

void SubMenu::configurarEstilo() {
    // Overlay )
    overlay.setSize(sf::Vector2f(800, 600));
    overlay.setPosition(0, 0);
    overlay.setFillColor(sf::Color(0, 0, 0, 200));

    // Fondo del submenu
    fondo.setSize(sf::Vector2f(500, 400));
    fondo.setPosition(150, 100);
    fondo.setFillColor(sf::Color(30, 40, 60, 250));
    fondo.setOutlineThickness(4);
    fondo.setOutlineColor(sf::Color(100, 200, 255));

    // Titulo
    titulo.setFont(font);
    titulo.setCharacterSize(40);
    titulo.setFillColor(sf::Color(255, 200, 100));
    titulo.setStyle(sf::Text::Bold);
}

// Metodos para memoria dinamica

void SubMenu::liberarTextos() {
    if (textos != nullptr) {
        delete[] textos;  // Liberar el array
        textos = nullptr;
        totalOpciones = 0;
        capacidadTextos = 0;
    }
}

void SubMenu::crearTextos(int cantidad) {
    liberarTextos();  // Primero liberar si habia algo

    textos = new sf::Text[cantidad];  // Crear nuevo array
    totalOpciones = cantidad;
    capacidadTextos = cantidad;
}

void SubMenu::agregarTexto(const std::string& texto, float posX, float posY,
                           int tamanio, sf::Color color, int indice) {
    if (indice >= 0 && indice < totalOpciones) {
        textos[indice].setFont(font);
        textos[indice].setString(texto);
        textos[indice].setCharacterSize(tamanio);
        textos[indice].setFillColor(color);
        textos[indice].setStyle(sf::Text::Bold);

        sf::FloatRect bounds = textos[indice].getLocalBounds();
        textos[indice].setOrigin(bounds.left + bounds.width / 2,
                                bounds.top + bounds.height / 2);
        textos[indice].setPosition(posX, posY);
    }
}

void SubMenu::mostrarSeleccionNivel() {
    activo = true;
    tipoActual = SELECCION_NIVEL;
    opcionSeleccionada = 0;

    // Configurar titulo
    titulo.setString("SELECCIONA NIVEL");
    sf::FloatRect titleBounds = titulo.getLocalBounds();
    titulo.setOrigin(titleBounds.left + titleBounds.width / 2, titleBounds.top + titleBounds.height / 2);
    titulo.setPosition(400, 160);

    // Crear array dinamico de 3 textos
    crearTextos(3);

    // Configuracion
    float posX = 400;
    float posYInicial = 240;
    float margen = 70;
    int tamanio = 34;

    // Agregar cada texto
    agregarTexto("Nivel 1", posX, posYInicial, tamanio, sf::Color::White, 0);
    agregarTexto("Nivel 2  [Bloqueado]", posX, posYInicial + margen, tamanio, sf::Color(120, 120, 120), 1);
    agregarTexto("Nivel 3  [Bloqueado]", posX, posYInicial + 2 * margen, tamanio, sf::Color(120, 120, 120), 2);
}

void SubMenu::mostrarSeleccionCancion(int nivel) {
    activo = true;
    tipoActual = SELECCION_CANCION;
    opcionSeleccionada = 0;

    // Configurar titulo
    titulo.setString("SELECCIONA CANCION");
    sf::FloatRect titleBounds = titulo.getLocalBounds();
    titulo.setOrigin(titleBounds.left + titleBounds.width / 2, titleBounds.top + titleBounds.height / 2);
    titulo.setPosition(400, 160);

    // Crear array dinamico de 3 textos
    crearTextos(3);

    // Configuracion
    float posX = 400;
    float posYInicial = 240;
    float margen = 70;
    int tamanio = 32;

    // Agregar cada texto
    agregarTexto("All Too Well", posX, posYInicial, tamanio, sf::Color::White, 0);
    agregarTexto("Cancion 2  [Bloqueado]", posX, posYInicial + margen, tamanio, sf::Color(120, 120, 120), 1);
    agregarTexto("Cancion 3  [Bloqueado]", posX, posYInicial + 2 * margen, tamanio, sf::Color(120, 120, 120), 2);
}

void SubMenu::mostrarMenuPausa() {
    activo = true;
    tipoActual = PAUSA;
    opcionSeleccionada = 0;

    titulo.setString("PAUSA");
    sf::FloatRect titleBounds = titulo.getLocalBounds();
    titulo.setOrigin(titleBounds.left + titleBounds.width / 2, titleBounds.top + titleBounds.height / 2);
    titulo.setPosition(400, 160);

    // 3 opciones: Continuar, Volver, Opciones
    crearTextos(3);

    float posX = 400;
    float posYInicial = 240;
    float margen = 70;
    int tamanio = 34;

    agregarTexto("Continuar", posX, posYInicial, tamanio, sf::Color::White, 0);
    agregarTexto("Volver al Menu", posX, posYInicial + margen, tamanio, sf::Color::White, 1);
    agregarTexto("Opciones", posX, posYInicial + 2 * margen, tamanio, sf::Color::White, 2);
}

void SubMenu::mostrarAdvertenciaSalir() {
    activo = true;
    tipoActual = ADVERTENCIA_SALIR;
    opcionSeleccionada = 0; // Por defecto en "No"

    titulo.setString("ADVERTENCIA");
    sf::FloatRect titleBounds = titulo.getLocalBounds();
    titulo.setOrigin(titleBounds.left + titleBounds.width / 2, titleBounds.top + titleBounds.height / 2);
    titulo.setPosition(400, 160);

    crearTextos(2); // 2 opciones: No, Si

    float posX = 400;
    float posYInicial = 260;
    float margen = 70;
    int tamanio = 32;

    // main espera que 0="No" y 1="Si"
    agregarTexto("No, continuar jugando", posX, posYInicial, tamanio, sf::Color::White, 0);
    agregarTexto("Si, volver al menu", posX, posYInicial + margen, tamanio, sf::Color::White, 1);
}


void SubMenu::ocultar() {
    activo = false;
    tipoActual = NINGUNO;
    liberarTextos();
}

bool SubMenu::estaActivo() const {
    return activo;
}

SubMenu::TipoSubMenu SubMenu::getTipo() const {
    return tipoActual;
}

void SubMenu::moverArriba() {
    opcionSeleccionada--;
    if (opcionSeleccionada < 0) {
        opcionSeleccionada = totalOpciones - 1;
    }
}

void SubMenu::moverAbajo() {
    opcionSeleccionada++;
    if (opcionSeleccionada >= totalOpciones) {
        opcionSeleccionada = 0;
    }
}

int SubMenu::getOpcionSeleccionada() const {
    return opcionSeleccionada;
}

void SubMenu::dibujar(sf::RenderWindow& window) {
    if (!activo) return;



    // Dibujar overlay
    window.draw(overlay);

    // Dibujar fondo del submenu
    window.draw(fondo);

    // Dibujar titulo
    window.draw(titulo);


    // Dibujar opciones
    for (int i = 0; i < totalOpciones; i++) {

        //  Resetear estilo
        textos[i].setScale(1.0f, 1.0f);
        textos[i].setFillColor(sf::Color::White); // Color base

        // Logica para menus de seleccion (con opciones bloqueadas)
        if (tipoActual == SELECCION_NIVEL || tipoActual == SELECCION_CANCION) {
            if (i > 0) { // Opciones bloqueadas
                textos[i].setFillColor(sf::Color(120, 120, 120));
            }
            // Solo resaltar si la opcion seleccionada es la 0 (la unica activa)
            if (i == 0 && opcionSeleccionada == 0) {
                textos[i].setFillColor(sf::Color(255, 200, 100)); // Amarillo
                textos[i].setScale(1.15f, 1.15f);
            }
        }
        // Logica para menus de Pausa/Advertencia (todas las opciones activas)
        else if (tipoActual == PAUSA || tipoActual == ADVERTENCIA_SALIR) {
            if (i == opcionSeleccionada) {
                textos[i].setFillColor(sf::Color(255, 200, 100)); // Amarillo
                textos[i].setScale(1.15f, 1.15f);
            } else {
                textos[i].setFillColor(sf::Color::White); // Blanco
            }
        }

        window.draw(textos[i]);
    }

    // Instrucciones
    sf::Text instruccion;
    instruccion.setFont(font);
    instruccion.setString("W/S - Navegar  |  ENTER - Seleccionar  |  ESC - Volver");
    instruccion.setCharacterSize(18);
    instruccion.setFillColor(sf::Color(200, 200, 200));
    sf::FloatRect bounds = instruccion.getLocalBounds();
    instruccion.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
    instruccion.setPosition(400, 460);
    window.draw(instruccion);
}
