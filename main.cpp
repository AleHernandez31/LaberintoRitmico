#include <SFML/Graphics.hpp>
#include <iostream>
#include "Menu.h"
#include "Player.h"
#include "Song.h"
#include "Mapa.h"
#include "PrototipoFuncionalidad.h"

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

    // PROTOTIPO FUNCIONALIDAD
    PrototipoFuncionalidad protoFunc(map.CELL, map.STREET);
    ConfigRitmo configRitmo;
    configRitmo.cadenciaMs = 500;

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

                                protoFunc.iniciar(player.getPosGrilla(), configRitmo); // PROTOTIPO FUNCIONALIDAD
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

            case JUGANDO: //lucas agregué un prefetch para corregir la cortina negra del juego, agregué codigo pero no cambie lo tuyo.
                          // el problema era que al setear al view de la ventana pinta el proxima mapa al final, ya que sigue al jugador. seria un (actualizo camara, fijo view, pinto el mapa y pinto al jugador. tonces aparece esa cortina negra hasta que el proximo chunk 2x2 se dibuje.

                // Agregar Imagenes del Gameplay
                viewGame.setCenter(player.getPosMundo()); // Centro la vista sobre el jugador. (mejora el seguimiento del dash)
                window.setView(viewGame);

                //agregue un "prefetch" que encontre de un chabon que lo que hace es dibujar por adelantado los chunks vecinos, asi cuando la camara sigue al pj, ya esta esa parte cargada en el borde y no se nota la cortina.
                const sf::Vector2i grid = player.getPosGrilla(); //tome el centro del mundo segun el chunk actual donde esta el pj
                map.draw2x2Framed(window, grid);

                //Con esto logro que se dibujen los chunks vecinos a medida que la camara/player se aleja de la antigua interseccion/centro.
                const sf::Vector2f world   = player.getPosMundo(); //marco la posicion del pj.
                const sf::Vector2f centerG = { grid.x * Map::CELL, grid.y * Map::CELL }; //multiplico por map cell para conseguir la posicion en el mapa donde va caer la proxima interseccion.
                const sf::Vector2f delta   = world - centerG; //calculo cuanto se dezplazo el pj desde el chunk anterior. asi puedo marcar un umbral para generar los otros chunks antes de que se genere la cortina.

                // el umbral es un valor "limite" que voy a usar como señal para decidir cuando generar los chunks vecinos.
                // imaginenlo como un circulo que cuando estas adentro es TH negativo, pero cuando salis es TH positivo y ahi meto el prefetch para dibujar por adelantado el chunk que sigue. (si,estoy enfermo).
                const float TH = Map::CELL * 0.25f; // lo defini como el 25% del tamaño de una manzana. tonces si se aleja mas del 25% del centro, asumimos la direccion a la que va y generamos ese chunk vecino.
                int sx = (delta.x >  TH) ? +1 : (delta.x < -TH ? -1 : 0); //aca segun sx = (+1 este); (0 oeste).
                int sy = (delta.y >  TH) ? +1 : (delta.y < -TH ? -1 : 0);// segun sy = (+1 sur); (0 norte).

                if (sx != 0) map.draw2x2Framed(window, grid + sf::Vector2i(sx, 0)); //si va hacia el este u oeste, se dibuja el vecino lateral.
                if (sy != 0) map.draw2x2Framed(window, grid + sf::Vector2i(0, sy));// lo mismo para norte y sur.
                if (sx != 0 && sy != 0) map.draw2x2Framed(window, grid + sf::Vector2i(sx, sy));//esto es para cuando se mueva en diagonal, por si llegamos a agregar que al inicio sale de la plaza y el primer dash es diagonal para llegar a una esquina, lo dejo armado para tenerlo  en cuenta.

                window.draw(player); // Dibujo Player

                protoFunc.actualizar(player.getPosGrilla(), window, deltaTiempo); // PROTOTIPO FUNCIONALIDAD
                break;

        }

        // Display
        window.display();
    }

    return 0;
}
