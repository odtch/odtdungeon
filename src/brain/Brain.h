#pragma once

#include "BrainCore.h"

struct genann;

class BrainState
{
	DEBUGCOUNTER( BrainState );
private:
	FloatBuffer _input;
	FloatBuffer _output;
	float _score = 0;
	float _scorediff = 0;
public:
	explicit BrainState( const FloatBuffer& input, const FloatBuffer& output );
	~BrainState();
public:
	friend class Brain;
};

//private:
//private:
//	MeshPNT* _targetMesh = null;
//	Material* _targetMaterial = null;
//	ObjectType* _targetObject = null;
//	ObjectRenderablePropertyType* _targetRenderable = null;
//public:
//	explicit BrainType( const char* id );
//	virtual ~BrainType() override;
//public:
//	virtual Object* newInstance() override;
//public:
//	friend class Brain;
//};

class Brain
{
	DEBUGCOUNTER( Brain );
private:
	FloatBuffer* _input = null;
	FloatBuffer* _output = null;
private:
	List<BrainState*> _states;
private:
	Random _rnd;
	int _learnindex = 0;
private:
	genann* _genann;
//    double* _input;
//    double* _output;
//	void* _kann = null;
public:
	explicit Brain( int inputSize, int outputSize );
	virtual ~Brain();
public:
	FloatBuffer& input(){ return *_input; }
	void tick();
	FloatBuffer& output(){ return *_output; }
public:
	void setScore( float score );
//protected:
//	virtual void onAddedToScene( Scene* scene ) override;
//public:
//	virtual void animate( float dt ) override;
};
