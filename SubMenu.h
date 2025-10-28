#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class SubMenu {
public:
    enum TipoSubMenu {
        NINGUNO,
        SELECCION_NIVEL,
        SELECCION_CANCION
    };

private:
    sf::Font font;

    // Fondo del submenu
    sf::RectangleShape fondo;
    sf::RectangleShape overlay;

    // Titulo del submenu
    sf::Text titulo;

    // Opciones
    std::vector<sf::Text> textos;
    int opcionSeleccionada;
    int totalOpciones;

    // Estado
    TipoSubMenu tipoActual;
    bool activo;

    // Configuracion visual
    void configurarEstilo();

public:
    SubMenu();

    void mostrarSeleccionNivel();
    void mostrarSeleccionCancion(int nivel);  // Recibe el nivel seleccionado
    void ocultar();

    bool estaActivo() const;
    TipoSubMenu getTipo() const;

    void moverArriba();
    void moverAbajo();
    int getOpcionSeleccionada() const;

    void dibujar(sf::RenderWindow& window);
};
