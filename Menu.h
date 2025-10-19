#include <SFML/Graphics.hpp>

class Menu {
private:
    //Imagen menu
    sf::Texture tex;
    sf::Sprite image;
    // Botones
    sf::Text textoJugar;
    sf::Text textoSalir;
    sf::Font font;
    int opcionSeleccionada = 0;
public:
    Menu();
    void dibujar(sf::RenderWindow& window);
    void moverArriba();
    void moverAbajo();
    int getOpcionSeleccionada();
};


