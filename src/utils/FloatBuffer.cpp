#include "stdodt.h"
#include "FloatBuffer.h"
#include "Logger.h"
#include "math/Vec3.h"

FloatBuffer::FloatBuffer( int size )
	:_datas( null )
	,_size( 0 )
	,_last( null )
{
	allocate( size );
	clear();
}
FloatBuffer::FloatBuffer(const FloatBuffer& tocopy)
	:FloatBuffer( tocopy.size() )
{
	copyFrom( tocopy );
}
//FloatBuffer::FloatBuffer( DeserializationStream& stream )
//	:_datas( null )
//	,_size( 0 )
//	,_last( null )
//{
//	allocate( stream.loadArraySize() );
//	float* data = _datas;
//	for( int i = 0; i < _size; i++ ){
//		(*data) = stream.loadQReal();
//		data++;
//	}
//}
//FloatBuffer::FloatBuffer(const QList<float>& values)
//	:FloatBuffer( values.size() )
//{
//	for( int i = 0; i < values.size(); i++ ){
//		set( i, values.at( i ) );
//	}
//}
FloatBuffer::~FloatBuffer(){
	delete _datas;
	_datas = null;
}
void FloatBuffer::allocate( int size ){
	ASSERT( size > 0 );
	_size = size;
	ASSERT( _datas == null );
	_datas = new float[ _size ];
	if( _datas == null ){
		assert( false );
	}
	_last = &_datas[ _size-1 ];
}
void FloatBuffer::operator=(const FloatBuffer& other){
	copyFrom( other );
}

void FloatBuffer::clear()
{
	setAll( 0 );
}
void FloatBuffer::copyFrom( const float* others ){
	ASSERT( others );
	float* w = _datas;
	for( int i = 0; i < _size; i++ ){
		*w = *others;
		w++;
		others++;
	}
}
void FloatBuffer::copyFrom(const FloatBuffer& other){
	ASSERT( _size == other._size );
	float* w = _datas;
	float* r = other._datas;
	for( int i = 0; i < _size; i++ ){
		*w = *r;
		w++;
		r++;
	}
}
//void FloatBuffer::copyFrom( const QList<float> &values ){
//	ASSERT( _size == values.size() );
//	float* w = _datas;
//	for( int i = 0; i < _size; i++ ){
//		*w = values.at( i );
//		w++;
//	}
//}
void FloatBuffer::copyFrom( int at, const FloatBuffer &other ){
	for( int o = 0; o < other.size(); o++ ){
		set( at + o, other.get( o ) );
	}
}
void FloatBuffer::setAll(float value){
	float* w = _datas;
	for( int i = 0; i < _size; i++ ){
		*w = value;
		w++;
	}
}
void FloatBuffer::set(int index, float value){
	ASSERT( 0 <= index && index < _size );
	_datas[ index ] = value;
}

//void FloatBuffer::add(float value)
//{
//	for( int i = 0; i +1 < _size; i++ ){
//		_datas[ i ] = _datas[ i  + 1 ];
//	}
//	_datas[ _size -1 ] = value;
//}
//void FloatBuffer::setFromDoubles( const double* source ){
//	ASSERT( source );
//	float* trg = first();
//	int i = size();
//	while( 0 < i ){
//		(*trg) = (float)( *source );
//		source++;
//		trg++;
//		i--;
//	}
//}
//void FloatBuffer::copyToDoubles( double* target ) const {
//	ASSERT( target );
//	const float* src = first();
//	int i = size();
//	while( 0 < i ){
//		(*target) = (double) (*src);
//		src++;
//		target++;
//		i--;
//	}
//}
////void FloatBuffer::exportCsv(QTextStream& stream) const {
////	for( int i = 0; i < _size; i++ ){
////		if( i != 0 )stream << ",";
////		stream << Number::FromReal( _datas[ i ] ).toString( 10 );
////	}
////}
////QString FloatBuffer::toString() const{
////	QString text;
////	text += "[";
////	for( int i = 0; i < _size; i++ ){
////		if( i != 0 )text += ",";
////		text += Number::FromReal( _datas[ i ] ).toString( 10 );
////	}
////	text += "]";
////	return text;
////}

