#ifndef LASER_H
#define LASER_H

#include <ngl/Vec3.h>
#include <ngl/Mat4.h>

class Laser
{
public:
    // Constructor
    Laser(const ngl::Vec3& _pos, const ngl::Vec3& _dir);

    // Update the laser's position based on its velocity
    virtual void update(float _deltaTime);

    [[nodiscard]] bool isOutOfBounds(float _yExtent) const;

    // Draw the laser
    void draw(const ngl::Mat4& _view, const ngl::Mat4& _proj) const;
    [[nodiscard]] ngl::Vec3 getPosition() const;
    [[nodiscard]] float getRadius() const;

protected:
    ngl::Vec3 m_position;   // Current position of the laser
    ngl::Vec3 m_direction;  // Direction of the laser, should be normalized
    float m_speed;          // Speed of the laser
    float m_radius;
};

#endif //LASER_H
