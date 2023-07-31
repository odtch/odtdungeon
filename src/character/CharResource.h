#pragma once

#include "CharCore.h"
#include "resource/ResourceType.h"
#include "utils/Module.h"

class CharRagdollResourceType : public ResourceType
{
public:
	static const char* Id;
public:
	explicit CharRagdollResourceType();
	virtual ~CharRagdollResourceType() override;
public:
	virtual Resource* newInstance() override;
};

class CharSkinResourceType : public ResourceType
{
public:
	static const char* Id;
public:
	explicit CharSkinResourceType();
	virtual ~CharSkinResourceType() override;
public:
	virtual Resource* newInstance() override;
};

class CharAnimationResourceType : public ResourceType
{
public:
	static const char* Id;
public:
	explicit CharAnimationResourceType();
	virtual ~CharAnimationResourceType() override;
public:
	virtual Resource* newInstance() override;
};


class CharModule : public Module
{
public:
	static const char* Id;
	static CharModule* Get(){ return Singleton::Get<CharModule>(); }
public:
	explicit CharModule();
	virtual ~CharModule() override;
public:
	virtual void start() override;
};
