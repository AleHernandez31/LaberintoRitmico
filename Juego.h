#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "SubMenu.h"
#include "Player.h"
#include "AudioControl.h"
#include "Mapa.h"
#include "PrototipoFuncionalidad.h"

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
    PrototipoFuncionalidad protoFunc;
    ConfigRitmo configRitmo;

    // Variables de estado
    int nivelSeleccionado;
    int cancionSeleccionada;
    float deltaTiempo;
};
