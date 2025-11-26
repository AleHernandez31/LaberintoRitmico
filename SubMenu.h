#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class SubMenu {

public:
    SubMenu();
    ~SubMenu();

    enum TipoSubMenu {
        NINGUNO,
        SELECCION_NIVEL,
        SELECCION_CANCION,
        PAUSA,
        ADVERTENCIA_SALIR,
        FIN_NIVEL
    };

    void mostrarSeleccionNivel();
    void mostrarSeleccionCancion(int nivel);// Recibe el nivel seleccionado
    void mostrarMenuPausa();
    void mostrarAdvertenciaSalir();
    void ocultar();
    void mostrarFinNivel(int puntuacionTotal);

    bool estaActivo() const;
    TipoSubMenu getTipo() const;

    void moverArriba();
    void moverAbajo();
    int getOpcionSeleccionada() const;

    void dibujar(sf::RenderWindow& window);
private:

    sf::Font font;

    // Fondo del submenu
    sf::RectangleShape fondo;
    sf::RectangleShape overlay;

    // Titulo del submenú
    sf::Text titulo;
    sf::Text textoPuntuacion;
    sf::Text* textos;// Puntero al array dinamico de textos
    int capacidadTextos;
    // Opciones

    int opcionSeleccionada;
    int totalOpciones;

    // Estado
    TipoSubMenu tipoActual;
    bool activo;

    // Configuracion visual
    void configurarEstilo();

    void liberarTextos();
    void crearTextos(int cantidad);
    void agregarTexto(const std::string& texto, float posX, float posY,
                      int tamanio, sf::Color color, int indice);

};
