#include "stdodt.h"

#include "Module.h"

Module::Module( const char* id )
	:Singleton( id )
{
	Modules::Get()->internalRegister( this );
}
Module::~Module(){
}

const char* Modules::Id = "Modules";
Modules* Modules::Get(){
	return Singleton::Get<Modules>();
}
Modules::Modules()
	:Singleton( Id )
{
}
Modules::~Modules(){
	_modules.removeAll();;
}
void Modules::internalRegister( Module* module ){
	_modules.add( module );
}
void Modules::start(){
	for( Module* module : _modules ){
        module->start();
	}
}
void Modules::stop(){
	for( Module* module : _modules ){
        module->stop();
	}
}
