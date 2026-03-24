/**
 * @file FireworksEffect.cpp
 * @brief Implementation of particle fireworks for the 64x32 LED matrix.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "FireworksEffect.h"

#include <cmath>

#include "Config.h"

void FireworksEffect::trigger(int durationSecs) {
	// Kill all existing particles and schedule bursts
	for (int i = 0; i < kMaxParticles; i++) {
		particles_[i].alive = false;
	}
	burstsPending_ = kBurstsPerTrigger * durationSecs;
	burstTimer_	   = 0.0f;
	active_		   = true;
}

void FireworksEffect::spawnBurst(float cx, float cy) {
	int spawned = 0;
	for (int i = 0; i < kMaxParticles && spawned < 20; i++) {
		if (!particles_[i].alive) {
			float angle		   = random(0, 360) * PI / 180.0f;
			float speed		   = random(15, 55);
			particles_[i].x	   = cx;
			particles_[i].y	   = cy;
			particles_[i].vx   = cosf(angle) * speed;
			particles_[i].vy   = sinf(angle) * speed;
			particles_[i].life = 1.0f;
			// Random bright color
			uint8_t r = random(128, 256);
			uint8_t g = random(128, 256);
			uint8_t b = random(128, 256);
			// Bias towards vibrant hue
			int channel = random(0, 3);
			if (channel == 0)
				r = 255;
			else if (channel == 1)
				g = 255;
			else
				b = 255;
			// Use the panel color565 packing manually
			particles_[i].color =
				((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
			particles_[i].alive = true;
			spawned++;
		}
	}
}

void FireworksEffect::update(float dt, DisplayManager &display) {
	if (!active_) return;

	auto *panel = display.getPanel();

	// Spawn staggered bursts
	if (burstsPending_ > 0) {
		burstTimer_ -= dt;
		if (burstTimer_ <= 0.0f) {
			float cx = random(8, Panel::kResX - 8);
			float cy = random(4, Panel::kResY - 12);
			spawnBurst(cx, cy);
			burstsPending_--;
			burstTimer_ = 0.25f;  // 250ms between bursts
		}
	}

	// Update & render particles
	bool anyAlive = false;
	for (int i = 0; i < kMaxParticles; i++) {
		Particle &p = particles_[i];
		if (!p.alive) continue;

		p.vy += kGravity * dt;
		p.x += p.vx * dt;
		p.y += p.vy * dt;
		p.life -= kFadeRate * dt;

		if (p.life <= 0.0f || p.x < 0 || p.x >= Panel::kResX || p.y < 0 ||
			p.y >= Panel::kResY) {
			p.alive = false;
			continue;
		}

		anyAlive = true;

		// Fade the color by life
		uint8_t brightness = (uint8_t)(p.life * 255);
		// Extract original RGB from stored color565
		uint8_t r = ((p.color >> 11) & 0x1F) << 3;
		uint8_t g = ((p.color >> 5) & 0x3F) << 2;
		uint8_t b = (p.color & 0x1F) << 3;
		r		  = (r * brightness) >> 8;
		g		  = (g * brightness) >> 8;
		b		  = (b * brightness) >> 8;

		panel->drawPixel((int)p.x, (int)p.y, panel->color565(r, g, b));
	}

	if (!anyAlive && burstsPending_ <= 0) {
		active_ = false;
	}
}
