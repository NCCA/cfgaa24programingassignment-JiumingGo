//
// Created by s5304091
//

#ifndef SHOOTINGENEMY_H
#define SHOOTINGENEMY_H
#include "Enemy.h"
#include "EnemyLaser.h"
#include "SpaceShip.h"
class ShootingEnemy : public Enemy
{
public:
    ShootingEnemy(const ngl::Vec3 &startPosition, float speed, int health, float radius);
    void update(float deltaTime) override;
    void firelaser();
    [[nodiscard]] const std::vector<EnemyLaser>& getLaser() const;
    bool checkLaserCollision(SpaceShip &ship);
private:
    std::vector<EnemyLaser> m_lasers;
    float m_fireRate;
    float m_timeSinceLastShot;
};

#endif //SHOOTINGENEMY_H
