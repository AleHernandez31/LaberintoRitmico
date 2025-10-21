#include <SFML/Graphics.hpp>
#include <iostream>
#include "Menu.h"
#include "Player.h"
#include "Song.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Laberinto Ritmico");
    window.setFramerateLimit(60);

    Menu menu;
    Player player;
    EstadoJuego estadoActual = MENU;
    Song nivel1Music("assets/songs/ATW.mp3", false);
    Song introMusic("assets/sounds/Intro.mp3", true);
    introMusic.play();

    // Game Loop
    while (window.isOpen()) {

        // ReadInput
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Menu principal
            switch (estadoActual) {
                case MENU:
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::W) {
                            menu.moverArriba(); // Colorea la opcion y le asigna un valor
                        }
                        else if (event.key.code == sf::Keyboard::S) {
                            menu.moverAbajo(); // Lo mismo que arriba
                        }
                        else if (event.key.code == sf::Keyboard::Enter) {
                            int opcion = menu.getOpcionSeleccionada(); // Asigna el valor a opcion cuando presiona enter

                            if (opcion == 0) {             //  !!ESTO ESTA HARDOCDEADO DESPUES HAY QUE CAMBIARLO PARA QUE SEA MAS FLEXIBLE!!
                                estadoActual = Gameplay;  // Gamplay = 0 ya que se lo asigno en moverArriba.
                            }
                            else if (opcion == 1) {     // Salir = 1
                                window.close();
                            }
                        }
                    }
                    break;

                case Gameplay:
                    if (event.type == sf::Event::KeyPressed) {  // Vuelve directo al menu (Provisional)
                        if (event.key.code == sf::Keyboard::Escape) {
                            estadoActual = MENU;
                        }
                        // Agregar funcionalidades del juego.
                    }
                    break;

                case Salir:
                    window.close();
                    break;
            }
        }

        //CMD

        // Update
        window.clear();
        player.update();

        // Draw
        switch (estadoActual) {
            case MENU:
                menu.dibujar(window);
                nivel1Music.setVolume(300);
                nivel1Music.play();
                break;

            case Gameplay:
                introMusic.stop();
                introMusic.play();
                window.draw(player);
                // Agregar Imagenes del Gameplay
                break;

            case Salir:
                break;
        }

        // Display
        window.display();
    }

    return 0;
}
