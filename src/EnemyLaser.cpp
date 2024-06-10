//
// Created by s5304091
//
#include "EnemyLaser.h"


void EnemyLaser::update(float deltaTime)
{
    m_position.m_y -= m_speed * deltaTime;  // Move the laser downward
}

EnemyLaser::EnemyLaser(const ngl::Vec3 &pos, const ngl::Vec3 &dir,  float speed): Laser(pos,dir)
{
}
