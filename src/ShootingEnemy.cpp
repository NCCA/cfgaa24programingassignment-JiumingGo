//
// Created by s5304091
//

#include "ShootingEnemy.h"
#include <iostream>
ShootingEnemy::ShootingEnemy(const ngl::Vec3 &startPosition, float speed, int health, float radius) : Enemy(
        startPosition, speed, health, radius),
        m_fireRate(1.5f),
        m_timeSinceLastShot(0.0f)
{
}

void ShootingEnemy::update(float deltaTime)
{
//    std::cout <<"Enemy shoot delta time  update " << deltaTime << std::endl;
    Enemy::update(deltaTime);
    m_timeSinceLastShot += deltaTime;
//    std::cout <<"Enemy shoot time update " << m_timeSinceLastShot << std::endl;
    if (m_timeSinceLastShot >= m_fireRate)
    {
        std::cout <<"Enemy shoot!\n";
        firelaser();
        m_timeSinceLastShot = 0.0f;
    }
    for (auto& laser : m_lasers)
    {
        laser.update(deltaTime);
    }
}

void ShootingEnemy::firelaser()
{
    ngl::Vec3 laserStartPosition = m_pos;
    laserStartPosition.m_y -= 1.0f;
    m_lasers.emplace_back(laserStartPosition, ngl::Vec3(0, -1, 0),10.0f);
}

const std::vector<EnemyLaser> &ShootingEnemy::getLaser() const
{
    return m_lasers;
}

bool ShootingEnemy::checkLaserCollision(SpaceShip &ship)
{
    bool hit = false;
    auto it = m_lasers.begin();
    while(it != m_lasers.end())
    {
        if (ship.checkCollision(it->getPosition(),it->getRadius()))
        {
            hit = true;
            ship.decreaseHealth(1);
            it = m_lasers.erase(it);
        }
        else
        {
            ++it;
        }
    }
    return hit;
}

