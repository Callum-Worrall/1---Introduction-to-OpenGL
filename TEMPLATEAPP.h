#include "Application.h"

#pragma once
class TEMPLATEAPP : public Application
{
public:
	TEMPLATEAPP();
	~TEMPLATEAPP();

	virtual bool StartUp();
	virtual bool ShutDown();

	virtual bool Update();
	virtual bool Draw();
};

