#include "cinder/Rand.h"
#include "cinder/Vector.h"
#include "ParticleController.h"
#include "cinder/Path2d.h"
#include "cinder/PolyLine.h"
#include "cinder/gl/gl.h"

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

	for( vector< list<Particle> >::iterator g = mParticleGroups.begin(); g != mParticleGroups.end(); ++g )
	{
		vector<Vec2f> mPoints;


		/*
		for( list<Particle>::iterator pf = g->begin(); pf != g->end(); ++pf ){
			mPoints.push_back(pf->mLoc - Vec2f(0.0f,pf->mRadius*0.5f));
		}
		mPoints.push_back(g->rbegin()->mLoc - Vec2f(0.0f,g->rbegin()->mRadius*0.5f)); // double up top right
		mPoints.push_back(g->rbegin()->mLoc + Vec2f(0.0f,g->rbegin()->mRadius*0.5f)); // double up bottom right
		
		for( list<Particle>::reverse_iterator pr = g->rbegin(); pr != g->rend(); ++pr ){
			mPoints.push_back(pr->mLoc + Vec2f(0.0f,pr->mRadius*0.5f));
		}
		mPoints.push_back(g->begin()->mLoc + Vec2f(0.0f,g->begin()->mRadius*0.5f)); // double up bottom left
		*/

		list<Particle>::iterator pf;
		for( pf = g->begin(); pf != g->end(); ++pf )
		{
			mPoints.push_back(pf->mLoc);
		}
		BSpline2f mSpline(mPoints, 3, false, true);
		//Path2d mPath(mSpline);
		//gl::drawSolid(mPath);

		int midpoints = 6;
		int npnts = g->size() * midpoints;
		
		pf = g->begin();
		for(int i = 0; i < npnts-1; i++)
		{
			float factor = (float)(i%(midpoints-1)) / (midpoints-1);
			float w;
			(boost::next(pf) != g->end()) ? w = pf->mRadius + (boost::next(pf)->mRadius - pf->mRadius) * factor : w = pf->mRadius;
			glLineWidth(w);
			gl::drawLine(	mSpline.getPosition(i / (npnts - 1.0f)), 
							mSpline.getPosition((i+1) / (npnts - 1.0f)));
			if (i!= 0 && i%midpoints == 0) ++pf;
		}

		
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

