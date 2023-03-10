#pragma once
#include "Prerequisites.h"
class Device;
class Texture;

class 
RenderTargetView {
public:
	RenderTargetView() = default;
  ~RenderTargetView() {};

	void
  init(Device device, Texture backBuffer, DXGI_FORMAT Format);

  void 
  update();
  
  void 
  render();
  
  void 
  destroy();

public: 
  ID3D11RenderTargetView* m_renderTargetView = nullptr;
};