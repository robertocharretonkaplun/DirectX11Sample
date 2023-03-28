#pragma once
#include "Prerequisites.h"
class Device;
class DeviceContext;

class 
ConstantBuffer {
public:
	ConstantBuffer() = default;
  ~ConstantBuffer() {};

	void
  init(Device device, unsigned int ByteWidth);

  void
  update(DeviceContext& deviceContext, 
         unsigned int DstSubresource,
         const D3D11_BOX* pDstBox,
         const void* pSrcData,
         unsigned int SrcRowPitch,
         unsigned int SrcDepthPitch);

  void
  render(DeviceContext& deviceContext);

  void
  destroy();

  void
  VSSetConstantBuffers(DeviceContext& deviceContext, 
                       unsigned int StartSlot,
                       unsigned int NumBuffers);

  void
  PSSetConstantBuffers(DeviceContext& deviceContext, 
                       unsigned int StartSlot,
                       unsigned int NumBuffers);
public:
  ID3D11Buffer* m_constantBuffer = nullptr;
};
