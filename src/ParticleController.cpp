#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/Vector.h"
#include "ParticleController.h"
#include "cinder/Path2d.h"

using namespace ci;
using std::list;
using std::vector;

ParticleController::ParticleController()
{
}

void ParticleController::update( const Perlin &perlin, const Channel32f &channel, const Vec2i &mouseLoc )
{
	for( vector< list<Particle> >::iterator g = mParticleGroups.begin(); g != mParticleGroups.end(); ++g )
	{
		for( list<Particle>::iterator p = g->begin(); p != g->end(); ++p) 
		{
				p->update( perlin, channel, mouseLoc );
		}
	}
}

void ParticleController::draw()
{
	gl::color(Colorf::white());

	for( vector< list<Particle> >::iterator g = mParticleGroups.begin(); g != mParticleGroups.end(); ++g ){
		Path2d mPath;
		mPath.clear();
		std::list<Particle>::iterator prev;
		std::list<Particle>::iterator next;
		std::list<Particle>::iterator next2;
		int index = 0;
		for( list<Particle>::iterator p = g->begin(); p != g->end(); ++p ){
			//p==g->begin() ? mPath.moveTo(p->mLoc) : mPath.lineTo(p->mLoc);
			
			Vec2f p0, p1, p2, p3;

			if (index == 0)
			{
				prev = p;
				next = boost::next(p);
				next2 = boost::next(p,2);
			}
			else if(index == g->size()-2)
			{
				prev = boost::prior(p);
				next = boost::next(p);
				next2 = next;
			}
			else if (index == g->size()-1)
			{
				prev = boost::prior(p);
				next = p;
				next2 = p;
			} 
			else
			{
				prev = boost::prior(p);
				next = boost::next(p);
				next2 = boost::next(p,2);
			}

			p0 = prev->mLoc;
			p1 = p->mLoc;
			p2 = next->mLoc;
			p3 = next2->mLoc;
			
			Vec2f pp[4] = {p0, p1, p2, p3};
			Vec2f c1 = calcCubicBezierPos(pp, 0.25f);
			Vec2f c2 = calcCubicBezierPos(pp, 0.75f);
			//app::console() << p0.x << " " << p1.x << " " << p2.x << " " << p3.x << " controls: " << c1.x << " " << c2.x << std::endl;
			//p==g->begin() ? mPath.moveTo(p->mLoc) : mPath.lineTo(p->mLoc);
			p==g->begin() ? mPath.moveTo(p->mLoc) : mPath.quadTo(p1, p2);
			index++;
		}
		gl::draw(mPath);
	}
}

Vec2f ParticleController::calcCubicBezierPos( const Vec2f p[4], float t )
{
	/*
	float nt = 1 - t;
	float w0 = nt * nt * nt;
	float w1 = 3 * nt * nt * t;
	float w2 = 3 * nt * t * t;
	float w3 = t * t * t;
	return Vec2f( w0 * p[0].x + w1 * p[1].x + w2 * p[2].x + w3 * p[3].x, w0 * p[0].y + w1 * p[1].y + w2 * p[2].y + w3 * p[3].y );
	*/
	return p[1] + 0.5 * t * (p[2]-p[0] + t * (2.0 * p[0] - 5.0 * p[1] + 4.0 * p[2]-p[3] + t * (3.0 * (p[1]-p[2]) + p[3] - p[0])));
}

void ParticleController::drawPoints()
{
	gl::color(Colorf::white());
	for( vector< list<Particle> >::iterator g = mParticleGroups.begin(); g != mParticleGroups.end(); ++g )
	{
		for( list<Particle>::iterator p = g->begin(); p != g->end(); ++p) 
		{
			p->draw();
		}
	}
}

void ParticleController::drawNoise()
{
	for( vector< list<Particle> >::iterator g = mParticleGroups.begin(); g != mParticleGroups.end(); ++g )
	{
		for( list<Particle>::iterator p = g->begin(); p != g->end(); ++p) 
		{
			p->drawNoise();
		}
	}
}

void ParticleController::addParticle(int xi, int yi )
{
	if (mParticleGroups.empty())
	{
		std::list<Particle> particles;
		mParticleGroups.push_back(particles);
	}
	Vec2f loc = Vec2f((float)xi, (float)yi);
	Vec2f vel = Vec2f::zero();
    mParticleGroups[0].push_back( Particle( loc, vel ) );
}

void ParticleController::addParticlesAsGroup(std::list<ci::Vec2f> group)
{
	std::list<Particle> particles;

	for( list<Vec2f>::iterator v = group.begin(); v != group.end(); ++v )
	{
		Vec2f loc = *v;
		particles.push_back(Particle(loc, Vec2f::zero()));
	}

	mParticleGroups.push_back(particles);
}

void ParticleController::removeParticles( int amt )
{
	for( int i=0; i<amt; i++ )
	{
		mParticleGroups[0].pop_back();
	}
}

