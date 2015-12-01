#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTimeGl.h"
#include "cinder/params/Params.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class QuickTimeSampleApp : public App {
  public:
	void setup() override;

	void keyDown( KeyEvent event ) override;
	void fileDrop( FileDropEvent event ) override;

	void update() override;
	void draw() override;

	void loadMovieFile( const fs::path &path );

//	ci::params::InterfaceGlRef	m_params;

//	gl::TextureRef			mFrameTexture;
	MovieRef				mMovie;
//	qtime::MovieGlRef		mMovie;

};

void QuickTimeSampleApp::setup()
{
//	m_params = params::InterfaceGl::create("Params", ivec2(210, 210));
//	m_params->addParam<bool>("Full Screen", [&](bool b)->void { setFullScreen(b); }, [&]()->bool { return isFullScreen(); }).key("f");

	fs::path moviePath = getOpenFilePath();
	if( ! moviePath.empty() )
		loadMovieFile( moviePath );
}

void QuickTimeSampleApp::keyDown( KeyEvent event )
{
	switch (event.getChar())
	{
		case 'o':
		{
			fs::path moviePath = getOpenFilePath();
			if (!moviePath.empty())
				loadMovieFile(moviePath);
		}
		break;
		case 'f':
			setFullScreen(!isFullScreen());
		break;
		default:
		break;
	}
}

void QuickTimeSampleApp::loadMovieFile( const fs::path &moviePath )
{
	try {
		// load up the movie, set it to loop, and begin playing
		mMovie = qtime::MovieGl::create( moviePath );
		mMovie->setLoop();
		mMovie->play();
	}
	catch( ci::Exception &exc ) {
		console() << "Exception caught trying to load the movie from path: " << moviePath << ", what: " << exc.what() << std::endl;
		mMovie.reset();
	}

//	mFrameTexture.reset();
}

void QuickTimeSampleApp::fileDrop( FileDropEvent event )
{
	loadMovieFile( event.getFile( 0 ) );
}

void QuickTimeSampleApp::update()
{
//	if( mMovie )
//		mFrameTexture = mMovie->getTexture();
}

void QuickTimeSampleApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );

	mMovie->drawCentreFit(getWindowBounds());

//	if( mFrameTexture ) {
//		Rectf centeredRect = Rectf( mFrameTexture->getBounds() ).getCenteredFit( getWindowBounds(), true );
//		gl::draw( mFrameTexture, centeredRect );
//	}
}

CINDER_APP( QuickTimeSampleApp, RendererGl );