//float FloatBuffer::DiffSquared(const FloatBuffer& a, const FloatBuffer& b)
//{
//	if( a.size() != b.size() ){
//		assert( false);
//	}
//	float total = 0;
//	for( int i = 0; i < a.size(); i++ ){
//		float error = a.get( i ) - b.get( i );
//		error = error * error;
//		total += error;
//	}
//	return total;
//}
//float FloatBuffer::equals(const FloatBuffer& other, float maxdiff) const {
//	if( size() != other.size() ){
//		assert( false);
//	}
//	float total = 0;
//	for( int i = 0; i < size(); i++ ){
//		float error = std::abs( get( i ) - other.get( i ) );
//		if( maxdiff <= error ) return 1.0f;
//		total += error * error;
//	}
//	return total;
//}
//bool FloatBuffer::equals2(const FloatBuffer &other, float maxdiff) const {
//	for( int i = 0; i < size(); i++ ){
//		float error = std::abs( get( i ) - other.get( i ) );
//		if( error > maxdiff ) return false;
//	}
//	return true;
//}
//float FloatBuffer::equalsSquared(const FloatBuffer& other, float maxdiff) const{
//	if( size() != other.size() ){
//		assert( false);
//	}
//	float total = 0;
//	for( int i = 0; i < size(); i++ ){
//		float error = std::abs( get( i ) - other.get( i ) );
//		if( maxdiff <= error ) return 1.0f;
//		error *= error;
//		total += error;
//	}
//	return total;
//}
//float FloatBuffer::min() const {
//	float v = get( 0 );
//	for( int i = 1; i < size(); i++ ){
//		v = std::min( v, get( i ) );
//	}
//	return v;
//}
//float FloatBuffer::max() const{
//	float v = get( 0 );
//	for( int i = 1; i < size(); i++ ){
//		v = std::max( v, get( i ) );
//	}
//	return v;
//}
//int FloatBuffer::maxindex() const
//{
//	float v = get( 0 );
//	int m = 0;
//	for( int i = 1; i < size(); i++ ){
//		float vi = get( i );
//		if( v < vi ){
//			v = vi;
//			m = i;
//		}
//	}
//	return m;
//}
//float FloatBuffer::sum() const{
//	float v = 0;
//	for( int i = 0; i < size(); i++ ){
//		v += get( i );
//	}
//	return v;
//}
//void FloatBuffer::setMaxTo1RestTo0( int start, int count ){
//	int max = start;
//	float maxValue = get( start );
//	set( start, 0 );
//	for( int i = start+1; i < start + count; i++ ){
//		float v = get( i );
//		if( maxValue < v ){
//			max = i;
//			maxValue = v;
//		}
//		set( i, 0.0f );
//	}
//	ASSERT( max >= 0 );
//	set( max, 1.0f );
//}
//void FloatBuffer::softmaximize(){
//	float minval = min();
//	if( minval < 0.0f ){
//		for( int i = 0; i < size(); i++ ){
//			set( i, get( i ) - minval );
//		}
//	}
//	for( int i = 0; i < size(); i++ ){
//		ASSERT( 0.0f <= get( i ) );
//	}
//	float sumval = sum();
//	if( sumval <= 0.0f ){
//		std::cout << "FloatBuffer::softmaximize <⁼ 0\n";
//		return;
//	}
//	ASSERT( 0.0f < sumval );
//	for( int i = 0; i < size(); i++ ){
//		set( i, get( i ) / sumval );
//	}
//	ASSERT( std::abs( 1.0f - sum() ) < 0.001f );
//	for( int i = 0; i < size(); i++ ){
//		ASSERT( 0.0f <= get( i ) && get( i ) <= 1.0f );
//	}
//}
//int FloatBuffer::getIndexOfSoftmax(float value) const {
//	ASSERT( 0.0f <= value && value <= 1.0f );
//	for( int i = 0; i < size(); i++ ){
//		float iv =  get( i );
//		if( value <= iv )
//			return i;
//		value -= iv;
//	}
//	ASSERT( false );
//	return size() -1;
//}
//int FloatBuffer::getIndexOfMax() const{
//	return getIndexOfMax( 0, size() -1 );
//}
//int FloatBuffer::getIndexOfMax(int s, int e) const{
//	assert( 0 <= s && s <= e && e < size() );
//	int mi = s;
//	float mv = get( s );
//	for( int i = s + 1; i <= e; i++ ){
//		float v = get( i );
//		if( mv < v ){
//			mi = i;
//			mv = v;
//		}
//	}
//	return mi;

