#include "Model.h"
#include "Device.h"
#include "DeviceContext.h"

void 
Model::init(Device device, LoadData loadData) {
  m_loadData = loadData;
  g_vertexBuffer.init(device, loadData);

  g_indexBuffer.init(device, loadData);

  g_modelBuffer.init(device, sizeof(CBChangesEveryFrame));

  m_transform.init();
}

void 
Model::update(DeviceContext& deviceContext,
                       unsigned int DstSubresource,
                       const D3D11_BOX* pDstBox,
                       const void* pSrcData,
                       unsigned int SrcRowPitch,
                       unsigned int SrcDepthPitch) {
  g_modelBuffer.update(deviceContext, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
}

void 
Model::render(DeviceContext& deviceContext, unsigned int StartSlot) {
  g_vertexBuffer.render(deviceContext, StartSlot);
  g_indexBuffer.render(deviceContext);
  g_modelBuffer.VSSetConstantBuffers(deviceContext, 1, 1);
  g_modelBuffer.PSSetConstantBuffers(deviceContext, 1, 1);

  deviceContext.DrawIndexed(m_loadData.numIndex, 0, 0);
}

void 
Model::destroy() {
  g_modelBuffer.destroy();
  g_vertexBuffer.destroy();
  g_indexBuffer.destroy();
}
