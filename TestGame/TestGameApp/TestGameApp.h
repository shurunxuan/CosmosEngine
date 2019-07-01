#ifndef TESTGAMEAPP_H
#define TESTGAMEAPP_H
#include "CosmosEngine/App/App.h"

class TestGameApp :
	public CEApp
{
public:

	~TestGameApp();

	void Init() override;
	void DeInit() override;

};


#endif // TESTGAMEAPP_H
