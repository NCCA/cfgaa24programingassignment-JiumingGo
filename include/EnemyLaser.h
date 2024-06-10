//
// Created by s5304091
//

#ifndef ENEMYLASER_H
#define ENEMYLASER_H

#include "laser.h"

class EnemyLaser: public Laser{
public:
    EnemyLaser(const ngl::Vec3 &pos, const ngl::Vec3 &dir, float speed);
    void update(float deltaTime) override;
};

#endif //ENEMYLASER_H
