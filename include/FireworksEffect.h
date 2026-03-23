/**
 * @file FireworksEffect.h
 * @brief Particle-based fireworks overlay effect for the LED matrix.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

#include <Arduino.h>
#include "DisplayManager.h"

class FireworksEffect {
public:
    /** @brief Trigger a new set of firework bursts. */
    void trigger(int durationSecs = 1);

    /** @brief Returns true if the effect is currently active (particles alive). */
    bool isActive() const { return active_; }

    /**
     * @brief Update particle physics and render onto the panel.
     * @param dt Delta time in seconds.
     * @param display Reference to the display manager.
     */
    void update(float dt, DisplayManager &display);

private:
    static constexpr int kMaxParticles = 80;
    static constexpr int kBurstsPerTrigger = 4;
    static constexpr float kGravity = 28.0f;
    static constexpr float kFadeRate = 1.8f;

    struct Particle {
        float x, y;
        float vx, vy;
        float life;     // 1.0 → 0.0
        uint16_t color;
        bool alive;
    };

    Particle particles_[kMaxParticles];
    bool active_ = false;
    int burstsPending_ = 0;
    float burstTimer_ = 0.0f;

    void spawnBurst(float cx, float cy);
};
