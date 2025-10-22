#include "Menu.h"
#include <iostream>

Menu::Menu() {
    // Cargar imagen de fondo
    tex.loadFromFile("assets/images/menu.png");
    image.setTexture(tex);
    // Cargar fuente
    font.loadFromFile("assets/fonts/arial.ttf");


    // Inicializar
    opcionSeleccionada = 0;

    // Nombres de las opciones del menu
    std::vector<std::string> nombresOpciones = {"JUGAR", "CONTROLES", "PUNTAJES", "SALIR"};
    totalOpciones = nombresOpciones.size();

    float posX = 400.0f;              // Posicion X (centrado horizontal)
    float posYInicial = 320.0f;       // Posicion Y de la primera opcion
    float margen = 80.0f;             // Espacio entre cada opcion (padding/margin)

    // Colores
    sf::Color colorTextoNormal = sf::Color::White;           // Blanco para opciones normales
    sf::Color colorTextoSeleccionado(255, 200, 100);         // Naranja para seleccionado

    // Tamaño del texto
    int tamanioTexto = 40;

    // Textos
    for (int i = 0; i < totalOpciones; i++) {
        sf::Text texto;
        texto.setFont(font);
        texto.setString(nombresOpciones[i]);
        texto.setCharacterSize(tamanioTexto);
        texto.setFillColor(colorTextoNormal);
        texto.setStyle(sf::Text::Bold);

        // Centrar el texto horizontalmente
        sf::FloatRect bounds = texto.getLocalBounds();
        texto.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);

        // Posicionar el texto
        texto.setPosition(posX, posYInicial + i * margen);

        textos.push_back(texto);
    }
}

void Menu::moverArriba() {
    opcionSeleccionada--;

    if (opcionSeleccionada < 0) {
        opcionSeleccionada = totalOpciones - 1;
    }
}

void Menu::moverAbajo() {
    opcionSeleccionada++;

    if (opcionSeleccionada >= totalOpciones) {
        opcionSeleccionada = 0;
    }
}

int Menu::getOpcionSeleccionada() {
    return opcionSeleccionada;
}

void Menu::dibujar(sf::RenderWindow& window) {
    // Dibujar fondo
    window.draw(image);

    // Dibujar todos los textos
    for (int i = 0; i < totalOpciones; i++) {
        // Si esta seleccionado, cambiar color y hacerlo mas grande
        if (i == opcionSeleccionada) {
            textos[i].setFillColor(sf::Color(255, 200, 100));  // Naranja
            textos[i].setScale(1.2f, 1.2f);  // Hacerlo más grande
        } else {
            textos[i].setFillColor(sf::Color::White);  // Blanco
            textos[i].setScale(1.0f, 1.0f);  // Tamaño normal
        }

        window.draw(textos[i]);
    }
}
