#include "stdodt.h"

#include "DummyRenderer.h"

DummyRenderer::DummyRenderer(){
}
DummyRenderer::~DummyRenderer(){
}
void DummyRenderer::run(){
    while( !should_stop() ){
        logDebug( "DummyRenderer::run" );
        sleep_ms( 1300 );
    }
}

