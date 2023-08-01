#include "stdodt.h"
#include "AudioSystem.h"
#include "SDL2/SDL.h"

AudioClip::AudioClip(){
}
AudioClip::~AudioClip(){
	ASSERT( _data == null );
	ASSERT( _length == 0 );
}

AudioPlayer::AudioPlayer(){
}
AudioPlayer::~AudioPlayer(){
	ASSERT( _external == false );
}
void AudioPlayer::reset(){
	_clip = null;
	_position_in_clip = 0;
	_looping = false;
	_volume = SDL_MIX_MAXVOLUME;
	_has_minmax_distance = false;
	_has_position = false;
}
void AudioPlayer::setLooping( bool looping ){
	_looping = looping;
}
void AudioPlayer::setVolume( float volume ){
	assert( 0 <= volume && volume <= 1 );
	_volume = (int)( SDL_MIX_MAXVOLUME * volume );
}
void AudioPlayer::setMinMaxDistance( float min, float max ){
	_has_minmax_distance = true;
	_minmax_distance.set( min, max );
}
void AudioPlayer::setPosition( const Vec3& position ){
	_has_position = true;
	_position = position;
}
void AudioPlayer::start( AudioClip* clip ){
	assert( clip ); assert( _clip == null );
	_clip = clip;
	_position_in_clip = 0;
}

AudioSystem* global_audiosystem = null;

void global_audiocallback( void* userdata, Uint8* stream, int len ){
	ASSERT( global_audiosystem );
	global_audiosystem->callback( stream, len );
}

void logAudioSpec( const SDL_AudioSpec& audiospec ){
	logDebug( "ausiospec.freq", audiospec.freq );
	logDebug( "        format", audiospec.format );
	logDebug( "      channels", (int) audiospec.channels );
	logDebug( "       silence", (int) audiospec.silence );
	logDebug( "       samples", audiospec.samples );
	logDebug( "       padding", audiospec.padding );
	logDebug( "          size", audiospec.size );
	logDebug( "      callback", audiospec.callback );
	logDebug( "      userdata", audiospec.userdata );
}

