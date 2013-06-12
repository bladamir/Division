#pragma once
#include "cinder/Channel.h"
#include "Particle.h"
#include "cinder/Perlin.h"
#include <list>
#include <vector>

class ParticleController {
 public:
	ParticleController();
	void update( const ci::Perlin &perlin, const ci::Channel32f &channel, const ci::Vec2i &mouseLoc );
	void drawNoise();
	void drawPoints();
	void draw();
	void addParticle(int xi, int yi );
	void addParticlesAsGroup(std::list<ci::Vec2f>);
	void addParticles( int amt, const ci::Vec2i &mouseLoc, const ci::Vec2f &mouseVel );
	void removeParticles( int amt );

	ci::Vec2f calcCubicBezierPos( const ci::Vec2f p[4], float t );
	
	std::vector< std::list<Particle> >	mParticleGroups;
};