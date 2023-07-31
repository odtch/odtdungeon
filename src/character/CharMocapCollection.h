#pragma once

#include "CharCore.h"
#include "resource/Collection.h"

class CharMocapCollection : public Collection
{
public:
	static const char* Id;
	static CharMocapCollection* Get();
public:
	explicit CharMocapCollection();
	virtual ~CharMocapCollection() override;
public:
	virtual void import( CollectionImporter& importer ) override;
};
