//
// Created by s5304091
//
#include "Enemy.h"
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>


Enemy::Enemy(const ngl::Vec3 &startPosition, float speed, int health, float radius) : m_pos(startPosition), m_speed(speed), m_radius(radius), m_health(health),m_isHit(
        false)
{
}

void Enemy::update(float deltaTime)
{
    m_pos.m_y -= m_speed * deltaTime; // move down from the top of the window
}

void Enemy::draw(const ngl::Mat4 &_view , const ngl::Mat4 &_project)
{

    ngl::ShaderLib::use("Phong"); // Ensure the correct shader is used
    ngl::Transformation transform;
    transform.setPosition(m_pos);
    transform.setScale(5.0f,5.0f,5.0f);
    ngl::Mat4 MVP = _project * _view * transform.getMatrix();
    ngl::ShaderLib::setUniform("MVP", MVP);
    ngl::VAOPrimitives::draw("troll"); // Draw the laser as a cube
}

ngl::Vec3 Enemy::getPosition()const
{
    return m_pos;
}

Enemy Enemy::spawnAt(const ngl::Vec3 &position)
{
    return Enemy(position);
}

bool Enemy::checkCollision(const ngl::Vec3 &otherPos, float otherRadius)
{
    float distance = (m_pos - otherPos).length();
    return distance <= (m_radius+otherRadius);
}

void Enemy::decreaseHealth(int amount)
{
    m_health -= amount;
}

float Enemy::get_Radius()
{
    return m_radius;
}

bool Enemy::isDestroyed()
{
    return m_health<=0;
}

void Enemy::getHit()
{
    m_isHit = true;
}

bool Enemy::hited() const
{
    return m_isHit;
}

void Enemy::move(float deltaTime)
{

}

