#include <QMouseEvent>
#include <QGuiApplication>
#include <QApplication>
#include "NGLScene.h"
#include <ngl/Transformation.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <iostream>
#include <QPainter>

constexpr static float s_shipUpdate = 0.2f;
//For enemy spawn
const int WINDOW_WIDTH = 10.0f; // Not use
const int NUM_SPAWN_POINTS = 5;
float spawnPoints[NUM_SPAWN_POINTS];
const float TOP_OF_WINDOW = 30.0f;
//Pointer for background
QOpenGLTexture *backgroundTexture = nullptr;

NGLScene::NGLScene()
{

//  setTitle("Game style key control in Qt");
    setTitle("3D Space Invader");
  // now we start a timer and assign it to the m_updateShipTimer variable
  // this will be triggered at an interval of every 5ms
  // to stop this timer we call killTimer(m_animationTimer) we can have
  // multiple timers but each must have it's own ID
//  m_updateShipTimer = startTimer(2);
//  m_redrawTimer = startTimer(20);
  // now clear the key set
  m_keysPressed.clear();
}

NGLScene::~NGLScene()
{
  std::cout << "Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL(int _w, int _h)
{
  m_project = ngl::perspective(45.0f, static_cast<float>(_w) / _h, 0.05f, 350.0f);
  m_win.width = static_cast<int>(_w * devicePixelRatio());
  std::cout << "window width is: " << m_win.width << "\n";
  m_win.height = static_cast<int>(_h * devicePixelRatio());
    std::cout << "window height is: " << m_win.height << "\n";
}

void NGLScene::initializeGL()
{
  // we must call this first before any other GL commands to load and link the
  // gl commands from the lib, if this is not done program will crash
  ngl::NGLInit::initialize();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // black Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  // we are creating a shader called Phong
  ngl::ShaderLib::createShaderProgram("Phong");
  // now we are going to create empty shaders for Frag and Vert
  ngl::ShaderLib::attachShader("PhongVertex", ngl::ShaderType::VERTEX);
  ngl::ShaderLib::attachShader("PhongFragment", ngl::ShaderType::FRAGMENT);
  // attach the source
  ngl::ShaderLib::loadShaderSource("PhongVertex", "../shaders/PhongVertex.glsl");
  ngl::ShaderLib::loadShaderSource("PhongFragment", "../shaders/PhongFragment.glsl");
  // compile the shaders
  ngl::ShaderLib::compileShader("PhongVertex");
  ngl::ShaderLib::compileShader("PhongFragment");
  // add them to the program
  ngl::ShaderLib::attachShaderToProgram("Phong", "PhongVertex");
  ngl::ShaderLib::attachShaderToProgram("Phong", "PhongFragment");

  // now we have associated this data we can link the shader
  ngl::ShaderLib::linkProgramObject("Phong");

  // and make it active ready to load values
  ngl::ShaderLib::use("Phong");
  ngl::Vec4 lightPos(0.0f, 0.0f, -2.0f, 0.0f);
  ngl::ShaderLib::setUniform("light.position", lightPos);
  ngl::ShaderLib::setUniform("light.ambient", 0.0f, 0.0f, 0.0f, 1.0f);
  ngl::ShaderLib::setUniform("light.diffuse", 1.0f, 1.0f, 1.0f, 1.0f);
  ngl::ShaderLib::setUniform("light.specular", 0.8f, 0.8f, 0.8f, 1.0f);
  // gold like phong material
  ngl::ShaderLib::setUniform("material.ambient", 0.274725f, 0.1995f, 0.0745f, 0.0f);
  ngl::ShaderLib::setUniform("material.diffuse", 0.75164f, 0.60648f, 0.22648f, 0.0f);
  ngl::ShaderLib::setUniform("material.specular", 0.628281f, 0.555802f, 0.3666065f, 0.0f);
  ngl::ShaderLib::setUniform("material.shininess", 51.2f);

  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0, 0, -80);
  ngl::Vec3 to(0, 0, 0);
  ngl::Vec3 up(0, 1, 0);
  // now load to our new camera
  m_view = ngl::lookAt(from, to, up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_project = ngl::perspective(45, 720.0f / 576.0f, 0.05f, 350);
  ngl::ShaderLib::setUniform("viewerPos", from);
  // create our spaceship
  for (int i = 0; i < NUM_SPAWN_POINTS; ++i)
  {
//      spawnPoints[i] = WINDOW_WIDTH * (i+0.5f)/ NUM_SPAWN_POINTS;
//40.0f is from const static float s_xExtents=20;
    float step = 40.0f / 5;
    spawnPoints[i] = -20 + step * (i+0.5f);
    std::cout << "Spawn Point " << i << ": " << spawnPoints[i] << std::endl;
  }
  m_ship = std::make_unique<SpaceShip>(ngl::Vec3(0, -30, 0), "../models/SpaceShip.obj");
  //Timer need to start after the spaceship is ready.
  m_updateShipTimer = startTimer(2);
  m_redrawTimer = startTimer(20);
  m_enemySpawnTimer = startTimer(1500);
  m_sineSpawnTimer = startTimer(2500);
  m_shooterSpawnTimer = startTimer(3500);
  m_timer.start();
}

void NGLScene::loadMatricesToShader()
{
  ngl::Mat4 MVP = m_project * m_view;
  ngl::ShaderLib::setUniform("MVP", MVP);
}

void NGLScene::paintGL()
{

  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // now load these values to the shader
  //m_ship->draw("Phong", m_view, m_project);
  //To display the text and fix the texture
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  ngl::ShaderLib::use("Phong");
  loadMatricesToShader();
  if (!m_gameOver)
  {
      if(m_ship){
          m_ship->draw("Phong", m_view, m_project);
      }
      //Draw each enemy
      for (auto& enemy: m_enemies)
      {
          enemy.draw(m_view, m_project);//This is the overlapping part I think
      }

      for (auto& enemy: m_sineWaveEnemies)
      {
          enemy.draw(m_view,m_project);
      }
      for (auto& enemy:m_shootEnemies)
      {
          enemy.draw(m_view,m_project);
          for(auto& laser:enemy.getLaser())
          {
              laser.draw(m_view,m_project);
          }
      }
  }
  else
  {
      // Start QPainter for drawing text
      QPainter painter(this);
      painter.setPen(Qt::red);
      painter.setFont(QFont("Arial", 24));
      painter.drawText(Qt::AlignCenter, Qt::AlignCenter, "GAME OVER");
      painter.end();
  }

//    QString("Score: %1 Health: %2").arg(m_score).arg(m_ship->getHealth());
    QPainter painter(this);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 16));
    QString scoreText = QString("Score: %1 Health: %2").arg(m_score).arg(m_ship->getHealth());
    painter.drawText(10, 20, scoreText);
    painter.end();

}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent(QMouseEvent *_event)
{
// note the method buttons() is the button state when event was called
// that is different from button() which is used to check which button was
// pressed when the mousePress/Release event is generated
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
  auto position = _event->position();
#else
  auto position = _event->pos();
#endif
  if (m_win.rotate && _event->buttons() == Qt::LeftButton)
  {
    int diffx = position.x() - m_win.origX;
    int diffy = position.y() - m_win.origY;
    m_win.spinXFace += static_cast<int>(0.5f * diffy);
    m_win.spinYFace += static_cast<int>(0.5f * diffx);
    m_win.origX = position.x();
    m_win.origY = position.y();
    update();
  }
  // right mouse translate code
  else if (m_win.translate && _event->buttons() == Qt::RightButton)
  {
    int diffX = static_cast<int>(position.x() - m_win.origXPos);
    int diffY = static_cast<int>(position.y() - m_win.origYPos);
    m_win.origXPos = position.x();
    m_win.origYPos = position.y();
    m_modelPos.m_x += INCREMENT * diffX;
    m_modelPos.m_y -= INCREMENT * diffY;
    update();
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent(QMouseEvent *_event)
{
// that method is called when the mouse button is pressed in this case we
// store the value where the maouse was clicked (x,y) and set the Rotate flag to true
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
  auto position = _event->position();
#else
  auto position = _event->pos();
#endif
  if (_event->button() == Qt::LeftButton)
  {
    m_win.origX = position.x();
    m_win.origY = position.y();
    m_win.rotate = true;
  }
  // right mouse translate mode
  else if (_event->button() == Qt::RightButton)
  {
    m_win.origXPos = position.x();
    m_win.origYPos = position.y();
    m_win.translate = true;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent(QMouseEvent *_event)
{
  // that event is called when the mouse button is released
  // we then set Rotate to false
  if (_event->button() == Qt::LeftButton)
  {
    m_win.rotate = false;
  }
  // right mouse translate mode
  if (_event->button() == Qt::RightButton)
  {
    m_win.translate = false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent(QWheelEvent *_event)
{

  // check the diff of the wheel position (0 means no change)
  if (_event->angleDelta().x() > 0)
  {
    m_modelPos.m_z += ZOOM;
  }
  else if (_event->angleDelta().x() < 0)
  {
    m_modelPos.m_z -= ZOOM;
  }
  update();
}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::timerEvent(QTimerEvent *_event)
{
  // the usual process is to check the event timerID and compare it to
  // any timers we have started with startTimer
  if (!m_gameOver)
  {
      if (_event->timerId()==m_enemySpawnTimer)
      {
          spawnEnemy();
      }
      if (_event->timerId()==m_sineSpawnTimer)
      {
          spawnSineEnemy();
          std::cout << "SIne enemy spawn!" << "\n";
      }
      if (_event->timerId()==m_shooterSpawnTimer)
      {
          spawnShootingEnemy();
          std::cout << "Shooter enemy spawn!" << "\n";
      }
      if (_event->timerId() == m_updateShipTimer)
      {
          static float lastTime = 0;
          float currentTime = getElapsedTime(); // Get the elapsed time since the last frame
          float deltaTime = currentTime - lastTime;
          lastTime = currentTime;
          if(m_ship){
              m_ship->update(deltaTime);
          }else{
              std::cerr << "Error: m_ship is not initialezed." << std::endl;
          }
          //check collision for each enemy
          for(auto &enemy: m_enemies)
          {
              enemy.update(deltaTime);
              if (m_ship->checkLaserCollision(enemy))
              {
                  std::cout << "Enemy hit by laser!" << std::endl;
                  if (enemy.isDestroyed())
                  {
                      std::cout << "Enemy is destroyed!" << std::endl;
                      m_score += 10;
                  }
              }
              if (m_ship->checkCollision(enemy.getPosition(), enemy.get_Radius()) && !enemy.hited())
              {
                  m_ship->decreaseHealth(1);
                  enemy.decreaseHealth(1);
                  enemy.getHit();
                  std::cout<<"Collision! ship and enemy health decreased" << std::endl;
                  std::cout << "Current player health: " << m_ship->getHealth() << std::endl;
                  if (enemy.isDestroyed())
                  {
                      std::cout<<"Enemy destroyed!" << std::endl;
                  }
              }
              if (enemy.getPosition().m_y<-30 && !enemy.hited())
              {
                  std::cout<<"Enemy escaped!!!\n";
                  m_ship->decreaseHealth(1);
                  enemy.decreaseHealth(1);
                  enemy.getHit();
              }
          }
          for (auto& enemy : m_sineWaveEnemies)
          {
              enemy.update(deltaTime);
              if (m_ship->checkLaserCollision(enemy))
              {
                  std::cout << "Enemy hit by laser!" << std::endl;
                  if (enemy.isDestroyed())
                  {
                      std::cout << "Enemy is destroyed!" << std::endl;
                      m_score += 30;
                  }
              }
              if (m_ship->checkCollision(enemy.getPosition(), enemy.get_Radius()) && !enemy.hited())
              {
                  m_ship->decreaseHealth(1);
                  enemy.decreaseHealth(1);
                  enemy.getHit();
                  std::cout<<"Collision! ship and enemy health decreased" << std::endl;
                  std::cout << "Current player health: " << m_ship->getHealth() << std::endl;
                  if (enemy.isDestroyed())
                  {
                      std::cout<<"Enemy destroyed!" << std::endl;
                  }
              }
              if (enemy.getPosition().m_y<-30 && !enemy.hited())
              {
                  std::cout<<"Enemy escaped!!!\n";
                  m_ship->decreaseHealth(1);
                  enemy.decreaseHealth(1);
                  enemy.getHit();
              }
          }

          for (auto &enemy: m_shootEnemies)
          {
              enemy.update(deltaTime);
              if (m_ship->checkLaserCollision(enemy))
              {
                  std::cout << "Enemy hit by laser!" << std::endl;
                  if (enemy.isDestroyed())
                  {
                      std::cout << "Enemy is destroyed!" << std::endl;
                      m_score += 50;
                  }
              }
              if (enemy.checkLaserCollision(*m_ship))
              {
                  std::cout << "Space ship hit by laser!" << std::endl;
              }
              if (m_ship->checkCollision(enemy.getPosition(), enemy.get_Radius()) && !enemy.hited())
              {
                  m_ship->decreaseHealth(1);
                  enemy.decreaseHealth(1);
                  enemy.getHit();
                  std::cout<<"Collision! ship and enemy health decreased" << std::endl;
                  std::cout << "Current player health: " << m_ship->getHealth() << std::endl;
                  if (enemy.isDestroyed())
                  {
                      std::cout<<"Enemy destroyed!" << std::endl;
                  }
              }
              if (enemy.getPosition().m_y<-30 && !enemy.hited())
              {
                  std::cout<<"Enemy escaped!!!\n";
                  m_ship->decreaseHealth(1);
                  enemy.decreaseHealth(1);
                  enemy.getHit();
              }
          }

          // Remove destroyed sine enemies
          m_sineWaveEnemies.erase(std::remove_if(m_sineWaveEnemies.begin(), m_sineWaveEnemies.end(),
                                                 [](SineWaveEnemy& e){ return e.isDestroyed(); }),
                                  m_sineWaveEnemies.end());
          //Remove destroyed enemies
          m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(),
                                         [](Enemy& enemy) { return enemy.isDestroyed(); }), // Change to non-const Enemy reference
                          m_enemies.end());
          //Remove destroyed shooter enemies
          m_shootEnemies.erase(std::remove_if(m_shootEnemies.begin(), m_shootEnemies.end(),
                                              [](Enemy& enemy) { return enemy.isDestroyed(); }), // Change to non-const Enemy reference
                               m_shootEnemies.end());
  }


//      //Update the position of each enemy
//      for (auto& enemy : m_enemies)
//      {
//          enemy.update(deltaTime);
//      }
    moveShip(); //MOve the ship based on user inputs
      update();

  }
  if (_event->timerId() == m_redrawTimer && ! m_gameOver)
  {
      checkGameOver();
      paintGL();
  }
}

void NGLScene::moveShip()
{
  /// first we reset the movement values
  float xDirection = 0.0;
  float yDirection = 0.0;
  float rotation = 0.0;
  // now we loop for each of the pressed keys in the the set
  // and see which ones have been pressed. If they have been pressed
  // we set the movement value to be an incremental value
  foreach (Qt::Key key, m_keysPressed)
  {
    switch (key)
    {
    case Qt::Key_Left:
    {
      xDirection = s_shipUpdate;
      break;
    }
    case Qt::Key_Right:
    {
      xDirection = -s_shipUpdate;
      break;
    }
    case Qt::Key_Up:
    {
      yDirection = s_shipUpdate;
      break;
    }
    case Qt::Key_Down:
    {
      yDirection = -s_shipUpdate;
      break;
    }
    case Qt::Key_R:
    {
      rotation = 1.0;
      break;
    }
    //This is for shoot right now one press release mutiple bullets
//    case Qt::Key_Space:
//    {
//        firelaser = true;
//        break;
//    }
    default:
      break;
    }
  }
  // if the set is non-zero size we can update the ship movement
  // then tell openGL to re-draw
  if (m_keysPressed.size() != 0)
  {
    m_ship->move(xDirection, yDirection);
    m_ship->rotate(rotation);
//    if (firelaser)
//    {
//        m_ship->fireLaser();
//        firelaser = false;
//    }
  }
//  if (m_keysPressed.contains(Qt::Key_Space)&& !m_spacePressed)
//  {
////      m_ship->fireLaser();
//      m_spacePressed = true;
//  }
}
//----------------------------------------------------------------------------------------------------------------------
// in this case this method is called from the MainWindow class and passed
// the key event from that, if we didn't need to process the events in main
// window we could just overload this as the keyPressEvent and only process here.
//----------------------------------------------------------------------------------------------------------------------
void NGLScene::keyPressEvent(QKeyEvent *_event)
{
    // Debugging output
    //std::cout << "Key Pressed: " << _event->key() << std::endl;
  // add to our keypress set the values of any keys pressed
  m_keysPressed += (Qt::Key)_event->key();
  if (_event->key()==Qt::Key_Space && !m_spacePressed)
  {
      m_ship->fireLaser(); // Ensure immediate response to space key press
      m_spacePressed = true;
  }
  else if (_event->key()==Qt::Key_Escape)
  {
      QApplication::exit(EXIT_SUCCESS);
  }
//  {
//
//  switch (_event->key())
//  {
//  case Qt::Key_Escape:
//    QApplication::exit(EXIT_SUCCESS);
//    break;
//  }
}

//----------------------------------------------------------------------------------------------------------------------
// in this case this method is called from the MainWindow class and passed
// the key event from that, if we didn't need to process the events in main
// window we could just overload this as the keyPressEvent and only process here.
//----------------------------------------------------------------------------------------------------------------------
void NGLScene::keyReleaseEvent(QKeyEvent *_event)
{
    // Debugging output
    std::cout << "Key Released: " << _event->key() << std::endl;
  // remove from our key set any keys that have been released
  m_keysPressed -= (Qt::Key)_event->key();
  // manage space key
  if (_event->key()==Qt::Key_Space)
  {
      m_spacePressed = false;
  }
}

float NGLScene::getElapsedTime()
{
    return float(m_timer.elapsed()) / 1000.0f;
}

void NGLScene::spawnEnemy()
{
    //This is for debug , to check if the enemy class worked. spawn enemies at 0,50,0
//    ngl::Vec3 spawnPosition(0.0f,50.0f,0.0f);
//    m_enemies.push_back(Enemy::spawnAt(spawnPosition));
//    std::cout << "Spawn enemy at origin (0,50,0)" << std::endl;
    int spawnIndex = rand() % NUM_SPAWN_POINTS;
    ngl::Vec3 spawnPosition(spawnPoints[spawnIndex], TOP_OF_WINDOW, 0.0f);
    m_enemies.push_back(Enemy::spawnAt(spawnPosition));
}

void NGLScene::spawnSineEnemy()
{
    float amplitude = 10.0f;
    float frequency = 5.0f;
    int spawnIndex = rand() % NUM_SPAWN_POINTS;
    ngl::Vec3 spawnPosition(spawnPoints[spawnIndex], TOP_OF_WINDOW, 0.0f);
    m_sineWaveEnemies.emplace_back(spawnPosition, amplitude, frequency);
}

void NGLScene::spawnShootingEnemy()
{
    int spawnIndex = rand() % NUM_SPAWN_POINTS;
    ngl::Vec3 spawnPosition(spawnPoints[spawnIndex], TOP_OF_WINDOW, 0.0f);
    m_shootEnemies.emplace_back(spawnPosition,10,1,1.5f);
}

void NGLScene::checkGameOver()
{
    if (m_ship->getHealth() <= 0)
    {
        m_gameOver = true;
    }
}

