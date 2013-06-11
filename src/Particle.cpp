#include "Particle.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"

using namespace ci;

Particle::Particle()
{
}

Particle::Particle( Vec2f loc, Vec2f vel )
{
	mLoc = mOrigin	= loc;
	mVel			= vel;

	mDecay			= Rand::randFloat( 0.96f, 0.99f );
	mRadius			= 0.0f;
	mScale			= 2.0f;
	mAge			= 0;
	mLifespan		= 100.0f;
	mAgePer			= 1.0f;
	mIsDead			= false;

	noiseVector = Vec2f(0.5f,0.5f);
}	

void Particle::update( const Perlin &perlin, const Channel32f &channel, const Vec2i &mouseLoc )
{	
	// get Perlin noise value based on my location and
	// elapsed seconds and turn this noise value into an angle.
	float nX = mOrigin.x * 0.005f;
	float nY = mOrigin.y * 0.005f;
	float nZ = app::getElapsedSeconds() * 0.5f;
	float noise = perlin.fBm( nX, nY, nZ );
	//float angle = noise * 15.0f;
	float angle = noise * 3.0f;

	noiseVector = Vec2f( cos( angle ), sin( angle ) );

	//mVel += noiseVector * 0.2f * ( 1.0f - mAgePer );
	//mVel += noiseVector * 0.2f;

	//mLoc += mVel;
	//mVel *= mDecay;
	mLoc.y = mOrigin.y + noiseVector.y * 50;

	//mRadius	= mScale * mAgePer;
	mRadius = mScale * ((noiseVector.y + 1.5f) * 0.5);
	
	mAge++;
	//if( mAge > mLifespan ) mIsDead = true;
	//mAgePer = 1.0f - ( (float)mAge/(float)mLifespan );
}

void Particle::drawNoise()
{
	float c = (noiseVector.y + 1.0f) * 0.5;

	gl::color(Colorf(c,c,c));
	//gl::drawSolidRect(ci::Rectf(mOrigin.x, mOrigin.y, 10.0f, 10.0f));
	gl::drawSolidCircle( mOrigin, 5.0f );
}

void Particle::draw()
{
	gl::drawSolidCircle( mLoc, mRadius * mAgePer );
}



