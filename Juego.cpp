#include "Juego.h"

Juego::Juego() :
    window(sf::VideoMode(800, 600), "Laberinto Ritmico"),
    menu(), // Llama al constructor de Menu
    subMenu(), // Llama al constructor de SubMenu
    estadoActual(MENU),
    musicaJuego("assets/songs/ATW.mp3", false),
    introMusic("assets/sounds/Intro.mp3", true),
    map(),
    // El player se inicializa usando 'map' y constantes
    player(map.CELL, 2000.f, musicaJuego, { 0,0 }),
    cancion("assets/songs/Song.csv"),
    gestorNodos(map.CELL, map.STREET),
    nivelSeleccionado(-1),
    cancionSeleccionada(-1),
    deltaTiempo(0.f),
    scoring(),
    scoringArchivo()
{
    fuentePrincipal.loadFromFile("assets/fonts/arial.ttf");
    fuenteSecundaria.loadFromFile("assets/fonts/comic.ttf");
    window.setFramerateLimit(60);

    // Menu y vista de menu
    viewUI = window.getDefaultView();

    // Musica
    musicaJuego.setVolume(100.f);
    introMusic.setVolume(100.f);
    introMusic.play();

    //Mundo y camara del juego.
    const float VIEW_W = 2.f * Map::CELL + Map::STREET;
    const float VIEW_H = 2.f * Map::CELL + Map::STREET;
    viewGame.setSize(VIEW_W, VIEW_H);

    // Prueba persistencia de puntuacion
    Scoring puntuaciones[10];
    scoringArchivo.leerPuntuaciones(puntuaciones);

    for (int i=0; i<10 ; i++)
    {
        if (puntuaciones[i].getPuntuacionTotal() > 0)
        {
            puntuaciones[i].toString();
        }
        else break;
    }

    // Estado inicial de la cuenta regresiva / partida
    enCuentaRegresiva    = false;
    valorCuentaRegresiva = 0;
    partidaActiva        = false;
}


//El Bucle Principal
void Juego::iniciarBucle()
{
    // Game Loop
    while (window.isOpen())
    {
        procesarEventos();
        actualizar();
        dibujar();
    }
}