//}
////void FloatBuffer::serialize(SerializationStream& stream) const {
////	stream.writeArraySize( _size );
////	serializeData( stream );
////}
////void FloatBuffer::serializeData(SerializationStream& stream) const {
////	for( int i = 0; i < _size; i++ ){
////		stream.writeFloat( _datas[ i ] );
////	}
////}
////void FloatBuffer::deserializeData(DeserializationStream& stream){
////	for( int i = 0; i < size(); i++ ){
////		set( i, stream.loadFloat() );
////	}
////}
////FloatBuffer FloatBuffer::Deserialize(DeserializationStream& stream){
////	FloatBuffer data( stream.loadArraySize() );
////	data.deserializeData( stream );
////	return data;
////}
////FloatBufferWithHistory::FloatBufferWithHistory(int size, int historySize){
////	_act = new FloatBuffer( size );
////	while( _history.size() < historySize ){
////		_history.append( new FloatBuffer( size ) );
////	}

////}

////FloatBufferWithHistory::~FloatBufferWithHistory()
////{
////	qDeleteAll( _history );
////	delete _act;

////}

////qreal FloatBufferWithHistory::get(int index)
////{
////	return _act->get( index );
////}

////void FloatBufferWithHistory::set(int index, qreal value)
////{
////	_act->set( index, value );

////}

////void FloatBufferWithHistory::pushActToHistory()
////{
////	FloatBuffer* prev = _act;
////	_history.append( _act );
////	_act = _history.takeFirst();
////	_act->copyFrom( *prev );
////}
////void testAdd2( float value, float e0, float e1, float e2, float e3, float e4 ){
////	FloatBuffer buffer( 5 );
////	FloatBufferWriter writer( &buffer );
////	writer.add( value, FloatRange( -0.2f, 0.2f ), 5 );
////	ASSERT( std::abs( buffer.get( 0 ) - e0 ) < 0.01f );
////	ASSERT( std::abs( buffer.get( 1 ) - e1 ) < 0.01f );
////	ASSERT( std::abs( buffer.get( 2 ) - e2 ) < 0.01f );
////	ASSERT( std::abs( buffer.get( 3 ) - e3 ) < 0.01f );
////	ASSERT( std::abs( buffer.get( 4 ) - e4 ) < 0.01f );
////}
////void testAdd(){
////	//         val    -0.2   -0.1   0      0.1    0.2
////	testAdd2( -0.25f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f );
////	testAdd2( -0.20f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f );
////	testAdd2( -0.15f, 0.62f, 0.00f, 0.00f, 0.00f, 0.00f );
////	testAdd2( -0.10f, 1.00f, 0.25f, 0.00f, 0.00f, 0.00f );
////	testAdd2( -0.05f, 1.00f, 0.87f, 0.00f, 0.00f, 0.00f );
////	testAdd2(  0.00f, 1.00f, 1.00f, 0.50f, 0.00f, 0.00f );
////	testAdd2(  0.05f, 1.00f, 1.00f, 1.00f, 0.13f, 0.00f );
////	testAdd2(  0.10f, 1.00f, 1.00f, 1.00f, 0.75f, 0.00f );
////	testAdd2(  0.15f, 1.00f, 1.00f, 1.00f, 1.00f, 0.38f );
////	testAdd2(  0.20f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f );
////	testAdd2(  0.25f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f );

