#include "Prerequisites.h"
#pragma once

class 
DeviceContext {
public:
	DeviceContext() = default;
	~DeviceContext() { SAFE_RELEASE(m_deviceContext); };

	void 
  init();

  void 
  update();
  
  void 
  render();
  
  void 
  destroy();

  void
  PSSetShaderResources(unsigned int StartSlot, 
                       unsigned int NumViews, 
                       ID3D11ShaderResourceView* const* ppShaderResourceViews);

  void 
  PSSetSamplers(unsigned int StartSlot, 
                unsigned int NumSamplers, 
                ID3D11SamplerState* const* ppSamplers);

  void 
  DrawIndexed(unsigned int IndexCount, 
              unsigned int StartIndexLocation, 
              unsigned int BaseVertexLocation);

public:
	ID3D11DeviceContext* m_deviceContext = nullptr;
};