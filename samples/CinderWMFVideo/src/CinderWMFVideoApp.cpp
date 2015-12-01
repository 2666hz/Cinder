#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"

#include "ciWMFVideoPlayer.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CinderWMFVideoApp : public App
{
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;
	static void prepareSettings(Settings *settings);

private:
	ci::params::InterfaceGlRef	m_params;

	ciWMFVideoPlayer			m_video1;
	float						m_videoPos = 0.0f;
	float						m_videoDuration = 0.0f;
	bool						m_bIsVideoLoaded = false;

	float						m_fps = 0.0f;
	bool						mIsFullScreen = false;
};

void CinderWMFVideoApp::setup()
{
	auto getIsFullScreen = [&]() -> bool
	{
		return mIsFullScreen;
	};

	auto setIsFullScreen = [&](bool value) -> void
	{
		mIsFullScreen = value;
		setFullScreen(mIsFullScreen);
	};

	auto loadVideo = [&]() -> void
	{
		fs::path videoPath = getOpenFilePath(fs::path(), { "mp4", "wmv", "avi", "mov" });
		if (!videoPath.empty()) 
		{
			if (!m_video1.isStopped()) 
			{
				m_video1.stop();
			}

			m_bIsVideoLoaded = m_video1.loadMovie(videoPath);
			m_videoDuration = m_video1.getDuration();
			m_videoPos = m_video1.getPosition();
			m_video1.play();
		}
	};

//	m_video1.loadMovie("D:\\Dev\\Cinder\\blocks\\Cinder-WMFVideo\\samples\\SimplePlayback\\assets\\1.wmv" , "Speakers (High Definition Audio Device)");
	m_video1.loadMovie("F:\\Projects\\Tour Reservoir\\Video\\V0005.mp4", "Speakers (High Definition Audio Device)");
//	m_video1.loadMovie("http://www.ladispersiondufils.net/temp/V0005.mp4", "Speakers (High Definition Audio Device)");
//	m_video1.loadMovie("http://qthttp.apple.com.edgesuite.net/1010qwoeiuryfg/sl.m3u8", "Speakers (High Definition Audio Device)");
//	m_video1.loadMovie("http://212.170.22.153:8080/mjpg/video.mjpg", "Speakers (High Definition Audio Device)");

	m_video1.play();

	m_params = params::InterfaceGl::create("Params", ivec2(210, 210));
	m_params->addParam<float>("FPS", &m_fps, true);
	m_params->addParam<bool>("Full Screen", setIsFullScreen, getIsFullScreen).key("F");
	m_params->addParam<float>("Video duration", &m_videoDuration, true);
	m_params->addParam<float>("Video position", &m_videoPos, true);
	m_params->addButton("Load video", loadVideo, "key=V");
	
	gl::enableVerticalSync(true);

}

void CinderWMFVideoApp::mouseDown(MouseEvent event)
{

}

void CinderWMFVideoApp::update()
{
	m_fps = getAverageFps();
	m_video1.update();
	m_videoPos = m_video1.getPosition();
}

void CinderWMFVideoApp::draw()
{
	gl::clear(Color(0, 0, 0));
	m_video1.draw(0, 0);
//	m_video1.draw(0, 0, m_video1.getWidth(), m_video1.getHeight());
	m_params->draw();
}

void CinderWMFVideoApp::prepareSettings(Settings *settings)
{
	// Create a 2:1 window.
	settings->setWindowSize(960, 480);
	settings->setTitle("CinderWMFVideo");

	// Allow high frame rates to test performance.
	// settings->disableFrameRate();
	settings->setFrameRate(25.0f);
	//  settings->setBorderless(true);
	//	settings->setFullScreen(true);
}

CINDER_APP(CinderWMFVideoApp, RendererGl, CinderWMFVideoApp::prepareSettings)