////}
////void FloatBufferWriter::add( float value, const FloatRange& valueRange, int rangeCount ){
////	static bool tested = false;
////	if( !tested ){
////		tested = true;
////		testAdd();
////	}
////	if( !valueRange.isInside( value ) ){
////		qDebug() << "FloatBufferWriter.add" << value << "not in range" << valueRange.toString();
////	}
////	ASSERT( 2 < rangeCount );
////	float rangeStep = ( valueRange.max() - valueRange.min() ) / rangeCount;
////	for( int i = 0; i < rangeCount; i++ ){
////		float cellStart = valueRange.min() + i * rangeStep;
////		float valueInCell = ( value - cellStart ) / rangeStep;
////		if( valueInCell <= 0.0f ){
////			add( 0.0f );
////		} else if( valueInCell <= 1.0f ){
////			add( valueInCell );
////		} else {
////			add( 1.0f );
////		}
////	}
////}

////void testAdd42( float value, float e0, float e1, float e2, float e3, float e4, float e5 ){
////	FloatBuffer buffer( 6 );
////	FloatBufferWriter writer( &buffer );
////	writer.add2( value, FloatRange( -6.0f, 6.0f ), 4 );
////	Q_UNUSED( e0 )
////	Q_UNUSED( e5 )
////	ASSERT( std::abs( buffer.get( 0 ) - e1 ) < 0.01f );
////	ASSERT( std::abs( buffer.get( 1 ) - e2 ) < 0.01f );
////	ASSERT( std::abs( buffer.get( 2 ) - e3 ) < 0.01f );
////	ASSERT( std::abs( buffer.get( 3 ) - e4 ) < 0.01f );
////}
////void testAdd4(){
////	testAdd42( -7.0f, 0.33f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f );
////	testAdd42( -6.0f, 0.00f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f );
////	testAdd42( -5.0f, 0.00f, 0.67f, 1.00f, 0.00f, 0.00f, 0.00f );
////	testAdd42( -4.0f, 0.00f, 0.33f, 1.00f, 0.00f, 0.00f, 0.00f );
////	testAdd42( -3.0f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, 0.00f );
////	testAdd42( -2.0f, 0.00f, 0.00f, 0.67f, 0.00f, 0.00f, 0.00f );
////	testAdd42( -1.0f, 0.00f, 0.00f, 0.33f, 0.00f, 0.00f, 0.00f );
////	testAdd42( 0.0f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f );
////	testAdd42( 1.0f, 0.00f, 0.00f, 0.00f, 0.33f, 0.00f, 0.00f );
////	testAdd42( 2.0f, 0.00f, 0.00f, 0.00f, 0.67f, 0.00f, 0.00f );
////	testAdd42( 3.0f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f );
////	testAdd42( 4.0f, 0.00f, 0.00f, 0.00f, 1.00f, 0.33f, 0.00f );
////	testAdd42( 5.0f, 0.00f, 0.00f, 0.00f, 1.00f, 0.67f, 0.00f );
////	testAdd42( 6.0f, 0.00f, 0.00f, 0.00f, 1.00f, 1.00f, 0.00f );
////	testAdd42( 7.0f, 0.00f, 0.00f, 0.00f, 1.00f, 1.00f, 0.33f );
////}
////void FloatBufferWriter::add2(float value, const FloatRange &valueRange, int rangeCount)
////{
////	static bool tested = false;
////	if( !tested ){
////		tested = true;
////		testAdd4();
////	}
////	static int debugChecked = 0;
////	if( debugChecked < 100 ){
////		if( !valueRange.isInside( value ) ){
////			debugChecked++;
////			qDebug() << "FloatBufferWriter.add2" << value << "not in range" << valueRange.toString();
////		}
////	}
////	ASSERT( 2 < rangeCount );
////	float rangeStep = ( valueRange.max() - valueRange.min() ) / rangeCount;
////	for( int i = 0; i < rangeCount; i++ ){
////		float cellStart = valueRange.min() + i * rangeStep;
////		float valueInCell = ( value - cellStart ) / rangeStep;
////		if( cellStart < 0 ) valueInCell = 1.0f - valueInCell;
////		if( valueInCell <= 0.0f ){
////			add( 0.0f );
////		} else if( valueInCell <= 1.0f ){
////			add( valueInCell );
////		} else {
////			add( 1.0f );
////		}
////	}
////}

