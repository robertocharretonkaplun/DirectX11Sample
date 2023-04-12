#pragma once
#include "Prerequisites.h"
#include "Commons.h"
#include "UserInterface.h"

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
	

	/* Este c�digo utiliza iteradores para recorrer los vectores de v�rtices e �ndices, 
	 * evita el uso de variables temporales innecesarias y conversiones de tipo innecesarias, 
	 * y utiliza reservas de capacidad para los vectores. Tambi�n devuelve los datos del modelo 
	 * directamente desde la funci�n Load.	
	*/
	LoadData
	LoadV2(std::string objFileName);

	void
	ui();
private:
	float percentage;
};