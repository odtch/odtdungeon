#pragma once

#include "AudioCore.h"

class AudioClip : public Resource
{
private:
	uint8_t* _data = null;
	uint32_t _length = 0;
public:
	explicit AudioClip();
	virtual ~AudioClip() override;
public:
	friend class AudioSystem;
};

struct AudioPlayer {
private:
	bool _external = false;
private:
	AudioClip* _clip = null;
	uint32_t _position_in_clip = 0;
private:
	bool _looping = false;
	int _volume = 0;
private:
	bool _has_minmax_distance = false;
	FloatRange _minmax_distance;
	bool _has_position = false;
	Vec3 _position;
private:
	AudioPlayer();
	~AudioPlayer();
public:
	void reset();
	void setLooping( bool looping = true );
	void setVolume( float volume );
	void setMinMaxDistance( float min, float max );
	void setPosition( const Vec3& position );
public:
	void start( AudioClip* clip );
public:
	friend class AudioSystem;
};

class AudioSystem : public Module
{
public:
	static const char* Id;
	static AudioSystem* Get(){ return Singleton::Get<AudioSystem>(); }
private:
	uint32_t _device_id = 0; // type SDL_AudioDeviceID
	SDL_AudioSpec* _device_audiospec = null;
private:
	List<AudioClip*> _clips;
private:
	Camera* _camera = null; // listener
private:
	enum { MaxAudioPlayerCount = 15 };
	AudioPlayer _players[ MaxAudioPlayerCount ];
public:
	explicit AudioSystem();
	virtual ~AudioSystem() override;
public:
	AudioClip* loadWAV( const String& filename );
public:
	void bindCamera( Camera* camera );
	bool hasListenerPosition() const{ return _camera != null; }
	const Vec3& listenerPosition() const{ assert( _camera ); return _camera->position(); }
public:
	void start( AudioClip* clip, float volume );
	void start( AudioClip* clip, const Vec3& position, float max_distance, float volume );
	void start( AudioClip* clip, bool has_position, const Vec3& position, bool has_minmax_distance, float max_distance, float volume );
public:
	AudioPlayer* createPlayer();
	void releasePlayer( AudioPlayer* player );
public:
	void callback( uint8_t* stream, int len );
public:
	virtual void start() override;
	virtual void stop() override;
};
