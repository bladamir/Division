#include "cinder/app/AppBasic.h"
#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"
#include "cinder/Perlin.h"
#include "cinder/Channel.h"
#include "cinder/Vector.h"
#include "cinder/Utilities.h"
#include "ParticleController.h"
#include "cinder/params/Params.h"

#define RESOLUTION 10
#define NUM_PARTICLES_TO_SPAWN 25

using namespace ci;
using namespace ci::app;

class DivApp : public AppBasic {
 public:
	void prepareSettings( Settings *settings );
	void keyDown( KeyEvent event );
	void mouseDown( MouseEvent event );
	void mouseUp( MouseEvent event );
	void mouseMove( MouseEvent event );
	void mouseDrag( MouseEvent event );
	void setup();
	void update();
	void draw();
	
	Perlin mPerlin;

	Channel32f mChannel;
	gl::Texture	mTexture;
	
	Vec2i mMouseLoc;
	Vec2f mMouseVel;
	bool mIsPressed;
	
	ParticleController mParticleController;
	
	bool mDrawParticles;
	bool mDrawImage;
	bool mSaveFrames;
	bool mShowFPS;
	bool mDrawNoise;

	params::InterfaceGl mParams;

	Font mFont;
};

void DivApp::prepareSettings( Settings *settings )
{
	//settings->setWindowSize( 800, 600 );
	settings->setFullScreen();
	settings->setFrameRate( 60.0f );
}

void DivApp::setup()
{	
	mPerlin = Perlin();
	
	Url url( "http://libcinder.org/media/tutorial/paris.jpg" );
	mChannel = Channel32f( loadImage( loadUrl( url ) ) );
	mTexture = mChannel;

	mMouseLoc = Vec2i( 0, 0 );
	mMouseVel = Vec2f::zero();
	mDrawParticles = true;
	mDrawImage = false;
	mIsPressed = false;
	mSaveFrames = false;
	mShowFPS = false;
	mDrawNoise = false;

	mParams = params::InterfaceGl( "Joy Division", Vec2i( 225, 200 ) );
	mParams.setOptions( "", "iconified=true" );
	mParams.addParam( "Show FPS", &mShowFPS);
	mParams.addParam( "Show Noise Channel", &mDrawNoise );

	int yn = 20;
	int xn = 200;
	for( int y=0; y<yn; y++ ){
		for( int x=0; x<xn; x++ ){
			float xf = (float)x/xn * getWindowWidth();
			float yf = (float)y/yn *  (getWindowHeight() * 0.75f) + (getWindowHeight() * 0.175f);
			mParticleController.addParticle( xf, yf);
		}
	}
}


void DivApp::mouseDown( MouseEvent event )
{
	mIsPressed = true;
}

void DivApp::mouseUp( MouseEvent event )
{
	mIsPressed = false;
}

void DivApp::mouseMove( MouseEvent event )
{
	mMouseVel = ( event.getPos() - mMouseLoc );
	mMouseLoc = event.getPos();
}

void DivApp::mouseDrag( MouseEvent event )
{
	mouseMove( event );
}

void DivApp::keyDown( KeyEvent event )
{
	if( event.getChar() == '1' ){
		mDrawImage = ! mDrawImage;
	} else if( event.getChar() == '2' ){
		mDrawParticles = ! mDrawParticles;
	}
	
	if( event.getChar() == 's' ){
		mSaveFrames = ! mSaveFrames;
	}
}

void DivApp::update()
{
	if( ! mChannel ) return;
	
	/*
	if( mIsPressed )
		mParticleController.addParticles( NUM_PARTICLES_TO_SPAWN, mMouseLoc, mMouseVel );
	*/
	
	mParticleController.update( mPerlin, mChannel, mMouseLoc );
}

void DivApp::draw()
{	
	gl::clear( Color( 0, 0, 0 ), true );

	if (mDrawNoise)
	{
		mDrawParticles = false;
		mParticleController.drawNoise();
	} else 
	{
		mDrawParticles = true;
	}
	
	if( mDrawImage ){
		mTexture.enableAndBind();
		gl::draw( mTexture, getWindowBounds() );
	}
	
	if( mDrawParticles ){
		glDisable( GL_TEXTURE_2D );
		mParticleController.draw();
	}
	
	if( mSaveFrames ){
		writeImage( getHomeDirectory() / ("image_" + toString( getElapsedFrames() ) + ".png"), copyWindowSurface() );
	}

	if( mShowFPS ){
		gl::drawString( "Framerate: " + toString(getAverageFps()), Vec2f( 10.0f, 10.0f ), Color::white() );
	}

	params::InterfaceGl::draw();
}

CINDER_APP_BASIC( DivApp, RendererGl )
