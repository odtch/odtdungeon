#include "stdodt.h"
#include "Brain.h"
#include "external/genann/genann.c"

BrainState::BrainState(const FloatBuffer& input, const FloatBuffer& output)
	:_input( input )
	,_output( output )
{
}
BrainState::~BrainState(){
}

Brain::Brain(int inputSize, int outputSize)
{
	_input = new FloatBuffer( inputSize );
	_output = new FloatBuffer( outputSize );
//	kad_node_t *t;
//	t = kann_layer_input( _input->size() ); // 784); // for MNIST
//	t = kad_relu(kann_layer_dense(t, 64)); // a 64-neuron hidden layer with ReLU activation
//	t = kann_layer_cost(t, _output->size(), KANN_C_CEM); // softmax output + multi-class cross-entropy cost
//	_kann = kann_new(t, 0);                   // compile the network and collate variables
//	assert( _kann );
	int hiddenLayerCount = 1;
	int hiddenNeuronCount = 50;
	_genann = genann_init( _input->size(), hiddenLayerCount, hiddenNeuronCount, _output->size() );
	assert( _genann );
}
Brain::~Brain(){
	_states.deleteAll();
	odelete( _input );
	odelete( _output );
//	kann_t* kann = (kann_t*) _kann;
//	odelete( kann );
//	_kann = null;
	if( _genann ){
		genann_free( _genann );
		_genann = null;
	}
}
void Brain::tick(){
//	if( _states.size() > 2000 ){
//		delete _states.takeFirst();
//	}
////	kann_t* kann = (kann_t*) _kann;
////	assert( kann );
//	if( !_states.isEmpty() ){
//		for( int l = 0; l < 10; l++ ){
//		_learnindex++;
//		if( _learnindex >= _states.size() )_learnindex = 0;
//		BrainState* state = _states.get( _learnindex );
//		if( state->_scorediff == 0 ){
//		} else {
//			float learning_rate = 0.00001f;
//			FloatBuffer output( state->_output );
//			if( state->_scorediff < 0 ){
//				for( int o = 0; o < output.size(); o++ ){
//					output.set( o, 1.0f - output.get( o ) );
//				}

//			}
//			//* ( state->_scorediff > 0 ? 1 : -1 );
//			genann_train( _genann, state->_input.data(), output.data(), learning_rate );
////			int training_sample_count = 1;
////			float **x; // model input, of size n * 784
////			float **y; // model output, of size n * 10
////			// fill in x and y here and then call:
////			x = (float**) _input->data();
////			y = (float**) _output->data();
////
////			kann_train_fnn1( kann, learning_rate, 1, 1, 1, 0.001f, training_sample_count, x, y );
//			int train;
//		}
//		}
//	}
	float const* genann_output = genann_run( _genann, _input->data() );
	if( genann_output == null ){
		logError( "genann_run failed" );
		assert( false );
	}
	_output->copyFrom( genann_output );
//	if( _rnd.getf() < 0.1f ){
//		int o = _rnd.geti( 0, _output->size() -1 );
//		float v = _output->get( o ) + _rnd.getf( -0.1f, 0.1f );
//		_output->set( o, v );
//	}
//	if( _rnd.getf() < 0.01f ){
//		for( int o = 0; o < _output->size(); o++ ){
//			float v = _rnd.getf();
//			_output->set( o, v );
//		}
//	}
//	float* input = _input->data();
//	// fill in x here and then call:
//	const float* output = kann_apply1( kann, input );
//	assert( output );
//	_output->copyFrom( output );
//	auto state = new BrainState( *_input, *_output );
//	_states.add( state );
}
void Brain::setScore( float score ){
	if( _states.isEmpty() )return;
	assert( !_states.isEmpty() );
	BrainState* prevstate = _states.last();
	assert( prevstate->_score == 0 );
	prevstate->_score = score;
	int backcompare = 15;
	for( int b = _states.size() - backcompare; b < _states.size() -1 ; b++ ){
		if( 0 <= b ){
			BrainState* backstate = _states.get( b );
			//assert( backstate->_scorediff == 0 );
			backstate->_scorediff += score - backstate->_score;
		}
	}
	if( backcompare < _states.size() ){
	}
}
float calcScore( Brain& brain, const List<BrainState*>& states ){
	float total = 0;
	for( BrainState* state : states ){
		brain.input().copyFrom( state->input() );
		brain.tick();
		float ts = 0;
		for( int o = 0; o < brain.output().size(); o++ ){
			float i = brain.output().get( o );
			float e = state->output().get( o );
			float s = - std::fabs( e - i );
			ts += s;
			//logDebug( "  i", o, i, e );
		}
		total += ts;
	}
	return total;
}

void Brain::learn( const List<BrainState*>& states, int count, float learningrate ){
	//logDebug( "Brain::learn", states.size() );
	float prev_score = calcScore( *this, states );
	Random rnd;
	for( int s = 0; s < count; s++ ){
		//logDebug( "Brain::learn", s, count, learningrate );
		List<BrainState*> pendings;
		pendings.addAll( states );
		while( !pendings.isEmpty() ){
			BrainState* state = rnd.oneof( pendings );
			pendings.remove( state );
			genann_train( _genann, state->_input.data(), state->_output.data(), learningrate );
		}
	}
	float new_score = calcScore( *this, states );
	logDebug( "Brain::learned", prev_score, new_score, ( new_score - prev_score ), learningrate );
}

