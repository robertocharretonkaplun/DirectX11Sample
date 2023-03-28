#include "ConstantBuffer.h"
#include "Device.h"
#include "DeviceContext.h"

void 
ConstantBuffer::init(Device device, unsigned int ByteWidth) {
  if (device.m_device == nullptr) {
    WARNING("ERROR: ConstantBuffer::init : [CREATION OF RESOURCE : FALSE] [CHECK FOR Device device] \n");
    exit(1);
  }
  else if (ByteWidth == 0) {
    WARNING("ERROR: ConstantBuffer::init : [CREATION OF RESOURCE : FALSE] [CHECK FOR unsigned int ByteWidth] \n");
    exit(1);
  }

  HRESULT hr = S_OK;
  D3D11_BUFFER_DESC ConstantBufferDesc;
  memset(&ConstantBufferDesc, 0, sizeof(ConstantBufferDesc));
  ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  ConstantBufferDesc.ByteWidth = ByteWidth;
  ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  ConstantBufferDesc.CPUAccessFlags = 0;
  hr = device.CreateBuffer(&ConstantBufferDesc, nullptr, &m_constantBuffer);
  if (FAILED(hr)) {
    WARNING("ERROR: ConstantBuffer::init : [CREATION OF RESOURCE : FALSE] [CHECK FOR CreateBuffer()] \n");
    exit(1);
  }
}

void 
ConstantBuffer::update(DeviceContext& deviceContext,
                       unsigned int DstSubresource,
                       const D3D11_BOX* pDstBox,
                       const void* pSrcData,
                       unsigned int SrcRowPitch,
                       unsigned int SrcDepthPitch) {
  deviceContext.UpdateSubresource(m_constantBuffer, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
}

void ConstantBuffer::render(DeviceContext& deviceContext)
{
}

void 
ConstantBuffer::destroy() {
  SAFE_RELEASE(m_constantBuffer);
}

void 
ConstantBuffer::VSSetConstantBuffers(DeviceContext& deviceContext, 
                                     unsigned int StartSlot, 
                                     unsigned int NumBuffers) {
  deviceContext.VSSetConstantBuffers(StartSlot, NumBuffers, &m_constantBuffer);
}

void 
ConstantBuffer::PSSetConstantBuffers(DeviceContext& deviceContext, 
                                     unsigned int StartSlot, 
                                     unsigned int NumBuffers) {
  deviceContext.PSSetConstantBuffers(StartSlot, NumBuffers, &m_constantBuffer);
}
