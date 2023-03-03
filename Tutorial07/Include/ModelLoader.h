#pragma once
#include "Prerequisites.h"
#include "Commons.h"


class 
ModelLoader {
public:
	ModelLoader() ;
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
	Load(std::string objFileName);

private:
};