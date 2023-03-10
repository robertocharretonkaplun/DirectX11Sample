#pragma once
#include "Prerequisites.h"
class Device;

class 
SamplerState {
public:
	SamplerState() = default;
	~SamplerState() {};

	void 
  init(Device device);

  void 
  update();
  
  void 
  render();
  
  void 
  destroy();

public:
	ID3D11SamplerState* m_sampler = nullptr;
};
