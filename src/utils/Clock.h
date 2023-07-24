#pragma once

#include "String.h"
#include "List.h"
#include "FloatStatistic.h"

class StopClock {
	DEBUGCOUNTER(StopClock);
public:
	enum State {
		Initialized,
		Running,
		Paused
	};
private:
	State _state = Initialized;
	std::chrono::high_resolution_clock::time_point _start_time;
	float _seconds = 0;
	int _frame_count = 0;
public:
	explicit StopClock();
	~StopClock();
public:
	void reset();
	void start();
	void pause();
public:
	bool isRunning() const{ return _state == Running; }
	float seconds() const;
	int frame_count() const;
	float fps() const;
};

class SectionedStopClock {
public:
	DEBUGCOUNTER(SectionedStopClock);
public:
	struct Section {
		SectionedStopClock* parent = null;
		String name;
		StopClock clock;
	public:
		void start() const{
			parent->activate( this );
		}
	};
private:
	StopClock _total;
private:
	List<Section*> _sections;
private:
	Section* _current_section = null;
public:
	explicit SectionedStopClock();
	~SectionedStopClock();
public:
	const Section* createSection( const String& name );
public:
	void reset();
	void start();
	void pause();
public:
	void activate( const Section* section );
public:
	const StopClock& total() const{ return _total; }
public:
	String sections_percents() const;
};

#include "utils/RingBuffer.h"
struct PerformanceCounter {
private:
	static 	List<PerformanceCounter*> _Counters;
public:
	static PerformanceCounter* Create( const String& name );
	static PerformanceCounter* GetOrCreate( const String& name );
	static const List<PerformanceCounter*>& Counters(){ return _Counters; }
	static void NewFrame();
	static void Destroy();
private:
	String _name;
private:
	enum Type { Framed, Ticked };
	Type _type = Framed;
private:
	//std::chrono::high_resolution_clock::time_point _startTime;
	bool _running = false;
	float _duration = 0;
	std::chrono::high_resolution_clock::time_point _prevTime;
private:
	float _prev_duration = 0;
private:
	int _frameCount = 0;
	FloatStatistic _statistic;
private:
	float _avg_fps = 0;
private:
	RingBuffer<float> _history;
public:
	PerformanceCounter( const String& name );
public:
	const String& name() const{ return _name; }
public:
	void setTypeTicked();
public:
	void start();
	void stop();
	void add_s( float seconds );
	float durationSinceLast() const{ return _duration; }
	void next();
public:
	float avg_fps() const{ return _avg_fps; }
	const FloatStatistic& statistic() const{ return _statistic; }
	const RingBuffer<float>& history() const{ return _history; }

};
