#include "Vida.h"

Vida::Vida() {
    _nivel = 100.f;
}



void Vida::setAudioControl(AudioControl &audioControl) {
    _audioControl = &audioControl;
}



void Vida::reducirVida(){
    if (_nivel > 0.f) {
        _nivel -= 10.f;
        if (_nivel < 0.f) _nivel = 0.f;

        if (_audioControl != nullptr) {
            _audioControl->setVolume(_nivel);
        }
        std::cout << "Nivel ajustado a: " << _nivel << std::endl;
    }
}




void Vida::aumentarVida() {
    if (_nivel < 100.f) {
        _nivel += 25.f;
        if (_nivel > 100.f) _nivel = 100.f;

        if (_audioControl != nullptr) {
            _audioControl->setVolume(_nivel);
        }
        std::cout << "Nivel ajustado a: " << _nivel << std::endl;
    }
}




float Vida::getVida() {
    return _nivel;
}



void Vida::restaurarVida() {
    if (_nivel != 100.f) {
        _nivel = 100.f;
        if (_audioControl != nullptr) {
            _audioControl->setVolume(_nivel);
        }
        std::cout << "Vida restaurada a: " << _nivel << std::endl;
    }
}

