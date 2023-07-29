#pragma once

#include "resource/Collection.h"

class DungeonCollection : public Collection
{
public:
	static const char* Id;
	static DungeonCollection* Get();
public:
	explicit DungeonCollection();
	virtual ~DungeonCollection() override;
public:
	virtual void import( CollectionImporter& importer ) override;
};
