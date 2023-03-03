#pragma once
#include "Prerequisites.h"
#include "Commons.h"

class Texture
{
public:
	Texture();
	~Texture();

	void 
  init(std::string textureName);

  void
  init(unsigned int width, unsigned int height, DXGI_FORMAT Format, UINT BindFlags);

  void 
  update();
  
  void 
  render();
  
  void 
  destroy();
private:

};
