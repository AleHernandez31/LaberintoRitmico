#pragma once
#include <iostream>
#include "AudioControl.h"

class Vida {
    public:
        Vida();
        void setAudioControl(AudioControl &audioControl);
        void reducirVida();
        void aumentarVida();
        void restaurarVida();
        float getVida();

    private:
        AudioControl* _audioControl = nullptr;
        float _nivel;
};
