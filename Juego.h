#pragma once
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "SubMenu.h"
#include "Player.h"
#include "AudioControl.h"
#include "Mapa.h"
#include "GestorNodos.h"
#include "Cancion.h"
#include "Scoring.h"
#include "ScoringArchivo.h"

// (EstadoJuego viene de Menu.h)

class Juego {
public:
    //configuración inicial
    Juego();

    // El metodo que contiene el "while(window.isOpen())"
    void iniciarBucle();

private:

    void procesarEventos();
    void actualizar();
    void dibujar();

    // Variables Miembro
    sf::RenderWindow window;
    sf::View viewUI;
    sf::View viewGame;

    sf::Font fuentePrincipal;
    sf::Font fuenteSecundaria;
    sf::Clock clock;

    // Estado
    EstadoJuego estadoActual;

    // Objetos del Juego
    Menu menu;
    SubMenu subMenu;
    Map map;
    Player player;
    AudioControl musicaJuego;
    AudioControl introMusic;
    Cancion cancion;
    GestorNodos gestorNodos;
    ConfigRitmo configRitmo;
    Scoring scoring;
    ScoringArchivo scoringArchivo;
    Scoring mejoresPuntuaciones[10];

    void dibujarTexto(std::string mensaje, float x, float y, int tamano, sf::Color color);

    // Variables de estado
    int nivelSeleccionado;
    int cancionSeleccionada;
    float deltaTiempo;

    // Cuenta regresiva antes de iniciar el nivel
    bool enCuentaRegresiva;
    int  valorCuentaRegresiva;
    sf::Clock relojCuentaRegresiva;
    bool partidaActiva; // true cuando la musica y GestorNodos estan corriendo
};
