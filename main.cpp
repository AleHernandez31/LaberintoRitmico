#include <SFML/Graphics.hpp>
#include <iostream>
#include "Menu.h"
#include "Player.h"

int main() {
    //Inicialización de ventana
    sf::RenderWindow window(sf::VideoMode(800, 600), "Laberinto Ritmico");

    Menu menu;
    Player player;

    // Game Loop
    while (window.isOpen()) {

        // ReadInput
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

        // CMD


        // Update
        window.clear();
        //player.update();


        // Draw
        menu.dibujar(window);
        //window.draw(player);


        // Display
        window.display();


    }

    return 0;
}
