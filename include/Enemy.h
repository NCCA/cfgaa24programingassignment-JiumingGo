//
// Created by s5304091
//

#ifndef ENEMY_H
#define ENEMY_H

#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
class Enemy{
public:
    explicit Enemy(const ngl::Vec3 &startPosition, float speed = 20.0f, int health = 1,float radius = 1.5f);
    virtual ~Enemy() = default;
    virtual void update(float deltaTime);
    virtual void draw(const ngl::Mat4 &_view , const ngl::Mat4 &_project);
    [[nodiscard]] ngl::Vec3 getPosition() const;
    static Enemy spawnAt(const ngl::Vec3& position);
    bool checkCollision(const ngl::Vec3 &otherPos, float otherRadius);
    void decreaseHealth(int amount);
    float get_Radius();
    bool isDestroyed();
    void getHit();
    [[nodiscard]] bool hited() const;
protected:
    ngl::Vec3 m_pos;
    float m_speed;
    //collision detection
    float m_radius;
    int m_health;
    bool m_isHit;
    //Helper function for derived classes to use
    virtual void move(float deltaTime);
};


#endif //ENEMY_H
