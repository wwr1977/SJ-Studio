#pragma once
#include "ObjBase.h"

class CCoreBuilder :public CObjBase
{
public:
	virtual bool CoreBuild() = 0;

public:
	CCoreBuilder();
	~CCoreBuilder();
};