const char* AudioSystem::Id = "AudioSystem";
AudioSystem::AudioSystem()
	:Module( Id )
{
	assert( global_audiosystem == null );
	global_audiosystem = this;
	_device_audiospec = new SDL_AudioSpec();
}
AudioSystem::~AudioSystem(){
	assert( global_audiosystem == this );
	global_audiosystem = null;
	_clips.deleteAll();
	odelete( _device_audiospec );
}
AudioClip* AudioSystem::loadWAV( const String& filename ){
	logDebug( "loadWAV", filename );
	AudioClip* clip = new AudioClip();
	_clips.add( clip );
	//clip->setId( File::GetNameFromPath( filename ) );
	SDL_AudioSpec wav_audiospec;
	Uint8* wav_data = null;
	Uint32 wav_length = 0;
	SDL_AudioSpec* wav_audiospec_result = SDL_LoadWAV( filename.asCStr(), &wav_audiospec, &wav_data, &wav_length );
	if( wav_audiospec_result == NULL ) {
		logError( "SDL_LoadWAV failed", filename, SDL_GetError() );
		return clip;
	}
	ASSERT( wav_audiospec_result == &wav_audiospec );
	SDL_AudioCVT cvt;
	int rc = SDL_BuildAudioCVT( &cvt, wav_audiospec.format, wav_audiospec.channels, wav_audiospec.freq, _device_audiospec->format, _device_audiospec->channels, _device_audiospec->freq );
	if( rc == 1 ){
		logDebug( "SDL_BuildAudioCVT prepared" );
	} else if( rc == 0 ){
		logDebug( "SDL_BuildAudioCVT no conversion needed" );
	} else {
		logError( "SDL_BuildAudioCVT ", rc, SDL_GetError() );
		return clip;
	}
	cvt.buf = (Uint8 *) malloc( wav_length * cvt.len_mult );
	memcpy( cvt.buf, wav_data, wav_length );
	cvt.len = wav_length;
	rc = SDL_ConvertAudio( &cvt );
	if( rc == 0 ){
		logDebug( "SDL_ConvertAudio conversion was completed successfully" );
	} else {
		logError( "SDL_ConvertAudio ", rc, SDL_GetError() );
		return clip;
	}
	SDL_FreeWAV( wav_data );
	clip->_data = cvt.buf;
	clip->_length = cvt.len_cvt;
	return clip;
}
void AudioSystem::bindCamera( Camera* camera ){
	_camera = camera;
}
void AudioSystem::start( AudioClip* clip, float volume ){
	start( clip, false, Vec3::Null, false, 0, volume );
}
void AudioSystem::start( AudioClip* clip, const Vec3& position, float max_distance, float volume ){
	start( clip, true, position, true, max_distance, volume );
}
void AudioSystem::start( AudioClip* clip, bool has_position, const Vec3& position, bool has_minmax_distance, float max_distance, float volume ){
	ASSERT( clip );
	if( volume < 0 ){
		ASSERT( false );
		return;
	}
	if( volume < 0 )
		return;
	if( 1 < volume ){
		ASSERT( false );
		volume = 1;
	}
	if( clip->_data == null )
		return;
	for( int p = 0; p < MaxAudioPlayerCount; p++ ){
		AudioPlayer& player = _players[ p ];
		if( player._external ){
			//std::cout << "e";
		} else if( player._clip != null ){
			//std::cout << "c";
		} else {
			player.reset();
			if( has_position )
				player.setPosition( position );
			if( has_minmax_distance )
				player.setMinMaxDistance( 0, max_distance );
			player.setVolume( volume );
			player._clip = clip;
			//SDL_LockAudioDevice( _device_id );
			//SDL_UnlockAudioDevice( _device_id );
			return;
		}
	}
	logDebug( "AudioSystem.start no free player", clip->id() );
//	for( int p = 0; p < MaxAudioPlayerCount; p++ ){
//		AudioPlayer& player = _players[ p ];
//		std::cout << "  " << p << " ";
//		if( player._external )std::cout << "external";
//		std::cout << "\t" << (int)(( 100.0f * player._position_in_clip ) ) << " ";
//		if( player._looping ) std::cout << "loop  ";
//		if( player._clip )
//			std::cout << player._clip->id().asStd();
//		std::cout << "\n";
//		std::cout.flush();
//	}
}
AudioPlayer* AudioSystem::createPlayer(){
	for( int p = 0; p < MaxAudioPlayerCount; p++ ){
		AudioPlayer& player = _players[ p ];
		if( player._external ){
		} else if( player._clip ){
		} else {
			player._external = true;
			player.reset();
			return &player;
		}
	}
	logError( "AudioSystem no free player" );
	ASSERT( false );
	return null;
}
void AudioSystem::releasePlayer( AudioPlayer* player ){
	assert( player );
	player->_external = false;
}
void AudioSystem::callback( uint8_t* stream, int len ){
	bool logging = false;
	static int log_in = 50;
	log_in--;
	if( log_in == 0 ){
		log_in = 50;
		//logging = true;
	}
	//	logDebug( "mixeAudio", laenge );
	for( int i = 0; i < len; i++ ){
		stream[i] = 0;
	}
	for( int p = 0; p < MaxAudioPlayerCount; p++ ){
		AudioPlayer& player = _players[ p ];
		if( player._clip ){
			int volume = player._volume;
			if( player._has_position ){
				assert( player._has_minmax_distance );
				assert( hasListenerPosition() );
				float dist = ( player._position - listenerPosition() ).length();
				if( dist < player._minmax_distance.min() ){
					//logDebug( "player near" );
				} else if( player._minmax_distance.max() < dist ){
					volume = 0;
					//logDebug( "player to faar" );
				} else {
					float dstfact = player._minmax_distance.valuetofactor( dist );
					float invfact = 1.0f - dstfact;
					volume = (int)( volume * invfact );
					//logDebug( "as", p, dist, dstfact, "\t", invfact, volume, ( volume * invfact ), "\t", player._volume );
				}
			}
			Uint32 player_len = player._clip->_length - player._position_in_clip;
			if( len < player_len ){
				player_len = len;
			}
			if( logging ){
				logDebug( "AudioSystem", p, volume, player._clip->id() );
			}
			if( volume < 1 ){
			} else {
				const uint8_t* src_data = &( player._clip->_data[ player._position_in_clip ] );
				ASSERT( 0 < len );
				SDL_MixAudioFormat( stream, src_data, _device_audiospec->format, player_len, volume );
			}
			{
				// allways
				player._position_in_clip += player_len;
				if( player._position_in_clip == player._clip->_length ){
					if( player._looping ){
						player._position_in_clip = 0;
					} else {
						player._clip = null;;
					}
				} else {
					if( player._position_in_clip < player._clip->_length ){
					} else {
						logDebug( "a", player._position_in_clip, len, player._clip->_length );
					}
					ASSERT( player._position_in_clip < player._clip->_length );
				}
			}
		}
	}
}
void AudioSystem::start(){
	int rc = SDL_Init( SDL_INIT_AUDIO );
	if( rc != 0 ){
		logError( "SDL_Init failed: ", rc, SDL_GetError() );
		return;
	}
	SDL_AudioSpec format;
	/* Format: 16 Bit, stereo, 22 KHz */
	format.freq = 22050;
	format.format = AUDIO_S16;
	format.channels = 2;
	format.samples = 512;        /* ein guter Wert für Spiele */
	// Hmm?
	format.freq = 44100;
	//specs.format = AUDIO_S32;
	//specs.format = AUDIO_S16LSB;
	format.channels = 2;
	//format.samples = 4096;
	format.callback = global_audiocallback;
	format.userdata = NULL;
	logAudioSpec( format );
	ASSERT( _device_id == 0 );
	SDL_AudioSpec obtained;
	_device_id = SDL_OpenAudioDevice( NULL, 0, &format, _device_audiospec, SDL_AUDIO_ALLOW_ANY_CHANGE );
	if( _device_id < 1 ){
		logError( "SDL_OpenAudioDevice failed", _device_id, SDL_GetError() );
		return;
	}
	logAudioSpec( *_device_audiospec );

	SDL_PauseAudioDevice( _device_id, 0);
}
void AudioSystem::stop(){
	SDL_PauseAudio( 1 );
	if( 0 < _device_id ){
		SDL_CloseAudioDevice( _device_id );
		_device_id = 0;
	}
	for( AudioClip* clip : _clips ){
		if( clip->_data ){
			free( clip->_data );
		}
		clip->_data = null;
		clip->_length = 0;
	}
	SDL_Quit();
}


