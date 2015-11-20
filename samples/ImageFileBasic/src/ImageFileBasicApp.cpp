#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/params/Params.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ImageFileBasicApp : public App {
  public:
	void setup() override;
	void keyDown( KeyEvent event ) override;
	void fileDrop( FileDropEvent event ) override;
	void draw() override;
	
	gl::TextureRef		mTexture;
	ci::params::InterfaceGlRef	m_params;
	bool			m_bFullscreen = false;
};

void ImageFileBasicApp::setup()
{
	m_params = params::InterfaceGl::create("Params", ivec2(210, 210));

	auto gfs = [&]() -> bool
	{
		return isFullScreen();
	};

	auto sfs = [&](bool value) -> void
	{
		setFullScreen(value);
	};

	m_params->addParam<bool>("Full Screen", sfs, gfs).key("g");
//	m_params->addParam<bool>("Full Screen", [&](bool b)->void { setFullScreen(b); }, [&]()->bool { return isFullScreen(); }).key("g");

	m_params->addParam("full screen", &m_bFullscreen).updateFn([this] 
	{	setFullScreen(m_bFullscreen);	CI_LOG_I("m_bFullscreen: " << m_bFullscreen); }).key("h");

	try {
		fs::path path = getOpenFilePath( "", ImageIo::getLoadExtensions() );
		if( ! path.empty() ) {
			mTexture = gl::Texture::create( loadImage( path ) );
		}
	}
	catch( Exception &exc ) {
		CI_LOG_EXCEPTION( "failed to load image.", exc );
	}
}

void ImageFileBasicApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'o' ) {
		fs::path path = getOpenFilePath( "", ImageIo::getLoadExtensions() );
		if( ! path.empty() )
			mTexture = gl::Texture::create( loadImage( path ) );
	}
	else if( event.getChar() == 's' ) {
		fs::path path = getSaveFilePath();
		if( ! path.empty() ) {
			Surface s8( mTexture->createSource() );
			writeImage( writeFile( path ), s8 );
		}
	}
	else if (event.getChar() == 'f')
	{
		setFullScreen(!isFullScreen());
	}
}

void ImageFileBasicApp::fileDrop( FileDropEvent event )
{
	try {
		mTexture = gl::Texture::create( loadImage( loadFile( event.getFile( 0 ) ) ) );
	}
	catch( Exception &exc ) {
		CI_LOG_EXCEPTION( "failed to load image: " << event.getFile( 0 ), exc );
	}
}

void ImageFileBasicApp::draw()
{
	gl::clear( Color( 0.5f, 0.5f, 0.5f ) );
	gl::enableAlphaBlending();
	
	if( mTexture ) {
		Rectf destRect = Rectf( mTexture->getBounds() ).getCenteredFit( getWindowBounds(), true ).scaledCentered( 0.85f );
		gl::draw(mTexture, getWindowBounds());
	}
	m_params->draw();
}

CINDER_APP( ImageFileBasicApp, RendererGl )
