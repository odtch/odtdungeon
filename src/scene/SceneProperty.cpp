#include "stdodt.h"

#include "SceneProperty.h"
#include "SceneNode.h"

SceneProperty::SceneProperty(SceneNode* node)
	:_node( asserted( node ) )
{
	_node->registerProperty( this );
}
SceneProperty::~SceneProperty(){
}
