//
// Created by s5304091
//

#ifndef SINEWAVEENEMY_H
#define SINEWAVEENEMY_H

#include "Enemy.h"

class SineWaveEnemy : public Enemy{
public:

    SineWaveEnemy(const ngl::Vec3 &startPosition, float amplitude, float frequency);

    void update(float deltaTime) override;

private:
    ngl::Vec3 m_startPosition;
    float m_amplitude;
    float m_frequency;
    float m_waveTimer;
};



#endif //SINEWAVEENEMY_H
