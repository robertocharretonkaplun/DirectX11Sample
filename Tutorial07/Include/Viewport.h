#pragma once
#include "Prerequisites.h"
class Window;

class 
Viewport {
public:
	Viewport() = default;
	~Viewport() {};

	void
  init(Window window);

  void 
  update();
  
  void 
  render();
  
  void 
  destroy();

public:
  D3D11_VIEWPORT m_viewport;
};
