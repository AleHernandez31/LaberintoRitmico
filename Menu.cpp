#include "Menu.h"
#include <iostream>

Menu::Menu() {
    tex.loadFromFile("assets/images/menu.png");
    image.setTexture(tex);
    font.loadFromFile("assets/fonts/arial.ttf");

    opcionSeleccionada = 0;


    const char* nombresOpciones[] = {"JUGAR", "CONTROLES", "PUNTAJES", "SALIR"};

    // Calculamos el total de opciones
    totalOpciones = sizeof(nombresOpciones) / sizeof(nombresOpciones[0]);


    // Asignar la memoria para el array de sf::Text
    textos = new sf::Text[totalOpciones];

    // configuracion visual
    float posX = 400.0f;
    float posYInicial = 320.0f;
    float margen = 80.0f;
    sf::Color colorTextoNormal = sf::Color::White;
    int tamanioTexto = 40;

    // Textos
    for (int i = 0; i < totalOpciones; i++) {
        textos[i].setFont(font);
        textos[i].setString(nombresOpciones[i]);
        textos[i].setCharacterSize(tamanioTexto);
        textos[i].setFillColor(colorTextoNormal);
        textos[i].setStyle(sf::Text::Bold);

        sf::FloatRect bounds = textos[i].getLocalBounds();
        textos[i].setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
        textos[i].setPosition(posX, posYInicial + i * margen);
    }
}


Menu::~Menu() {
    delete[] textos;
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
    window.draw(image);

    for (int i = 0; i < totalOpciones; i++) {
        if (i == opcionSeleccionada) {
            textos[i].setFillColor(sf::Color(255, 200, 100));
            textos[i].setScale(1.2f, 1.2f);
        } else {
            textos[i].setFillColor(sf::Color::White);
            textos[i].setScale(1.0f, 1.0f);
        }
        window.draw(textos[i]);
    }
}
