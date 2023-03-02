#include "Prerequisites.h"
#include "Commons.h"
#include "OBJ_Loader.h"
#pragma once

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	void
	init();

	void
	update();

	void
	render();

	void
	destroy();

	LoadData
	Load();

private:

};