//Procesar Eventos ('pollEvent')
void Juego::procesarEventos()
{
    // ReadInput
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        // Menu principal
        switch (estadoActual)
        {
        case MENU:
            // Si el submenu esta activo (selección de nivel)
            if (subMenu.estaActivo())
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    // Navegación dentro del submenu
                    if (event.key.code == sf::Keyboard::W)
                    {
                        subMenu.moverArriba();
                    }
                    else if (event.key.code == sf::Keyboard::S)
                    {
                        subMenu.moverAbajo();
                    }
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        int opcion = subMenu.getOpcionSeleccionada();

                        // En el MENU sólo usamos el submenu como SELECCION_NIVEL
                        if (subMenu.getTipo() == SubMenu::SELECCION_NIVEL)
                        {
                            // opcion: 0,1,2 -> niveles 1,2,3
                            nivelSeleccionado   = opcion + 1;
                            cancionSeleccionada = nivelSeleccionado;

                            std::cout << "Nivel seleccionado: " << nivelSeleccionado << std::endl;

                            // 1) Elegimos el CSV de nodos segun el nivel
                            std::string pathCancion;
                            if (nivelSeleccionado == 1)
                            {
                                // Nivel 1 = Around the World (fácil)
                                pathCancion = "assets/songs/around_the_world_osu_easy_filtered_v2.csv";
                            }
                            else if (nivelSeleccionado == 2)
                            {
                                // Nivel 2 = Back in Black (intermedio)
                                pathCancion = "assets/songs/back_in_black_normal.csv";
                            }
                            else if (nivelSeleccionado == 3)
                            {
                                // Nivel 3 = Through the Fire and Flames (difícil)
                                pathCancion = "assets/songs/ttfaf_myth_hard_filtered.csv";
                            }
                            else
                            {
                                // Fallback por si las dudas
                                pathCancion = "assets/songs/around_the_world_osu_easy_filtered_v2.csv";
                            }

                            // 2) MP3 a reproducir segun el nivel
                            std::string pathMusica;
                            if (nivelSeleccionado == 1)
                            {
                                pathMusica = "assets/songs/ATW.mp3";   // Around the World
                            }
                            else if (nivelSeleccionado == 2)
                            {
                                pathMusica = "assets/songs/BIB.mp3";   // Back in Black
                            }
                            else if (nivelSeleccionado == 3)
                            {
                                pathMusica = "assets/songs/TTFAF.mp3"; // Through the Fire and Flames
                            }
                            else
                            {
                                pathMusica = "assets/songs/ATW.mp3";
                            }

                            // Pasamos a estado JUGANDO con cuenta regresiva
                            subMenu.ocultar();
                            estadoActual = JUGANDO;
                            introMusic.stop();

                            // Recargamos la música del nivel antes de reproducir
                            musicaJuego.stop();
                            musicaJuego.load(pathMusica);

                            scoring.reiniciarScoring();

                            // Cargamos/recargamos la canción (CSV) del nivel elegido
                            if (!cancion.cargarDesdeCSV(pathCancion))
                            {
                                std::cout << "Advertencia: no se pudo cargar "
                                          << pathCancion
                                          << ". El sistema de nodos no funcionara correctamente."
                                          << std::endl;
                            }

                            // Contador de 3,2,1
                            enCuentaRegresiva    = true;
                            valorCuentaRegresiva = 3;
                            relojCuentaRegresiva.restart();
                            partidaActiva        = false;

                            std::cout << "Iniciando cuenta regresiva - Nivel "
                                      << nivelSeleccionado << " - Cancion "
                                      << cancionSeleccionada << std::endl;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Escape)
                    {
                        // ESC desde selección de nivel vuelve al menú principal
                        std::cout << "Cerrando submenu de nivel..." << std::endl;
                        subMenu.ocultar();
                    }
                }
            }
            // Si el submenu NO está activo (menú principal)
            else
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::W)
                    {
                        menu.moverArriba();
                    }
                    else if (event.key.code == sf::Keyboard::S)
                    {
                        menu.moverAbajo();
                    }
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        int opcion = menu.getOpcionSeleccionada();

                        if (opcion == 0)
                        {
                            // JUGAR -> abrir selección de nivel (1,2,3)
                            std::cout << "Abriendo seleccion de nivel..." << std::endl;
                            subMenu.mostrarSeleccionNivel();
                        }
                        else if (opcion == 1)
                        {
                            estadoActual = CONTROLES;
                        }
                        else if (opcion == 2)
                        {
                            estadoActual = PUNTAJES;
                        }
                        else if (opcion == 3)
                        {
                            window.close();
                        }
                    }
                }
            }
            break;


        case JUGANDO:
            // Si el submenu esta activo (pausa o advertencia)
            if (subMenu.estaActivo())
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::W)
                    {
                        subMenu.moverArriba();
                    }
                    else if (event.key.code == sf::Keyboard::S)
                    {
                        subMenu.moverAbajo();
                    }
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        int opcion = subMenu.getOpcionSeleccionada();

                        if (subMenu.getTipo() == SubMenu::PAUSA)
                        {
                            if (opcion == 0)
                            {
                                // Continuar - Cerrar menu de pausa
                                subMenu.ocultar();
                                musicaJuego.play();
                                std::cout << "Continuando juego..." << std::endl;
                            }
                            else if (opcion == 1)
                            {
                                // Volver al menu - Mostrar advertencia
                                subMenu.mostrarAdvertenciaSalir();
                                std::cout << "Mostrando advertencia..." << std::endl;
                            }
                            else if (opcion == 2)
                            {
                                // Opciones
                                std::cout << "Opciones en desarrollo..." << std::endl;
                            }
                        }
                        else if (subMenu.getTipo() == SubMenu::ADVERTENCIA_SALIR)
                        {
                            if (opcion == 0)
                            {
                                // No, continuar jugando - Volver a pausa
                                subMenu.mostrarMenuPausa();
                                std::cout << "Cancelando salida..." << std::endl;
                            }
                            else if (opcion == 1)
                            {
                                // Sí, volver al menu - Salir al menu principal
                                subMenu.ocultar();
                                estadoActual = MENU;
                                musicaJuego.stop();
                                introMusic.play();
                                std::cout << "Volviendo al menu principal..." << std::endl;
                            }
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Escape)
                    {
                        // ESC solo funciona en pausa para continuar
                        if (subMenu.getTipo() == SubMenu::PAUSA)
                        {
                            subMenu.ocultar();
                            musicaJuego.play();
                            std::cout << "Continuando juego (ESC)..." << std::endl;
                        }
                    }
                }
            }
            // Si el submenu NO esta activo (jugando normal)
            else
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        // Abrir menu de pausa
                        subMenu.mostrarMenuPausa();
                        musicaJuego.pause();
                        std::cout << "Juego pausado..." << std::endl;
                    }
                }
                player.manejadorEventos(event);
            }
            break;

        case CONTROLES:
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    estadoActual = MENU;
                }
            }
            break;

        case PUNTAJES:
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    estadoActual = MENU;
                }
            }
            break;

        case SALIR:
            window.close();
            break;
        }
    }
}

