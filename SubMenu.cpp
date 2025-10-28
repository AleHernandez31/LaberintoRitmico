#include "SubMenu.h"
#include <iostream>

SubMenu::SubMenu() {
    font.loadFromFile("assets/fonts/arial.ttf");
    activo = false;
    opcionSeleccionada = 0;
    tipoActual = NINGUNO;

    configurarEstilo();
}

void SubMenu::configurarEstilo() {
    // Overlay (fondo oscuro semi-transparente)
    overlay.setSize(sf::Vector2f(800, 600));
    overlay.setPosition(0, 0);
    overlay.setFillColor(sf::Color(0, 0, 0, 200));  // Negro

    // Fondo del submenu
    fondo.setSize(sf::Vector2f(500, 400));
    fondo.setPosition(150, 100);
    fondo.setFillColor(sf::Color(30, 40, 60, 250));
    fondo.setOutlineThickness(4);
    fondo.setOutlineColor(sf::Color(100, 200, 255));  // Borde azul

    // Titulo
    titulo.setFont(font);
    titulo.setCharacterSize(40);  //
    titulo.setFillColor(sf::Color(255, 200, 100));
    titulo.setStyle(sf::Text::Bold);
}

void SubMenu::mostrarSeleccionNivel() {
    activo = true;
    tipoActual = SELECCION_NIVEL;
    opcionSeleccionada = 0;  // Se activa, actulamente esta en selecion de nivel y la opcion es la primera
    textos.clear();

    // Configurar titulo
    titulo.setString("SELECCIONA NIVEL");
    sf::FloatRect titleBounds = titulo.getLocalBounds();
    titulo.setOrigin(titleBounds.left + titleBounds.width / 2, titleBounds.top + titleBounds.height / 2);
    titulo.setPosition(400, 160);

    // Crear opciones de niveles
    std::vector<std::string> opcionesNiveles = {
        "Nivel 1",
        "Nivel 2  [Bloqueado]",
        "Nivel 3  [Bloqueado]"
    };

    totalOpciones = opcionesNiveles.size();
    float posX = 400;
    float posYInicial = 240;
    float margen = 70;
    // Crea las opciones
    for (int i = 0; i < totalOpciones; i++) {
        sf::Text texto;
        texto.setFont(font);
        texto.setString(opcionesNiveles[i]);
        texto.setCharacterSize(34);

        // Nivel 1 disponible, el resto bloqueado (Por ahora)
        if (i == 0) {
            texto.setFillColor(sf::Color::White);
            texto.setStyle(sf::Text::Bold);
        } else {
            texto.setFillColor(sf::Color(120, 120, 120));
            texto.setStyle(sf::Text::Regular);
        }

        sf::FloatRect bounds = texto.getLocalBounds();
        texto.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
        texto.setPosition(posX, posYInicial + i * margen);

        textos.push_back(texto);
    }
}
// Igual que mostrasSeleccionNivel pero con distinto tipoActual
void SubMenu::mostrarSeleccionCancion(int nivel) {
    activo = true;
    tipoActual = SELECCION_CANCION;
    opcionSeleccionada = 0;
    textos.clear();

    // Configurar titulo
    titulo.setString("SELECCIONA CANCION");
    sf::FloatRect titleBounds = titulo.getLocalBounds();
    titulo.setOrigin(titleBounds.left + titleBounds.width / 2, titleBounds.top + titleBounds.height / 2);
    titulo.setPosition(400, 160);

    // Crear opciones de canciones
    std::vector<std::string> opcionesCanciones = {
        "Around the World",
        "Cancion 2  [Bloqueado]",
        "Cancion 3  [Bloqueado]"
    };

    totalOpciones = opcionesCanciones.size();
    float posX = 400;
    float posYInicial = 240;
    float margen = 70;

    for (int i = 0; i < totalOpciones; i++) {
        sf::Text texto;
        texto.setFont(font);
        texto.setString(opcionesCanciones[i]);
        texto.setCharacterSize(32);

        // Solo la primera cancion disponible (Por ahora)
        if (i == 0) {
            texto.setFillColor(sf::Color::White);
            texto.setStyle(sf::Text::Bold);
        } else {
            texto.setFillColor(sf::Color(120, 120, 120));
            texto.setStyle(sf::Text::Regular);
        }

        sf::FloatRect bounds = texto.getLocalBounds();
        texto.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
        texto.setPosition(posX, posYInicial + i * margen);

        textos.push_back(texto);
    }
}

void SubMenu::ocultar() {
    activo = false;
    tipoActual = NINGUNO;
    textos.clear();
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
    if (!activo) return; // Si no esta activo no dibuja nada

    // Dibujar overlay
    window.draw(overlay);

    // Dibujar fondo del submenu
    window.draw(fondo);

    // Dibujar titulo
    window.draw(titulo);

    // Dibujar opciones
    for (int i = 0; i < totalOpciones; i++) {
        if (i == opcionSeleccionada) {
            // Solo resaltar si no está bloqueado (i == 0 para nivel 1 y canción 1)
            if (i == 0) {
                textos[i].setFillColor(sf::Color(255, 150, 50));
                textos[i].setScale(1.15f, 1.15f);
            } else {
                textos[i].setScale(1.0f, 1.0f);
            }
        } else {
            if (i == 0) {
                textos[i].setFillColor(sf::Color::White);
            }
            textos[i].setScale(1.0f, 1.0f);
        }

        window.draw(textos[i]);
    }

    // Instrucciones (Info abajo del submenu)
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
