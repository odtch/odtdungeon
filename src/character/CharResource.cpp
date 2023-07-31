#include "stdodt.h"
#include "CharResource.h"
#include "CharRagdoll.h"
#include "CharAnimation.h"
#include "CharSkin.h"

const char* CharRagdollResourceType::Id = "CharRagdollResourceType";
CharRagdollResourceType::CharRagdollResourceType()
	:ResourceType( 41, Id )
{
}
CharRagdollResourceType::~CharRagdollResourceType(){
}
Resource* CharRagdollResourceType::newInstance(){
	return new CharRagdollType();
}

const char* CharSkinResourceType::Id = "CharSkinResourceType";
CharSkinResourceType::CharSkinResourceType()
	:ResourceType( 42, Id )
{
}
CharSkinResourceType::~CharSkinResourceType(){
}
Resource* CharSkinResourceType::newInstance(){
	return new CharSkinType();
}

const char* CharAnimationResourceType::Id = "CharAnimationResourceType";
CharAnimationResourceType::CharAnimationResourceType()
	:ResourceType( 43, Id )
{
}
CharAnimationResourceType::~CharAnimationResourceType(){
}
Resource* CharAnimationResourceType::newInstance(){
	return new CharAnimation();
}

const char* CharModule::Id = "CharModule";
CharModule::CharModule()
	:Module( Id )
{
}
CharModule::~CharModule(){
}
void CharModule::start(){
	new CharRagdollResourceType();
	new CharAnimationResourceType();
	new CharSkinResourceType();
}