void Juego::actualizar()
{
    // Update
    deltaTiempo = clock.restart().asSeconds(); // Tomo el tiempo transcurrido entre frames para pasarselo al objeto Player

    if (estadoActual == JUGANDO && !subMenu.estaActivo())
    {
        // Mientras no haya empezado la partida, la nota actual la dejamos en 0
        int indiceNota = partidaActiva ? gestorNodos.siguienteNota() : 0;
        player.update(deltaTiempo, indiceNota);

        // --- Cuenta regresiva 3-2-1 ---
        if (enCuentaRegresiva)
        {
            float elapsed = relojCuentaRegresiva.getElapsedTime().asSeconds();
            int nuevoValor = 3 - static_cast<int>(elapsed);
            if (nuevoValor >= 0)
            {
                valorCuentaRegresiva = nuevoValor;
            }

            if (elapsed >= 3.f)
            {
                // Termina la cuenta regresiva: arrancan musica y nodos
                enCuentaRegresiva = false;
                partidaActiva     = true;

                musicaJuego.play();
                gestorNodos.iniciar(&player, &cancion, configRitmo, &scoring);
            }
        }
    }
}


// Dibujar
void Juego::dibujar()
{
    // Draw
    window.clear();

    switch (estadoActual)
    {
    case MENU:
    {
        window.setView(viewUI);  // Cambio a vista Menu
        menu.dibujar(window);
        subMenu.dibujar(window); // Dibujar submenu encima si esta activo
        if(!subMenu.estaActivo())
        {
            sf::Text titulo;
            titulo.setFont(fuenteSecundaria); // <--- Usamos la nueva fuente "comic.ttf"
            titulo.setString("LABERINTO RITMICO");
            titulo.setCharacterSize(50); // Tamaño grande
            titulo.setFillColor(sf::Color(255, 220, 0)); // Un amarillo/dorado brillante
            titulo.setOutlineColor(sf::Color::Black); // Contorno negro para resaltarlo
            titulo.setOutlineThickness(4.f); // Grosor del contorno

            // Centrar el título
            sf::FloatRect bounds = titulo.getLocalBounds();
            titulo.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
            titulo.setPosition(400, 100); // Posición X centrada, Y más arriba para dejar espacio

            window.draw(titulo);
        }
        subMenu.dibujar(window);
        break;
    }

    case JUGANDO:
    {
        // Agregar Imagenes del Gameplay
        viewGame.setCenter(player.getPosMundo()); // Centro la vista sobre el jugador. (mejora el seguimiento del dash)
        window.setView(viewGame);

        // Prefetch de chunks vecinos
        const sf::Vector2i grid = player.getPosGrilla();
        map.draw2x2Framed(window, grid);

        const sf::Vector2f world    = player.getPosMundo();
        const sf::Vector2f centerG = { grid.x * Map::CELL, grid.y * Map::CELL };
        const sf::Vector2f delta   = world - centerG;

        const float TH = Map::CELL * 0.25f;
        int sx = (delta.x >  TH) ? +1 : (delta.x < -TH ? -1 : 0);
        int sy = (delta.y >  TH) ? +1 : (delta.y < -TH ? -1 : 0);

        if (sx != 0) map.draw2x2Framed(window, grid + sf::Vector2i(sx, 0));
        if (sy != 0) map.draw2x2Framed(window, grid + sf::Vector2i(0, sy));
        if (sx != 0 && sy != 0) map.draw2x2Framed(window, grid + sf::Vector2i(sx, sy));

        window.draw(player); // Dibujo Player

        bool continuarJugando = true;
        if (!subMenu.estaActivo())
        {
            if (partidaActiva)
            {
                // GestorNodos maneja nodos + sincronizacion con la Cancion
                continuarJugando = gestorNodos.actualizar(window, deltaTiempo);

                if (!continuarJugando)
                {
                    scoring.calcularPuntuacionTotal(nivelSeleccionado);
                    estadoActual = MENU;
                    // Detener musica del juego y reiniciar la del intro
                    musicaJuego.stop();
                    introMusic.play();

                    if (scoringArchivo.guardarNuevaPuntuacion(scoring))
                    {
                        std::cout << "Puntuacion guardada correctamente.";
                    }
                    else std::cout << "Error al guardar la puntuacion.";
                }
                scoringArchivo.leerPuntuaciones(mejoresPuntuaciones);
            }
        }

        if (subMenu.estaActivo())
        {
            // VOLVEMOS A LA VISTA FIJA
            window.setView(viewUI);
            // DIBUJAMOS EL MENÚ
            subMenu.dibujar(window);
        }

        // --- Dibujar cuenta regresiva 3-2-1 ---
        if (enCuentaRegresiva)
        {
            // Usamos la vista actual (viewGame) para centrar el texto
            sf::View viewActual = window.getView();
            sf::Vector2f centro = viewActual.getCenter();

            sf::Text countdownText;
            countdownText.setFont(fuenteSecundaria);
            countdownText.setCharacterSize(80);
            countdownText.setFillColor(sf::Color::White);
            countdownText.setOutlineColor(sf::Color::Black);
            countdownText.setOutlineThickness(4.f);

            int valorMostrar = valorCuentaRegresiva;
            if (valorMostrar < 1) valorMostrar = 1;

            countdownText.setString(std::to_string(valorMostrar));

            sf::FloatRect bounds = countdownText.getLocalBounds();
            countdownText.setOrigin(bounds.left + bounds.width  / 2.0f,
                                    bounds.top  + bounds.height / 2.0f);
            countdownText.setPosition(centro);

            window.draw(countdownText);
        }
        break;
    }

    case PUNTAJES:
        window.setView(viewUI);

        {
            sf::RectangleShape fondoPuntajes(sf::Vector2f(window.getSize()));
            fondoPuntajes.setFillColor(sf::Color(20, 20, 30)); // Azul muy oscuro
            window.draw(fondoPuntajes);
        }

        dibujarTexto("TOP 10 MEJORES", 400, 60, 45, sf::Color(255, 200, 100));
        dibujarTexto("Pos      Puntaje       Precision", 400, 130, 22, sf::Color::Cyan);

        for (int i = 0; i < 10; i++)
        {
            float puntos = mejoresPuntuaciones[i].getPuntuacionTotal();
            float msPromedio = mejoresPuntuaciones[i].getPromedioMsAterrizaje();

            float posY = 170 + (i * 35);

            sf::Color colorTexto = sf::Color::White;
            if (i == 0) colorTexto = sf::Color(255, 215, 0);
            else if (i == 1) colorTexto = sf::Color(192, 192, 192);
            else if (i == 2) colorTexto = sf::Color(205, 127, 50);

            if (puntos > 0)
            {
                std::string linea = std::to_string(i + 1) + ".      " +
                                    std::to_string((int)puntos) + " pts      " +
                                    "(" + std::to_string((int)msPromedio) + "ms)";

                dibujarTexto(linea, 400, posY, 24, colorTexto);
            }
            else
            {
                std::string linea = std::to_string(i + 1) + ".      - - - - -";
                dibujarTexto(linea, 400, posY, 24, sf::Color(100, 100, 100));
            }
        }

        {
            sf::RectangleShape linea(sf::Vector2f(600, 2));
            linea.setFillColor(sf::Color(100, 100, 100));
            linea.setOrigin(300, 1);
            linea.setPosition(400, 540);
            window.draw(linea);
        }

        dibujarTexto("Presione ESC para volver", 400, 560, 18, sf::Color(200, 200, 200));
        break;
    }

    window.display();
}

void Juego::dibujarTexto(std::string mensaje, float x, float y, int tamano, sf::Color color)
{
    sf::Text texto;
    texto.setFont(fuentePrincipal);
    texto.setString(mensaje);
    texto.setCharacterSize(tamano);
    texto.setFillColor(color);

    sf::FloatRect bounds = texto.getLocalBounds();
    texto.setOrigin(bounds.left + bounds.width / 2.0f,
                    bounds.top + bounds.height / 2.0f);
    texto.setPosition(x, y);

    window.draw(texto);
}
