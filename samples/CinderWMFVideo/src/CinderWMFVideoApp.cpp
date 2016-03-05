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
	void keyDown(KeyEvent event) override;
	void update() override;
	void draw() override;
	void cleanup() override;
	static void prepareSettings(Settings *settings);

private:
	ci::params::InterfaceGlRef	m_params;

	ciWMFVideoPlayer			m_video1;
	float						m_videoPos = 0.0f;
	float						m_videoDuration = 0.0f;
	bool						m_bIsVideoLoaded = false;

	float						m_fps = 0.0f;
};

void CinderWMFVideoApp::setup()
{
	auto getIsFullScreen = [&]() -> bool { return isFullScreen(); };
	auto setIsFullScreen = [&](bool value) -> void { setFullScreen(!isFullScreen()); };

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

	m_video1.loadMovie("D:\\Dev\\Cinder\\blocks\\Cinder-WMFVideo\\samples\\SimplePlayback\\assets\\1.wmv" , "Speakers (High Definition Audio Device)");
//	m_video1.loadMovie("T:\\Video\\mp4\\1920x960@10mbps\\V0005.mp4", "Speakers (High Definition Audio Device)");
//	m_video1.loadMovie("http://www.ladispersiondufils.net/temp/V0005.mp4", "Speakers (High Definition Audio Device)");
//	m_video1.loadMovie("http://qthttp.apple.com.edgesuite.net/1010qwoeiuryfg/sl.m3u8", "Speakers (High Definition Audio Device)");
//	m_video1.loadMovie("http://212.170.22.153:8080/mjpg/video.mjpg", "Speakers (High Definition Audio Device)");

	m_video1.play();

	m_params = params::InterfaceGl::create("Params", ivec2(210, 210));
	m_params->addParam<float>("FPS", &m_fps, true);
	m_params->addParam<bool>("Full Screen", setIsFullScreen, getIsFullScreen).key("F"); // Buggy - doesnt fill window
	m_params->addParam<float>("Video duration", &m_videoDuration, true);
	m_params->addParam<float>("Video position", &m_videoPos, true);
	m_params->addButton("Load video", loadVideo, "key=V");

	getWindow()->getSignalClose().connect([this] { cleanup(); });

	gl::enableVerticalSync(true);
}

void CinderWMFVideoApp::cleanup()
{
	m_video1.close();
}

void CinderWMFVideoApp::mouseDown(MouseEvent event)
{
	quit();
}

void CinderWMFVideoApp::keyDown(KeyEvent event)
{
	const char c = event.getChar();

	switch (c)
	{
	case 'f':
		setFullScreen(!isFullScreen());
		break;
	case 'c':
		m_video1.close();
		break;
	case 'q':
		quit();
		break;
	default:
		break;
	}
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
