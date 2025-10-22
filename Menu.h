#include <SFML/Graphics.hpp>
#include <vector>

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

    // Solo textos
    std::vector<sf::Text> textos;

    int opcionSeleccionada;
    int totalOpciones;

public:
    Menu();
    void dibujar(sf::RenderWindow& window);
    void moverArriba();
    void moverAbajo();
    int getOpcionSeleccionada();
};
