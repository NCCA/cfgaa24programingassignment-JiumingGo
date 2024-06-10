#include "SpaceShip.h"
#include <ngl/ShaderLib.h>
#include <iostream>

SpaceShip::SpaceShip( ngl::Vec3 _pos, std::string _fname )
 {
   m_pos=_pos;
   m_mesh.reset( new ngl::Obj(_fname));
   m_mesh->createVAO();
   m_rotation=0;
   m_radius = 2.5f;
   m_health = 10;
 }

void SpaceShip::draw(const std::string &_shader,  const ngl::Mat4 &_view , const ngl::Mat4 &_project)
{
  ngl::ShaderLib::use(_shader);
	m_transform.setPosition(m_pos);
	m_transform.setRotation(0,m_rotation,0);
  ngl::Mat4 MVP= _project*_view*m_transform.getMatrix()  ;
  ngl::ShaderLib::setUniform("MVP",MVP);
  m_mesh->draw();
  //Draw all lasers
  for (auto& laser: m_lasers)
  {
      laser.draw(_view, _project);
  }
}
//Original s_xExtents = 45; s_yExtents = 30
//const static float s_xExtents=45;
//const static float s_yExtents=30;
const static float s_xExtents=20;
const static float s_yExtents=35;
void SpaceShip::move( float _x,float _y )
{
	float currentX=m_pos.m_x;
	float currentY=m_pos.m_y;
	m_pos.m_x+=_x;
	m_pos.m_y+=_y;
	if(m_pos.m_x<=-s_xExtents || m_pos.m_x>=s_xExtents)
	{
		m_pos.m_x=currentX;
	}


	if(m_pos.m_y<=-s_yExtents || m_pos.m_y>=s_yExtents)
	{
		m_pos.m_y=currentY;
	}
}

void SpaceShip::rotate(float _rot )
{
	m_rotation+=_rot;
}

ngl::Vec3 SpaceShip::getLaserPosition()
{
    ngl::BBox bbox = m_mesh->getBBox();
    //TO get ship's height
    float height = bbox.maxY() - bbox.minY();
    ngl::Vec3 muzzle = m_pos + ngl::Vec3(0,height,0);
    return muzzle;
}

void SpaceShip::fireLaser()
{
    ngl::Vec3 laserStart = getLaserPosition();
    auto laser_x = laserStart.m_x;
    auto laser_y = laserStart.m_y;
    auto laser_z = laserStart.m_z;
    ngl::Vec3 laserDirection = ngl::Vec3(0,1,0);
    m_lasers.emplace_back(laserStart,laserDirection);
    std::cout << "Laser from: X=" << laserStart.m_x << ", Y=" << laserStart.m_y << ", Z=" << laserStart.m_z << std::endl;
}

void SpaceShip::update(float _deltaTime)
{
    // Update each laser's position
    for (auto& laser : m_lasers)
    {
        laser.update(_deltaTime);
    }

    // Remove lasers that have gone out of bounds
    m_lasers.erase(std::remove_if(m_lasers.begin(), m_lasers.end(),
                                  [](const Laser& l) { return l.isOutOfBounds(s_yExtents); }),
                   m_lasers.end());
}

bool SpaceShip::checkCollision(const ngl::Vec3 &otherPos, float otherRadius)
{
    float dist = (m_pos - otherPos).length();
    return  dist < (m_radius + otherRadius);
}

void SpaceShip::decreaseHealth(int amount)
{
    m_health -= amount;
}

int SpaceShip::getHealth()
{
    return m_health;
}

bool SpaceShip::checkLaserCollision(Enemy &enemy)
{
    bool hit = false;
    auto it = m_lasers.begin();
    while (it != m_lasers.end())
    {
        if (enemy.checkCollision(it->getPosition(),it->getRadius()))
        {
            hit = true;
            enemy.decreaseHealth(1);
            it = m_lasers.erase(it);
        }
        else
        {
            ++it;
        }
    }
    return hit;
}











