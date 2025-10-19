#include <SFML/Graphics.hpp>
#include "Menu.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Laberinto Ritmico");

    Menu menu;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
                // Movimiento y seleccion de menu.
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up)
                    menu.moverArriba();
                else if (event.key.code == sf::Keyboard::Down)
                    menu.moverAbajo();
                else if (event.key.code == sf::Keyboard::Enter) {
                    int opcion = menu.getOpcionSeleccionada();

                    if (opcion == 0)
                        window.close();
                    else if (opcion == 1)
                        window.close();
                }
            }
        }

        window.clear();
        menu.dibujar(window);
        window.display();
    }

    return 0;
}
