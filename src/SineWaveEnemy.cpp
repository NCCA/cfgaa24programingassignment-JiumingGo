//
// Created by s5304091
//
#include "SineWaveEnemy.h"
#include <cmath> // Include cmath for the sin function

// Constructor initializing with base class Enemy's parameters and additional sine wave parameters
SineWaveEnemy::SineWaveEnemy(const ngl::Vec3 &startPosition, float amplitude, float frequency)
        : Enemy(startPosition),m_startPosition(startPosition), m_amplitude(amplitude), m_frequency(frequency), m_waveTimer(0.0f)
{
}


// Override the update method to include sine wave movement
void SineWaveEnemy::update(float deltaTime)
{
    // Update the base class (Enemy) movement, usually downward movement
    Enemy::update(deltaTime);

    // Additional sine wave movement along the x-axis
    m_waveTimer += deltaTime; // Increment wave timer by the elapsed time
    // Calculate new x position based on the sine wave
    m_pos.m_x = m_startPosition.m_x + m_amplitude * sin(m_frequency * m_waveTimer);
}