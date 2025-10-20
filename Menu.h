#pragma once
#include <SFML/Graphics.hpp>

class Menu {
private:
    //Imagen menu
    sf::Texture tex;
    sf::Sprite image;
    // Textos menu
    sf::Text textoJugar;
    sf::Text textoSalir;
    sf::Font font;
    int opcionSeleccionada = 0;
public:
    Menu();
    void dibujar(sf::RenderWindow& window);
    void moverArriba();
    void moverAbajo();
    int getOpcionSeleccionada();
};

// Estados del juego(Para saber donde estamos parados)
// enum = le das nombre a los numeros. 0 = MENU, 1 = Gameplay , 2 = Salir
// Se utiliza para el switch
enum EstadoJuego {
    MENU,
    Gameplay,
    Salir
};


