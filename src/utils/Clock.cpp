#include "stdodt.h"
#include "Clock.h"
#include "Logger.h"

StopClock::StopClock(){
	reset();
}
StopClock::~StopClock(){
}
void StopClock::reset(){
	_state = Initialized;
	_seconds = 0;
	_frame_count = 0;
}
void StopClock::start(){
	assert( _state == Initialized || _state == Paused );
	_start_time = std::chrono::high_resolution_clock::now();
	_state = Running;
}
void StopClock::pause(){
	assert( _state == Running );
	_state = Paused;
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - _start_time ).count();
	_seconds += time;
	_frame_count++;
}
float StopClock::seconds() const{
	assert( _state == Paused );
	assert( 0 < _frame_count );
	return _seconds;
}
int StopClock::frame_count() const{
	assert( _state == Paused );
	assert( 0 < _frame_count );
	return _frame_count;
}
float StopClock::fps() const{
	assert( _state == Paused );
	assert( 0 < _frame_count );
	return ( _frame_count / seconds() );
}

SectionedStopClock::SectionedStopClock(){
	reset();
}
SectionedStopClock::~SectionedStopClock(){
	_current_section = null;
	_sections.deleteAll();
}
const SectionedStopClock::Section* SectionedStopClock::createSection(const String& name){
	Section* section = new Section();
	section->parent = this;
	section->name = name;
	_sections.add( section );
	return section;
}
void SectionedStopClock::reset(){
	_total.reset();
	_current_section = null;
	for( auto section : _sections )
		section->clock.reset();
}
void SectionedStopClock::start(){
	_total.start();
}
void SectionedStopClock::pause(){
	if( _current_section ){
		_current_section->clock.pause();
		_current_section = null;
	}
	_total.pause();
}
void SectionedStopClock::activate( const Section* section ){
	assert( section );
	assert( _total.isRunning() );
	if( _current_section ){
		_current_section->clock.pause();
	}
	_current_section = (Section*) section;
	_current_section->clock.start();
}
String SectionedStopClock::sections_percents() const {
	assert( !_total.isRunning() );
	String text;
	for( auto section : _sections ){
		if( !text.isEmpty() )
			text += String( ", " );
		text += String::FromInt( (int)( 100 * section->clock.seconds() / _total.seconds() ) );
		text += String( "% " );
		text += section->name;
	}
	return text;
}


List<PerformanceCounter*> PerformanceCounter::_Counters;
PerformanceCounter* PerformanceCounter::Create(const String& name){
	auto counter = new PerformanceCounter( name );
	_Counters.add( counter );
	return counter;
}
PerformanceCounter* PerformanceCounter::GetOrCreate( const String& name ){
	for( auto counter : _Counters ){
		if( counter->_name == name )
			return counter;
	}
	return Create( name );
}
void PerformanceCounter::NewFrame(){
	for( auto counter : _Counters ){
		counter->next();
	}
}
void PerformanceCounter::Destroy(){
	_Counters.deleteAll();
}

PerformanceCounter::PerformanceCounter( const String& name )
	:_name( name )
	,_history( 500 )
{
	for( int i = 0; i < _history.size(); i++ ){
		_history.next() = 0;
	}
}
void PerformanceCounter::setTypeTicked(){
	_type = Ticked;
}
int pcsc = 0;
void PerformanceCounter::start(){
	ASSERT( _type == Framed );
	if( _running ){
		logError( "PerformanceCounter::start already running" );
	}
	_running = true;
	_prevTime = std::chrono::high_resolution_clock::now();
//	for( int i = 0; i < pcsc; i++ ){
//		std::cout << "  ";
//	}
//	std::cout << _name.asStd() << "\n";
//	pcsc++;
}
void PerformanceCounter::stop(){
//	pcsc--; if( pcsc < 0 )pcsc = 0;
//	for( int i = 0; i < pcsc; i++ ){
//		std::cout << "  ";
//	}
//	std::cout << "~" << _name.asStd() << "\n";
	ASSERT( _type == Framed );
	if( !_running ){
		logError( "PerformanceCounter::stop not running" );
		_running = false;
		return;
	}
	_running = false;
	auto currentTime = std::chrono::high_resolution_clock::now();
	_duration += std::chrono::duration<float, std::chrono::seconds::period>( currentTime - _prevTime ).count();
}
void PerformanceCounter::add_s(float seconds){
	if( _type == Ticked ){
		_statistic.add( seconds );
		_history.next() = seconds;
	} else {
		_duration += seconds;
	}
}
void PerformanceCounter::next(){
	if( _type == Ticked )
		return;
	if( _running ){
		logError( "PerformanceCounter::next running", _name );
	}
	_prev_duration = _duration;
	_duration = 0;
	if( ( _frameCount % 60 == 0  ) && _statistic.sum() > 0 ){
		ASSERT( 0 < _statistic.sum() );
		_avg_fps = _frameCount / _statistic.sum();
		_statistic.reset();
		_frameCount = 0;
	}
	_frameCount++;
	_statistic.add( _prev_duration );
	_history.next() = _prev_duration;
}

FpsCounter::FpsCounter(){
	reset();
}
FpsCounter::~FpsCounter(){
}
void FpsCounter::reset(){
	_start_time = std::chrono::high_resolution_clock::now();
	_frame_count = 0;
}
bool FpsCounter::tick(){
	_frame_count++;
	auto currentTime = std::chrono::high_resolution_clock::now();
	float seconds = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - _start_time ).count();
	_current_fps = _frame_count / seconds;
	if( 160 < _frame_count ){
		_start_time = currentTime;
		_frame_count = 0;
		return true;
	} else {
		return false;
	}
}

FrameRateLimiter::FrameRateLimiter( float maxFramesPerSeconds ){
	prev_time = std::chrono::high_resolution_clock::now();
	min_duration = std::chrono::milliseconds( (int)( 1000 / ( maxFramesPerSeconds ) ) + 1 );
}
FrameRateLimiter::~FrameRateLimiter(){
}
void FrameRateLimiter::tick(){
	static std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	auto delay = now - prev_time;
	auto sleep_time = min_duration - delay;
	if( _debug ){
//		std::time_t t_now = std::chrono::system_clock::to_time_t(now);
//		   std::tm* gmtime = std::gmtime(&t_now);
//		   // Extract the milliseconds
//		   auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
//		   // Format the time
//		   std::cout << std::put_time(gmtime, "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << milliseconds.count() << " UTC" << std::endl;
		std::time_t now_c = std::chrono::system_clock::to_time_t( now );
		//std::cout << "frl.t now=" << std::ctime(&now_c);
		std::cout << "frl " << std::chrono::duration_cast<std::chrono::milliseconds>( now - start ).count() << " ms\t";
		std::cout << "   delay=" << std::chrono::duration_cast<std::chrono::milliseconds>( delay ).count() << " ms";
		std::cout << "   sleep=" << std::chrono::duration_cast<std::chrono::milliseconds>( sleep_time ).count() << " ms" << std::endl;
	}
	if( delay < min_duration ){
		_sleep_time_per_frame.add( std::chrono::duration_cast<std::chrono::milliseconds>( sleep_time ).count() );
		std::this_thread::sleep_for( sleep_time );
		//if( _debug )std::cout << "s";
	} else {
		_sleep_time_per_frame.add( 0 );
		//if( _debug )std::cout << "N";
	}
	//prev_time = now;
	prev_time = std::chrono::high_resolution_clock::now();
}
