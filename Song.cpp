#include <String>
#include <iostream>
#include "Song.h"

/*
    Parametros:
        path: ruta de la cancion
        loop: indica si se debe repetir la canción
        isMenu: lo usamos para determinar si usamos sf::Music o sf::Sound (Music se carga desde disco
                                                                        y Sound desde memoria)
*/
Song::Song(std::string path, bool isMenu)
    : _isMenu(isMenu) {

    if (_isMenu) {
        if (!_music.openFromFile(path)) {
            std::cout << "Error al cargar la musica del menu.";
            exit(101);
        }
        _music.setLoop(true);
    }

    if (!_isMenu) {
        if (!_soundBuffer.loadFromFile(path)) {
            std::cout << "Error al cargar la musica del menu.";
            exit(101);
        }
        _sound.setBuffer(_soundBuffer);
    }
}


void Song::play() {
    _isMenu ? _music.play() : _sound.play();
}


void Song::stop() {
    _isMenu ? _music.stop() : _sound.stop();
}


void Song::pause() {
    _isMenu ? _music.pause() : _sound.pause();
}


void Song::setVolume(float volume) {
    _isMenu ? _music.setVolume(volume) : _sound.setVolume(volume);
}
