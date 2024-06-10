//
// Created by s5304091
//
#include "laser.h"
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>

// Constructor implementation
Laser::Laser(const ngl::Vec3& _pos, const ngl::Vec3& _dir)
        : m_position(_pos), m_speed(20.0f), m_radius(1.0f) // Speed can be adjusted as necessary
{
    ngl::Vec3 dir = _dir;
    dir.normalize();
    m_direction = dir;
}

// Update method implementation
void Laser::update(float _deltaTime)
{
    m_position += m_direction * m_speed * _deltaTime; // Move the laser
}

// Draw method implementation
void Laser::draw(const ngl::Mat4& _view, const ngl::Mat4& _proj) const
{
    ngl::ShaderLib::use("Phong"); // Ensure the correct shader is used
    ngl::Transformation transform;
    transform.setPosition(m_position);
    ngl::Mat4 MVP = _proj * _view * transform.getMatrix();
    ngl::ShaderLib::setUniform("MVP", MVP);
    ngl::VAOPrimitives::draw("cube"); // Draw the laser as a cube
}

bool Laser::isOutOfBounds(float _yExtent) const
{
    return m_position.m_y > _yExtent || m_position.m_y < -_yExtent;
}

ngl::Vec3 Laser::getPosition() const
{
    return m_position;
}

float Laser::getRadius() const
{
    return m_radius;
}