////void FloatBufferWriter::add(const QVector2D& vector){
////	add( vector.x() );
////	add( vector.y() );
////}
////void FloatBufferWriter::add( const QVector3D& vector ){
////	add( vector.x() );
////	add( vector.y() );
////	add( vector.z() );
////}
////void FloatBufferWriter::add(const FloatBuffer& data){
////	for( int r = 0; r < data.size(); r++ ){
////		add( data.get( r ) );
////	}
////}
////QVector3D FloatBufferReader::readVector3D(){
////	float x = next();
////	float y = next();
////	float z = next();
////	return QVector3D( x, y, z );
////}

//FloatBufferNormalizer::FloatBufferNormalizer(int size)
//	:_size( size )
//{
//	assert( 0 < _size && _size < Max );
//}
//FloatBufferNormalizer::~FloatBufferNormalizer(){
//}
//void FloatBufferNormalizer::adjust( const FloatBuffer& values ){
//	assert( values.size() == _size );
//	for( int i = 0; i < _size; i++ ){
//		_ranges[i].adjust( values.get( i ) - 0.001f );
//		_ranges[i].adjust( values.get( i ) + 0.001f );
//	}
//}
//void FloatBufferNormalizer::normalize( const FloatBuffer& values, FloatBuffer& factors ){
//	assert( values.size() == _size );
//	assert( factors.size() == _size );
//	for( int i = 0; i < _size; i++ ){
//		float v = values.get( i );
//		float f = _ranges[i].valuetofactor( v );
//		if( f < 0 ){
//			std::cout << "FloatBufferNormalizer::normalize < 0\n";
//			f = 0;
//		}
//		if( 1 < f ){
//			std::cout << "FloatBufferNormalizer::normalize > 1\n";
//			f = 1;
//		}
//		assert( 0 <= f && f <= 1 );
//		factors.set( i, f );
//	}
//}
//void FloatBufferNormalizer::denormalize(const FloatBuffer& factors, FloatBuffer& values) const{
//	assert( values.size() == _size );
//	assert( factors.size() == _size );
//	for( int i = 0; i < _size; i++ ){
//		float f = factors.get( i );
//		assert( 0 <= f && f <= 1 );
//		float v  = _ranges[i].factortovalue( f );
//		values.set( i, v );
//	}
//}
//Vec3 FloatBufferReader::readVec3(){
//	float x = next();
//	float y = next();
//	float z = next();
//	return Vec3( x, y, z );
//	Vec3 v;
//}
//void FloatBufferWriter::add( const Vec3& data ){
//	add( data.x() );
//	add( data.y() );
//	add( data.z() );
//}

FloatBufferWriter::FloatBufferWriter( FloatBuffer* buffer )
	:_buffer( buffer )
	,_index( buffer->first() )
	,_last( buffer->last() )
{
	assert( buffer );
}
FloatBufferWriter::~FloatBufferWriter(){
	if( !atEnd() ){
		logDebug( "FloatBufferWriter~ not at end" );
	}
}
void FloatBufferWriter::add( float v ){
	ASSERT( !atEnd() );
	*_index = v;
	_index++;
}
void FloatBufferWriter::add( const Vec3& v ){
	add( v.x() );
	add( v.y() );
	add( v.z() );
}

FloatBufferReader::FloatBufferReader( const FloatBuffer* buffer )
	:_buffer( buffer )
	,_index( buffer->first() )
	,_last( buffer->last() )
{
	assert( _buffer );
}
FloatBufferReader::~FloatBufferReader(){
	if( !atEnd() ){
		logDebug( "FloatBufferReader~ not at end" );
	}
}
float FloatBufferReader::get(){
	ASSERT( !atEnd() );
	float v = *_index;
	_index++;
	return v;
}
