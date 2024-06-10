#ifndef NGLSCENE_H_
#define NGLSCENE_H_
#include "SpaceShip.h"
#include "Enemy.h"
#include "SineWaveEnemy.h"
#include "ShootingEnemy.h"
#include <ngl/Text.h>
#include "WindowParams.h"
#include <QOpenGLWindow>
#include <QElapsedTimer>
#include <QSet>
#include <memory>
#include <QImage>
#include <QOpenGLTexture>
//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/9/13
/// Revision History :
/// This is an initial version used for the new NGL6 / Qt 5 demos
/// @class NGLScene
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------

class NGLScene : public QOpenGLWindow
{
  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for our NGL drawing class
    /// @param [in] parent the parent window to the class
    //----------------------------------------------------------------------------------------------------------------------
    NGLScene();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor must close down ngl and release OpenGL resources
    //----------------------------------------------------------------------------------------------------------------------
    ~NGLScene();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the initialize class is called once when the window is created and we have a valid GL context
    /// use this to setup any default GL stuff
    //----------------------------------------------------------------------------------------------------------------------
    void initializeGL();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we want to draw the scene
    //----------------------------------------------------------------------------------------------------------------------
    void paintGL();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we resize
    //----------------------------------------------------------------------------------------------------------------------
    void resizeGL(int _w, int _h);

    //Method to get elapsed time since the timer was started
    float getElapsedTime();
    //Method to spawn enemy
    void spawnEnemy();
    //Method to spawn sine wave enemy
    void spawnSineEnemy();
    //Method to spawn shooting enemy
    void spawnShootingEnemy();
    //To display player score
    int m_score = 0;
    void checkGameOver();

private:
    //----------------------------------------------------------------------------------------------------------------------
    /// the new add variables for game control
    //----------------------------------------------------------------------------------------------------------------------
    bool m_spacePressed = false;
    QElapsedTimer m_timer;
    bool m_gameOver = false;


    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the windows params such as mouse and rotations etc
    //----------------------------------------------------------------------------------------------------------------------
    WinParams m_win;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our Camera
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Mat4 m_view;
    ngl::Mat4 m_project;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the model position for mouse movement
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_modelPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a timer triggered by the startTimer call in the ctor this is called
    /// to update the ship position based on the key presses
    //----------------------------------------------------------------------------------------------------------------------
    int m_updateShipTimer;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief we will draw with a different timer to the update
    //----------------------------------------------------------------------------------------------------------------------
    int m_redrawTimer;
    //timer for spawn enemies
    int m_enemySpawnTimer;
    //timer for spawn sine enemies
    int m_sineSpawnTimer;
    //timer for spawn shooting enemies
    int m_shooterSpawnTimer;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a pointer to our spaceship
    //----------------------------------------------------------------------------------------------------------------------
    std::unique_ptr<SpaceShip> m_ship;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the keys being pressed
    //----------------------------------------------------------------------------------------------------------------------
    QSet<Qt::Key> m_keysPressed;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a pointer to our enemies
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<Enemy> m_enemies;
    std::vector<SineWaveEnemy> m_sineWaveEnemies;
    std::vector<ShootingEnemy> m_shootEnemies;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to load transform matrices to the shader
    //----------------------------------------------------------------------------------------------------------------------
    void loadMatricesToShader();
     //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse is moved
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent (QMouseEvent * _event );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is pressed
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent ( QMouseEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is released
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent ( QMouseEvent *_event );

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent( QWheelEvent *_event);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief processKeyDown passed from the main window class
    /// @param *_event a pointer to the QKeyEvent passed from main window
    /// class
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief processKeyUp passed from the main window class
    /// @param *_event a pointer to the QKeyEvent passed from main window
    /// class
    //----------------------------------------------------------------------------------------------------------------------
    void keyReleaseEvent(QKeyEvent *_event );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief called when the timer triggers used to update and draw
    /// @param *_event a pointer to the timer event id
    //----------------------------------------------------------------------------------------------------------------------
    void timerEvent( QTimerEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief update ship based on stored key movements
    //----------------------------------------------------------------------------------------------------------------------

    void moveShip();

};



#endif
