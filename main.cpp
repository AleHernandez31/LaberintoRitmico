#include <SFML/Graphics.hpp>
#include <iostream>
#include "Menu.h"
#include "Player.h"
#include "Song.h"
#include "Mapa.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Laberinto Ritmico");
    window.setFramerateLimit(60);

    // Menu y vista de menu
    Menu menu;
    sf::View viewUI;
    viewUI = window.getDefaultView();

    EstadoJuego estadoActual = MENU;
    Song nivel1Music("assets/songs/ATW.mp3", false);
    Song introMusic("assets/sounds/Intro.mp3", true);
    introMusic.play();

    //Mundo y camara del juego.
    Map map;
    sf::View viewGame;
    const float VIEW_W = 2.f * Map::CELL + Map::STREET; //tamaño del mapa 2x2 para tener visibles siempre 4 opciones (3 viables)
    const float VIEW_H = 2.f * Map::CELL + Map::STREET;
    viewGame.setSize(VIEW_W, VIEW_H);

    // Player.
    sf::Vector2f posInicial = {0,0};
    float velocidad = 2000.f;
    Player player(map.CELL, velocidad, posInicial);
    sf::Clock clock;

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
                            menu.moverArriba();
                        }
                        else if (event.key.code == sf::Keyboard::S) {
                            menu.moverAbajo();
                        }
                        else if (event.key.code == sf::Keyboard::Enter) {
                            int opcion = menu.getOpcionSeleccionada(); // Asigna el valor a opcion cuando presiona enter

                            if (opcion == 0) {
                                estadoActual = JUGANDO;
                                introMusic.stop();
                                nivel1Music.play();
                            }
                            else if (opcion == 1) {
                                estadoActual = CONTROLES;
                            }
                            else if (opcion == 2) {
                                estadoActual = PUNTAJES;
                            }
                            else if (opcion == 3) {
                                window.close();
                            }
                        }
                    }
                    break;

                case JUGANDO:
                    if (event.type == sf::Event::KeyPressed) {  // Vuelve directo al menu (Provisional)
                        if (event.key.code == sf::Keyboard::Escape) {
                            estadoActual = MENU;
                            nivel1Music.stop();
                            introMusic.play();
                        }
                    }
                    player.manejadorEventos(event);   // Envio el evento a la clase Player para procesarlo ahi.
                    break;

                case CONTROLES:
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Escape) {
                            estadoActual = MENU;
                        }
                    }
                    break;

                case PUNTAJES:
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Escape) {
                            estadoActual = MENU;
                        }
                    }
                    break;

                case SALIR:
                    window.close();
                    break;
            }
        }

        //CMD

        // Update
        float deltaTiempo = clock.restart().asSeconds();  // Tomo el tiempo transcurrido entre frames para pasarselo al objeto Player

        if (estadoActual == JUGANDO) {
            player.update(deltaTiempo);
        }

        // Draw
        window.clear();

        switch (estadoActual) {
            case MENU:
                window.setView(viewUI);  // Cambio a vista Menu
                menu.dibujar(window);
                break;

            case JUGANDO:
                // Agregar Imagenes del Gameplay
                viewGame.setCenter(player.getPosMundo()); // Centro la vista sobre el jugador.
                window.setView(viewGame);
                //map.draw2x2Framed(window, player.getWorldPos(), player.getPosGrilla().x, player.getPosGrilla().y); // Dibujo mapa centrado sobre la posición del jugador.
                map.draw2x2Framed(window, player.getPosGrilla());
                window.draw(player); // Dibujo Player
                break;

            case CONTROLES:
                window.setView(viewUI);
                // Agregar contenido de controles
                break;

            case PUNTAJES:
                window.setView(viewUI);
                // Agregar contenido de puntajes
                break;

            case SALIR:
                break;
        }

        // Display
        window.display();
    }

    return 0;
}
