#pragma once

#include "Singleton.h"
#include "String.h"

class Module : public Singleton {
	DEBUGCOUNTER(Module)
private:
public:
	explicit Module( const char* id );
	virtual ~Module() override;
public:
    virtual void start(){}
    virtual void stop(){}
};

class Modules : public Singleton {
	DEBUGCOUNTER( Modules )
public:
	static const char* Id;
	static Modules* Get();
private:
	List<Module*> _modules;
public:
	explicit Modules();
	virtual ~Modules() override;
public:
	void internalRegister( Module* module );
public:
    void start();
    void stop();
};

