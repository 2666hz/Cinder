#pragma once
#include "cinder/Rect.h"
#include "cinder/Vector.h"

// A common base-clase for Movie classes (qtime::MovieGL, CinderWMFVideo, etc..)
namespace cinder {

class MovieInterface
{
public:
	virtual		~MovieInterface() {};

	// Common to both QuicktimeImplLegacy and QuickTimeImlAvf	///////////////////////////////////////

	//! Returns the native width of the movie in pixels
	virtual	int			getWidth() const = 0;
	//! Returns the native height of the movie in pixels
	virtual	int			getHeight() const = 0;
	//! Returns the native resolution of the movie in pixels
	virtual ivec2		getSize() const { return ivec2(getWidth(), getHeight()); };
	//! Returns the movie's aspect ratio, the ratio of its width to its height
	virtual float		getAspectRatio() const { return static_cast<float>(getWidth()) / static_cast<float>(getHeight()); };
	//! the Area defining the Movie's bounds in pixels: [0,0]-[width,height]
	virtual Area		getBounds() const { return Area(0, 0, getWidth(), getHeight()); };
	//! Returns the movie's pixel aspect ratio. Returns 1.0 if the movie does not contain an explicit pixel aspect ratio.
	virtual float		getPixelAspectRatio() const { return 1.0f; };

	//! Returns the movie's length measured in seconds
	virtual	float		getDuration() const = 0;
	//! Returns the movie's framerate measured as frames per second
	virtual	float		getFramerate() const = 0;
	//! Returns the total number of frames (video samples) in the movie
	virtual	int			getNumFrames() const = 0;

	//! Returns whether a movie contains at least one visual track, defined as Video, MPEG, Sprite, QuickDraw3D, Text, or TimeCode tracks
	virtual	bool		hasVisuals() const = 0;
	//! Returns whether a movie contains at least one audio track, defined as Sound, Music, or MPEG tracks
	virtual	bool		hasAudio() const = 0;
	//! Returns whether the first video track in the movie contains an alpha channel. Returns false in the absence of visual media.
	virtual	bool		hasAlpha() const = 0;

	//! Returns the current time of a movie in seconds
	virtual	float		getCurrentTime() const = 0;
	//! Sets the movie to the time \a seconds
	virtual	void		seekToTime(float seconds) = 0;
	//! Sets the movie time to the start time of frame \a frame
	virtual	void		seekToFrame(int frame) = 0;
	//! Sets the movie time to its beginning
	virtual	void		seekToStart() = 0;
	//! Sets the movie time to its end
	virtual	void		seekToEnd() = 0;
	//! Limits the active portion of a movie to a subset beginning at \a startTime seconds and lasting for \a duration seconds. QuickTime will not process the movie outside the active segment.
	virtual	void		setActiveSegment(float startTime, float duration) = 0;
	//! Resets the active segment to be the entire movie
	virtual	void		resetActiveSegment() = 0;
	//! Sets whether the movie is set to loop during playback. If \a palindrome is true, the movie will "ping-pong" back and forth
	virtual	void		setLoop(bool loop = true, bool palindrome = false) = 0;
	//! Advances the movie by one frame (a single video sample). Ignores looping settings.
	virtual	void		stepForward() = 0;
	//! Steps backward by one frame (a single video sample). Ignores looping settings.
	virtual	void		stepBackward() = 0;

	//! Sets the playback rate, which begins playback immediately for nonzero values.
	//! 1.0 represents normal speed. Negative values indicate reverse playback and \c 0 stops.
	//! Returns a boolean value indicating whether the rate value can be played (some media types cannot be played backwards)
	virtual	bool		setRate(float rate) = 0;
	//	virtual void		setRate(float rate);	// Legacy version does not return bool

	//! Gets the playback rate, 1.0 represents normal speed. Negative values indicate reverse playback.
	virtual float		getRate() const = 0;

	//! Sets the audio playback volume ranging from [0 - 1.0]
	virtual	void		setVolume(float volume) = 0;
	//! Gets the audio playback volume ranging from [0 - 1.0]
	virtual	float		getVolume() const = 0;

	//! Returns whether the movie is currently playing or is paused/stopped.
	virtual	bool		isPlaying() const = 0;
	//! Returns whether the movie is paused or not
	virtual	bool		isPaused() const = 0;
	//! Returns whether the movie has been stopped or not
	virtual	bool		isStopped() const = 0;
	//! Returns whether the movie has completed playing or not
	virtual	bool		isDone() const = 0;

	//! Begins movie playback. if bToggle = true and movie is already playing, stops movie playback (silly I know)
	virtual	void		play(bool bToggle = false) = 0;	// Avf version
	virtual	void		stop() = 0;
	virtual	void		pause() = 0;

	//! Returns whether the movie has loaded and buffered enough to playback without interruption
//	virtual	bool checkPlayable();		// Legacy version.
	virtual	bool checkPlaythroughOk() = 0;	// Avf version

	//! Returns whether a movie has a new frame available
	virtual	bool checkNewFrame() = 0;

	/// From QuickTimeImplLegacy.h ///////////////////////////////////////////////

	//! Sets a function which is called whenever the movie has rendered a new frame during playback. Generally only necessary for advanced users.
//	void setNewFrameCallback(void(*aNewFrameCallback)(long, void *), void *aNewFrameCallbackRefcon);

//	::Movie	getMovieHandle() const;			// Legacy version. Returns the native QuickTime Movie data structure

	/// From QuickTimeImplAvf.h ///////////////////////////////////////////////////////////////////////////////

	//! Returns whether the movie is in a loaded state, implying its structures are ready for reading but it may not be ready for playback
//	virtual	bool		isLoaded() const = 0;
	//! Returns whether the movie is playable, implying the movie is fully formed and can be played but media data is still downloading
//	virtual	bool		isPlayable() const = 0;
	//! Returns true if the content represented by the movie is protected by DRM
//	virtual	bool		isProtected() const = 0;

//	AVPlayer*	getPlayerHandle() const;	// AVF version. Returns the native AvFoundation Player data structure

	/// New features  ///////////////////////////////////////////////////////////////////////////////

	virtual void draw(const Area &srcArea, const Rectf &dstRect) = 0;
	virtual void draw(const Rectf& destRect) = 0;
	virtual void drawCentreFit(const Rectf& windowBounds, bool bExpand = true) = 0;

	//! Returns the gl::Texture representing the Movie's current frame, bound to the \c GL_TEXTURE_RECTANGLE_ARB target
//	const gl::TextureRef	getTexture();
	
	friend struct ScopedTextureLock;
	struct ScopedTextureLock
	{
	public:
		ScopedTextureLock(const MovieInterface& m);
		virtual ~ScopedTextureLock();
	};

	friend struct ScopedTextureBind;
	struct ScopedTextureBind : private ci::Noncopyable
	{
	public:
		ScopedTextureBind(const MovieInterface& m, uint8_t textureUnit);
		virtual ~ScopedTextureBind();

	private:
//		ci::gl::Context *mCtx;
//		GLenum			mTarget;
//		uint8_t			mTextureUnit;
//		CPlayer			*mPlayer;
	};
};

typedef std::shared_ptr<class MovieInterface>	MovieRef;

} // namespace cinder