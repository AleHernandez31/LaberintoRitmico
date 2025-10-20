#include "Menu.h"


Menu::Menu() {
    // Imagen menu
    tex.loadFromFile("assets/images/menu.png");
    image.setTexture(tex);

    // Cargar Fuente
    font.loadFromFile("assets/fonts/arial.ttf");
    // Creacion del texto
    textoJugar.setFont(font); // Fuente
    textoJugar.setString("JUGAR"); // Texto
    textoJugar.setCharacterSize(30); // Tamaño en pixeles
    textoJugar.setFillColor(sf::Color::Yellow); // Color
    textoJugar.setStyle(sf::Text::Bold); // Ponerlo en Negrita
    textoJugar.setPosition(325,250);  // Posicion

    textoSalir.setFont(font);
    textoSalir.setString("SALIR");
    textoSalir.setCharacterSize(30);
    textoSalir.setFillColor(sf::Color::White);
    textoSalir.setStyle(sf::Text::Bold);
    textoSalir.setPosition(325, 300);

}
// Muestra (Dibuja) la imagen y los textos
void Menu::dibujar(sf::RenderWindow& window) {
    window.draw(image);
    window.draw(textoJugar);
    window.draw(textoSalir);
}
// "Movimiento del teclado"
void Menu::moverArriba() {
    textoJugar.setFillColor(sf::Color::Yellow);
    textoSalir.setFillColor(sf::Color::White);
    opcionSeleccionada = 0;
}
// Igual que el anterior
void Menu::moverAbajo() {
    textoJugar.setFillColor(sf::Color::White);
    textoSalir.setFillColor(sf::Color::Yellow);
    opcionSeleccionada = 1;
}

int Menu::getOpcionSeleccionada(){
    return opcionSeleccionada;
}
