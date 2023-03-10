#pragma once
#include "Prerequisites.h"
class Device;

class 
InputLayout {
public:
	InputLayout() = default;
	~InputLayout() { SAFE_RELEASE(m_inputLayout); };

	void
  init(Device device, 
       std::vector<D3D11_INPUT_ELEMENT_DESC> Layout, 
       ID3DBlob* VertexShaderData);

  void 
  update();
  
  void 
  render();
  
  void 
  destroy();

public:
	ID3D11InputLayout* m_inputLayout = nullptr;
};
