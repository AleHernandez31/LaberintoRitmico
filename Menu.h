#include <SFML/Graphics.hpp>

// Enum de estados del juego (Asignacion de nombres a los numeros)

enum EstadoJuego {
    MENU,
    JUGANDO,
    CONTROLES,
    PUNTAJES,
    SALIR
};

class Menu {
private:
    sf::Texture tex;
    sf::Sprite image;
    sf::Font font;

    sf::Text* textos;

    int opcionSeleccionada;
    int totalOpciones;

public:
    Menu();
    ~Menu();

    void dibujar(sf::RenderWindow& window);
    void moverArriba();
    void moverAbajo();
    int getOpcionSeleccionada();
};

